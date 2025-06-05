#ifndef CACHE_ORDER_BOOK_L2_H
#define CACHE_ORDER_BOOK_L2_H

#include "types.h"
#include <map>
#include <string>
#include <mutex>
#include "order.h"

using namespace std;

#include<nlohmann/json.hpp>
using json = nlohmann::json;

class Cache_OrderBookL2
{
    private:
        map<string, float, greater<string> > bid_book_l2;
        map<string, float, less<string> > ask_book_l2;

        mutable mutex bid_book_l2_look;
        mutable mutex ask_book_l2_look;

    public:
        Return_Type OrderAdded(Order order);

        Return_Type OrderCanceled(Order order);

        Return_Type OrderFilled(string price, float quantity, enum eOrderSide_t book_order_side);

        Return_Type GetOrderBookL2(json *l2_book);
};

#endif