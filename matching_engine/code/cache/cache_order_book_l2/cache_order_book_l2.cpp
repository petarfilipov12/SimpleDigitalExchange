#include "cache_order_book_l2.h"

#include "unistd.h"

#include "event.h"

#include "json.h"


CacheOrderBookL2::CacheOrderBookL2(const std::string& symbol)
{
    this->symbol = symbol;
}

CacheOrderBookL2::~CacheOrderBookL2() {}

returnType CacheOrderBookL2::OrderAdded(const Order& order)
{
    returnType ret = RET_NOT_OK;

    if (ORDER_SIDE_BUY == order.order_side)
    {
        this->bid_book_l2_look.lock();
        this->bid_book_l2[order.price] += (order.quantity - order.filled);
        this->bid_book_l2_look.unlock();

        ret = RET_OK;
    }
    else if (ORDER_SIDE_SELL == order.order_side)
    {
        this->ask_book_l2_look.lock();
        this->ask_book_l2[order.price] += (order.quantity - order.filled);
        this->ask_book_l2_look.unlock();

        ret = RET_OK;
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
            this->bid_book_l2_look.lock();
            this->bid_book_l2[order.price] -= (order.quantity - order.filled);
            if (this->bid_book_l2[order.price] <= 0.0)
            {
                this->bid_book_l2.erase(order.price);
            }
            this->bid_book_l2_look.unlock();

            ret = RET_OK;
        }
        else if (ORDER_SIDE_SELL == order.order_side)
        {
            this->ask_book_l2_look.lock();
            this->ask_book_l2[order.price] -= (order.quantity - order.filled);
            if (this->ask_book_l2[order.price] <= 0.0)
            {
                this->ask_book_l2.erase(order.price);
            }
            this->ask_book_l2_look.unlock();

            ret = RET_OK;
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
            this->bid_book_l2_look.lock();
            if(this->bid_book_l2[price] >= (quantity - 0.1))
            {
                this->bid_book_l2[price] -= quantity;
                if (this->bid_book_l2[price] <= 0.1)
                {
                    this->bid_book_l2.erase(price);
                }

                flag = true;
            }
            this->bid_book_l2_look.unlock();

            if(false == flag)
            {
                usleep(10);
            }
        }
    }
    else if (ORDER_SIDE_SELL == book_order_side)
    {
        while(false == flag)
        {
            this->ask_book_l2_look.lock();
            if(this->ask_book_l2[price] >= (quantity - 0.1))
            {
                this->ask_book_l2[price] -= quantity;
                if (this->ask_book_l2[price] <= 0.1)
                {
                    this->ask_book_l2.erase(price);
                }

                flag = true;
            }
            this->ask_book_l2_look.unlock();

            if(false == flag)
            {
                usleep(10);
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

    this->bid_book_l2_look.lock();
    this->ask_book_l2_look.lock();
    temp_bid_book_l2 = this->bid_book_l2;
    temp_ask_book_l2 = this->ask_book_l2;
    this->bid_book_l2_look.unlock();
    this->ask_book_l2_look.unlock();

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
/**************************/
/*Init Func implementation*/
/**************************/
void CacheOrderBookL2::init(EventBus& event_bus, receiverId_t receiver_id)
{
    EventReceiver event_receiver = EventReceiver(
        receiver_id, 
        std::bind(&CacheOrderBookL2::EventHandler, this, std::placeholders::_1),
        std::bind(&CacheOrderBookL2::Filter, this, std::placeholders::_1)
    );

    event_bus.AddReceiver(event_receiver);

    event_bus.Subscribe(receiver_id, EVENT_ID_MAKER_ORDER_ADDED);
    event_bus.Subscribe(receiver_id, EVENT_ID_MAKER_ORDER_CANCELED);
    event_bus.Subscribe(receiver_id, EVENT_ID_ORDER_FILLED);
    event_bus.Subscribe(receiver_id, EVENT_ID_GET_ORDER_BOOK);
}

/******************************/
/*Event_Handler Implementation*/
/******************************/
void CacheOrderBookL2::EventHandler_OrderAdded(Event& event)
{
    json j_data = event.GetJsonData();
    this->OrderAdded(Order::ConvertJsonToOrder(j_data));
}

void CacheOrderBookL2::EventHandler_OrderCanceled(Event& event)
{
    json j_data = event.GetJsonData();
    this->OrderCanceled(Order::ConvertJsonToOrder(j_data));
}

void CacheOrderBookL2::EventHandler_OrderFilled(Event& event)
{
    this->OrderFilled(
        event.GetJsonData()["price"], event.GetJsonData()["quantity"],
        event.GetJsonData()["book_order"]["order_side"]
    );
}

void CacheOrderBookL2::EventHandler_GetOrderBookL2(Event& event)
{
    json l2_book;
    returnType ret = RET_NOT_OK;

    if(nullptr != event.GetResponceDataPtr())
    {
        ret = this->GetOrderBookL2(l2_book);

        if(RET_OK == ret)
        {
            (*event.GetResponceDataPtr())["data"] = l2_book;
        }

        (*event.GetResponceDataPtr())["error"] = ret;
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

/***********************/
/*Filter Implementation*/
/***********************/
returnType CacheOrderBookL2::Filter(Event& event)
{
    returnType ret = RET_NOT_OK;

    if(this->symbol == event.GetJsonData()["symbol"])
    {
        ret = RET_OK;
    }

    return ret;
}