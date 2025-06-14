#ifndef CACHE_ORDERS_EVENT_RECEIVER_H
#define CACHE_ORDERS_EVENT_RECEIVER_H

#include "types.h"

#include "event_receiver.h"
#include "cache_orders.h"
#include "event.h"

#include <iostream>

class CacheOrders_EventReceiver: public EventReceiver
{
    private:
        CacheOrders& cache_orders;

    public:
    CacheOrders_EventReceiver(const enum eReceiverId_t id, CacheOrders& cache_ordersP): EventReceiver(id), cache_orders(cache_ordersP) {}

        void CallEventHandler(const Event& event)
        {
            cout << "CacheOrders_EventReceiver.CallEventHandler()\n";
            this->cache_orders.EventHandler(event);
        }
};

#endif