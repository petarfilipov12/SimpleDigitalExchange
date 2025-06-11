#include "cache_order_book_l2.h"

#include "event.h"
#include "globals.h"

using namespace std;

#include <nlohmann/json.hpp>
using json = nlohmann::json;

ReturnType Cache_OrderBookL2::OrderAdded(const Order& order)
{
    ReturnType ret = RET_NOT_OK;

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

ReturnType Cache_OrderBookL2::OrderCanceled(const Order& order)
{
    ReturnType ret = RET_NOT_OK;

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

ReturnType Cache_OrderBookL2::OrderFilled(const string& price, const float quantity, const enum eOrderSide_t book_order_side)
{
    ReturnType ret = RET_NOT_OK;
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

ReturnType Cache_OrderBookL2::GetOrderBookL2(json& l2_book)const
{
    map<string, float, greater<string> > temp_bid_book_l2;
    map<string, float, less<string> > temp_ask_book_l2;

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
void Cache_OrderBookL2::init(EventBus& event_bus)
{
    event_bus.AddReceiver(RECEIVER_ID_CACHE_ORDER_BOOK_L2, Cache_OrderBookL2::EventHandler);
    
    event_bus.Subscribe(RECEIVER_ID_CACHE_ORDER_BOOK_L2, EVENT_ID_MAKER_ORDER_ADDED);
    event_bus.Subscribe(RECEIVER_ID_CACHE_ORDER_BOOK_L2, EVENT_ID_MAKER_ORDER_CANCELED);
    event_bus.Subscribe(RECEIVER_ID_CACHE_ORDER_BOOK_L2, EVENT_ID_ORDER_FILLED);
    event_bus.Subscribe(RECEIVER_ID_CACHE_ORDER_BOOK_L2, EVENT_ID_GET_ORDER_BOOK);
}

/******************************/
/*Event_Handler Implementation*/
/******************************/
static inline void Cache_OrderBookL2_EventHandler_OrderAdded(Event event)
{
    json j_data = event.GetJsonData();
    cache_order_book_l2.OrderAdded(Order::ConvertJsonToOrder(j_data));
}

static inline void Cache_OrderBookL2_EventHandler_OrderCanceled(Event event)
{
    json j_data = event.GetJsonData();
    cache_order_book_l2.OrderCanceled(Order::ConvertJsonToOrder(j_data));
}

static inline void Cache_OrderBookL2_EventHandler_OrderFilled(Event event)
{
    cache_order_book_l2.OrderFilled(
        event.GetJsonData()["price"], event.GetJsonData()["quantity"],
        event.GetJsonData()["book_order"]["order_side"]
    );
}

static inline void Cache_OrderBookL2_EventHandler_GetOrderBookL2(Event event)
{
    json l2_book;
    ReturnType ret = RET_NOT_OK;

    if(nullptr != event.GetResponceDataPtr())
    {
        ret = cache_order_book_l2.GetOrderBookL2(l2_book);

        if(RET_OK == ret)
        {
            (*event.GetResponceDataPtr())["data"] = l2_book;
        }

        (*event.GetResponceDataPtr())["error"] = ret;
    }
}

void Cache_OrderBookL2::EventHandler(Event event)
{
    switch(event.GetEventId())
    {
        case EVENT_ID_MAKER_ORDER_ADDED:
            Cache_OrderBookL2_EventHandler_OrderAdded(event);
            break;
        case EVENT_ID_MAKER_ORDER_CANCELED:
            Cache_OrderBookL2_EventHandler_OrderCanceled(event);
            break;
        case EVENT_ID_ORDER_FILLED:
            Cache_OrderBookL2_EventHandler_OrderFilled(event);
            break;
        case EVENT_ID_GET_ORDER_BOOK:
            Cache_OrderBookL2_EventHandler_GetOrderBookL2(event);
            break;
        default:
            break;
    }
}