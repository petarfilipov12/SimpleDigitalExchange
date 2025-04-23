#ifndef CACHE_ORDER_BOOK_L2_H
#define CACHE_ORDER_BOOK_L2_H

#include "types.h"
#include <map>
#include <string>

using namespace std;

#include<nlohmann/json.hpp>
using json = nlohmann::json;

class Cache_OrderBookL2
{
    private:
        map<string, float, greater<string> > bid_book_l2;
        map<string, float, less<string> > ask_book_l2;

    public:
        Return_Type OrderAdded(string price, float quantity)
        {

        }

        Return_Type OrderCanceled(int order_id)
        {

        }

        Return_Type OrderFilled(string price, float quantity)
        {

        }
};

#endif