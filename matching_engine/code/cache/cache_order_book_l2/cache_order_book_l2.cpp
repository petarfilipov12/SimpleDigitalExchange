#include "cache_order_book_l2.h"

#include "event.h"
#include "globals.h"

using namespace std;

#include <nlohmann/json.hpp>
using json = nlohmann::json;

Return_Type Cache_OrderBookL2::OrderAdded(Order order)
{
    Return_Type ret = RET_NOT_OK;

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

Return_Type Cache_OrderBookL2::OrderCanceled(Order order)
{
    Return_Type ret = RET_NOT_OK;

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

Return_Type Cache_OrderBookL2::OrderFilled(string price, float quantity, enum eOrderSide_t book_order_side)
{
    Return_Type ret = RET_NOT_OK;
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

Return_Type Cache_OrderBookL2::GetOrderBookL2(json *l2_book)
{
    map<string, float, greater<string> > temp_bid_book_l2;
    map<string, float, less<string> > temp_ask_book_l2;

    this->bid_book_l2_look.lock();
    this->ask_book_l2_look.lock();
    temp_bid_book_l2 = this->bid_book_l2;
    temp_ask_book_l2 = this->ask_book_l2;
    this->bid_book_l2_look.unlock();
    this->ask_book_l2_look.unlock();

    (*l2_book)["bid"] = {};
    if (!this->bid_book_l2.empty())
    {
        (*l2_book)["bid"] = temp_bid_book_l2;
    }

    (*l2_book)["ask"] = {};
    if (!this->ask_book_l2.empty())
    {
        (*l2_book)["ask"] = temp_ask_book_l2;
    }

    return RET_OK;
}