#ifndef CACHE_ORDER_BOOK_L2_H
#define CACHE_ORDER_BOOK_L2_H

#include "types.h"
#include <map>
#include <string>
#include "order.h"
#include "event.h"
#include "globals.h"

using namespace std;

#include<nlohmann/json.hpp>
using json = nlohmann::json;

class Cache_OrderBookL2
{
    private:
        map<string, float, greater<string> > bid_book_l2;
        map<string, float, less<string> > ask_book_l2;

    public:
        Return_Type OrderAdded(Order order)
        {
            Return_Type ret = RET_NOT_OK;

            if(ORDER_TYPE_LIMIT == order.order_type)
            {
                if(ORDER_SIDE_BUY == order.order_side)
                {
                    this->bid_book_l2[order.price] += order.quantity;

                    ret = RET_OK;
                }
                else if(ORDER_SIDE_SELL == order.order_side)
                {
                    this->ask_book_l2[order.price] += order.quantity;

                    ret = RET_OK;
                }
                else
                {
                    //Error
                }
            }

            return ret;
        }

        Return_Type OrderCanceled(int order_id)
        {
            Return_Type ret = RET_NOT_OK;
            json responce_data;

            Event event(EVENT_ID_GET_ORDER, {{"order_id", order_id}}, &responce_data);

            event_bus.Send(event);

            while (responce_data.empty())
            {
                // Timer
            }

            if (RET_OK == responce_data["error"])
            {
                Order order = Order::ConvertJsonToOrder(responce_data["error"]["data"]);

                if (ORDER_TYPE_LIMIT == order.order_type)
                {
                    if (ORDER_SIDE_BUY == order.order_side)
                    {
                        this->bid_book_l2[order.price] -= (order.quantity - order.filled);
                        if(this->bid_book_l2[order.price] <= 0.0)
                        {
                            this->bid_book_l2.erase(order.price);
                        }

                        ret = RET_OK;
                    }
                    else if (ORDER_SIDE_SELL == order.order_side)
                    {
                        this->ask_book_l2[order.price] -= (order.quantity - order.filled);
                        if(this->ask_book_l2[order.price] <= 0.0)
                        {
                            this->ask_book_l2.erase(order.price);
                        }

                        ret = RET_OK;
                    }
                    else
                    {
                        // Error
                    }
                }
            }

            return ret;
        }

        Return_Type OrderFilled(int bid_order_id, int ask_order_id, float quantity)
        {
            Return_Type ret = RET_NOT_OK;
            Order order;
            json bid_order_responce_data;
            json ask_order_responce_data;

            Event event(EVENT_ID_GET_ORDER, {{"order_id", bid_order_id}}, &bid_order_responce_data);
            event_bus.Send(event);

            Event event(EVENT_ID_GET_ORDER, {{"order_id", ask_order_id}}, &ask_order_responce_data);
            event_bus.Send(event);

            while ( (bid_order_responce_data.empty()) || (ask_order_responce_data.empty()) )
            {
                // Timer
            }

            ret = bid_order_responce_data["error"];
            if (RET_OK == ret)
            {
                order = Order::ConvertJsonToOrder(bid_order_responce_data["error"]["data"]);
                if (ORDER_TYPE_LIMIT == order.order_type)
                {
                    this->bid_book_l2[order.price] -= quantity;
                    if (this->bid_book_l2[order.price] <= 0.0)
                    {
                        this->bid_book_l2.erase(order.price);
                    }
                }
            }
            else
            {
                return ret;
            }

            ret = ask_order_responce_data["error"];
            if (RET_OK == ret)
            {
                order = Order::ConvertJsonToOrder(ask_order_responce_data["error"]["data"]);
                if (ORDER_TYPE_LIMIT == order.order_type)
                {
                    this->ask_book_l2[order.price] -= quantity;
                    if (this->ask_book_l2[order.price] <= 0.0)
                    {
                        this->ask_book_l2.erase(order.price);
                    }
                }
            }

            return ret;
        }

        Return_Type GetOrderBook(json *l2_book)
        {
            (*l2_book)["bid"] = {};
            if(!this->bid_book_l2.empty())
            {
                (*l2_book)["bid"] = this->bid_book_l2;
            }

            (*l2_book)["ask"] = {};
            if(!this->ask_book_l2.empty())
            {
                (*l2_book)["ask"] = this->ask_book_l2;
            }

            return RET_OK;
        }
};

#endif