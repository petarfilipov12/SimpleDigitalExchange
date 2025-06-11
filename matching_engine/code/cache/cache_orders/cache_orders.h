#ifndef CACHE_ORDERS_H
#define CACHE_ORDERS_H

#include "types.h"
#include <unordered_map>
#include <mutex>

#include "order.h"
#include "event.h"
#include "event_bus.h"

using namespace std;

class CacheOrders
{
    private:
        unordered_map<int, Order> orders;

        mutable mutex order_lock;

        ReturnType OrderChange(const int order_id, const float quantity);
    
    public:
        CacheOrders();
        ~CacheOrders();

        ReturnType OrderAdded(const Order& order);

        ReturnType OrderCanceled(const int order_id);

        ReturnType OrderFilled(const int taker_order_id, const int book_order_id, const float quantity);

        ReturnType GetOrder(const int order_id, Order& pOrder);

        static void EventHandler(Event event);

        static void init(EventBus& event_bus);
};

#endif