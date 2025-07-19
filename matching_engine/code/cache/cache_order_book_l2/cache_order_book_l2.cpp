#include "cache_order_book_l2.h"

#include <thread>
#include <chrono>

#include "event.h"

#include "json.h"


CacheOrderBookL2::CacheOrderBookL2(const std::string& symbol, EventBus& event_bus, receiverId_t receiver_id):
Cache(
    symbol,
    EventReceiver(
        receiver_id, 
        std::bind(&CacheOrderBookL2::EventHandler, this, std::placeholders::_1),
        std::bind(&CacheOrderBookL2::Filter, this, std::placeholders::_1)
    )
)
{
    Cache::init(event_bus, {
        EVENT_ID_MAKER_ORDER_ADDED,
        EVENT_ID_MAKER_ORDER_CANCELED,
        EVENT_ID_ORDER_FILLED,
        EVENT_ID_GET_ORDER_BOOK
    });
}

CacheOrderBookL2::~CacheOrderBookL2() {}

returnType CacheOrderBookL2::OrderAdded(const Order& order)
{
    returnType ret = RET_NOT_OK;

    if (ORDER_SIDE_BUY == order.order_side)
    {
        ret = RET_OK;

        std::lock_guard<std::mutex> bid_book_l2_lock(this->bid_book_l2_mtx);
        this->bid_book_l2[order.price] += (order.quantity - order.filled);
    }
    else if (ORDER_SIDE_SELL == order.order_side)
    {
        ret = RET_OK;

        std::lock_guard<std::mutex> ask_book_l2_lock(this->ask_book_l2_mtx);
        this->ask_book_l2[order.price] += (order.quantity - order.filled);
    }
    else
    {
        // Error
    }

    return ret;
}

returnType CacheOrderBookL2::OrderCanceled(const Order& order)
{
    returnType ret = RET_NOT_OK;

    if (ORDER_TYPE_LIMIT == order.order_type)
    {
        if (ORDER_SIDE_BUY == order.order_side)
        {
            ret = RET_OK;

            std::lock_guard<std::mutex> bid_book_l2_lock(this->bid_book_l2_mtx);
            this->bid_book_l2[order.price] -= (order.quantity - order.filled);
            if (this->bid_book_l2[order.price] <= 0.0)
            {
                this->bid_book_l2.erase(order.price);
            }
        }
        else if (ORDER_SIDE_SELL == order.order_side)
        {
            ret = RET_OK;

            std::lock_guard<std::mutex> ask_book_l2_lock(this->ask_book_l2_mtx);
            this->ask_book_l2[order.price] -= (order.quantity - order.filled);
            if (this->ask_book_l2[order.price] <= 0.0)
            {
                this->ask_book_l2.erase(order.price);
            }
        }
        else
        {
            // Error
        }
    }

    return ret;
}

returnType CacheOrderBookL2::OrderFilled(const std::string& price, const float quantity, const orderSide_t book_order_side)
{
    returnType ret = RET_NOT_OK;
    bool flag = false;

    if (ORDER_SIDE_BUY == book_order_side)
    {
        while(false == flag)
        {
            std::unique_lock<std::mutex> bid_book_l2_lock(this->bid_book_l2_mtx);
            if(this->bid_book_l2[price] >= (quantity - 0.1))
            {
                this->bid_book_l2[price] -= quantity;
                if (this->bid_book_l2[price] <= 0.1)
                {
                    this->bid_book_l2.erase(price);
                }

                flag = true;
            }
            bid_book_l2_lock.unlock();

            if(false == flag)
            {
                std::this_thread::sleep_for(std::chrono::microseconds(10));
            }
        }
    }
    else if (ORDER_SIDE_SELL == book_order_side)
    {
        while(false == flag)
        {
            std::unique_lock<std::mutex> ask_book_l2_lock(this->ask_book_l2_mtx);
            if(this->ask_book_l2[price] >= (quantity - 0.1))
            {
                this->ask_book_l2[price] -= quantity;
                if (this->ask_book_l2[price] <= 0.1)
                {
                    this->ask_book_l2.erase(price);
                }

                flag = true;
            }
            ask_book_l2_lock.unlock();

            if(false == flag)
            {
                std::this_thread::sleep_for(std::chrono::microseconds(10));
            }
        }
    }
    else
    {
        //Nothing to do
    }

    return ret;
}

returnType CacheOrderBookL2::GetOrderBookL2(json& l2_book)const
{
    std::map<std::string, float, std::greater<std::string> > temp_bid_book_l2;
    std::map<std::string, float, std::less<std::string> > temp_ask_book_l2;

    std::unique_lock<std::mutex> bid_book_l2_lock(this->bid_book_l2_mtx);
    std::unique_lock<std::mutex> ask_book_l2_lock(this->ask_book_l2_mtx);
    temp_bid_book_l2 = this->bid_book_l2;
    temp_ask_book_l2 = this->ask_book_l2;
    bid_book_l2_lock.unlock();
    ask_book_l2_lock.unlock();

    l2_book["bid"] = {};
    if (!this->bid_book_l2.empty())
    {
        l2_book["bid"] = temp_bid_book_l2;
    }

    l2_book["ask"] = {};
    if (!this->ask_book_l2.empty())
    {
        l2_book["ask"] = temp_ask_book_l2;
    }

    return RET_OK;
}

/******************************/
/*Event_Handler Implementation*/
/******************************/
void CacheOrderBookL2::EventHandler_OrderAdded(Event& event)
{
    json j_data = event.GetDataIn();
    this->OrderAdded(Order::ConvertJsonToOrder(j_data));
}

void CacheOrderBookL2::EventHandler_OrderCanceled(Event& event)
{
    json j_data = event.GetDataIn();
    this->OrderCanceled(Order::ConvertJsonToOrder(j_data));
}

void CacheOrderBookL2::EventHandler_OrderFilled(Event& event)
{
    this->OrderFilled(
        event.GetDataIn()["price"], event.GetDataIn()["quantity"],
        event.GetDataIn()["book_order"]["order_side"]
    );
}

void CacheOrderBookL2::EventHandler_GetOrderBookL2(Event& event)
{
    json l2_book;
    returnType ret = RET_NOT_OK;

    if(nullptr != event.GetDataOut())
    {
        ret = this->GetOrderBookL2(l2_book);

        if(RET_OK == ret)
        {
            (*event.GetDataOut())["data"] = l2_book; //std::move
        }

        (*event.GetDataOut())["error"] = ret;
    }
}

void CacheOrderBookL2::EventHandler(Event event)
{
    switch(event.GetEventId())
    {
        case EVENT_ID_MAKER_ORDER_ADDED:
            this->EventHandler_OrderAdded(event);
            break;
        case EVENT_ID_MAKER_ORDER_CANCELED:
            this->EventHandler_OrderCanceled(event);
            break;
        case EVENT_ID_ORDER_FILLED:
            this->EventHandler_OrderFilled(event);
            break;
        case EVENT_ID_GET_ORDER_BOOK:
            this->EventHandler_GetOrderBookL2(event);
            break;
        default:
            break;
    }
}