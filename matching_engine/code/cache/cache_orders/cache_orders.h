#ifndef CACHE_ORDERS_H
#define CACHE_ORDERS_H

#include "return_type.h"
#include "cache.h"

#include <unordered_map>
#include <mutex>

#include "order.h"
#include "event.h"
#include "event_bus.h"



class CacheOrders: public Cache
{
    private:
        std::unordered_map<int, Order> orders;

        mutable std::mutex order_lock;

        returnType OrderChange(const int order_id, const float quantity);

        returnType OrderAdded(const Order& order);

        returnType OrderCanceled(const int order_id);

        returnType OrderFilled(const int taker_order_id, const int book_order_id, const float quantity);

        returnType GetOrder(const int order_id, Order& pOrder);

        void EventHandler_OrderAdded(Event& event);

        void EventHandler_OrderCanceled(Event& event);

        void EventHandler_OrderFilled(Event& event);

        void EventHandler_GetOrder(Event& event);

        void EventHandler(Event event);
    
    public:
        CacheOrders(const std::string& symbol, EventBus& event_bus, receiverId_t receiver_id);
        ~CacheOrders();
};

#endif