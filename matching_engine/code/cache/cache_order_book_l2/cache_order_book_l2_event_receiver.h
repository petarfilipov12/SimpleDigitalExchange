#ifndef CACHE_ORDER_BOOK_L2_EVENT_RECEIVER_H
#define CACHE_ORDER_BOOK_L2_EVENT_RECEIVER_H

#include "types.h"

#include "event_receiver.h"
#include "cache_order_book_l2.h"
#include "event.h"

#include <iostream>

class CacheOrderBookL2_EventReceiver: public EventReceiver
{
    private:
        CacheOrderBookL2& cache_order_book_l2;

    public:
        CacheOrderBookL2_EventReceiver(const enum eReceiverId_t id, CacheOrderBookL2& cache_order_book_l2P): EventReceiver(id), cache_order_book_l2(cache_order_book_l2P) {}

        void CallEventHandler(const Event& event)
        {
            cout << "CacheOrderBookL2_EventReceiver.CallEventHandler()\n";
            this->cache_order_book_l2.EventHandler(event);
        }
};

#endif