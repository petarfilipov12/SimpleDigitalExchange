#ifndef CACHE_ORDER_BOOK_L2_H
#define CACHE_ORDER_BOOK_L2_H

#include "types.h"
#include <map>
#include <string>
#include <mutex>

#include "order.h"
#include "event.h"
#include "event_bus.h"

using namespace std;

#include<nlohmann/json.hpp>
using json = nlohmann::json;

class CacheOrderBookL2
{
    private:
        map<string, float, greater<string> > bid_book_l2;
        map<string, float, less<string> > ask_book_l2;

        mutable mutex bid_book_l2_look;
        mutable mutex ask_book_l2_look;

        void EventHandler_OrderAdded(Event& event);

        void EventHandler_OrderCanceled(Event& event);

        void EventHandler_OrderFilled(Event& event);

        void EventHandler_GetOrderBookL2(Event& event);

    public:
        ReturnType OrderAdded(const Order& order);

        ReturnType OrderCanceled(const Order& order);

        ReturnType OrderFilled(const string& price, const float quantity, const enum eOrderSide_t book_order_side);

        ReturnType GetOrderBookL2(json& l2_book)const;

        void EventHandler(Event event);

        void init(EventBus& event_bus);
};

#endif