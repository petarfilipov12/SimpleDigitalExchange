#ifndef CACHE_ORDER_BOOK_L2_H
#define CACHE_ORDER_BOOK_L2_H

#include "return_type.h"
#include <map>
#include <string>
#include <mutex>

#include "order.h"
#include "event.h"
#include "event_bus.h"



#include "json.h"


class CacheOrderBookL2
{
    private:
        std::map<std::string, float, std::greater<std::string> > bid_book_l2;
        std::map<std::string, float, std::less<std::string> > ask_book_l2;

        mutable std::mutex bid_book_l2_look;
        mutable std::mutex ask_book_l2_look;

        void EventHandler_OrderAdded(Event& event);

        void EventHandler_OrderCanceled(Event& event);

        void EventHandler_OrderFilled(Event& event);

        void EventHandler_GetOrderBookL2(Event& event);

    public:
        returnType OrderAdded(const Order& order);

        returnType OrderCanceled(const Order& order);

        returnType OrderFilled(const std::string& price, const float quantity, const orderSide_t book_order_side);

        returnType GetOrderBookL2(json& l2_book)const;

        void EventHandler(Event event);

        void init(EventBus& event_bus);
};

#endif