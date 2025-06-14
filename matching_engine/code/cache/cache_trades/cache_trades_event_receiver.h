#ifndef CACHE_TRADES_EVENT_RECEIVER_H
#define CACHE_TRADES_EVENT_RECEIVER_H

#include "types.h"

#include "event_receiver.h"
#include "cache_trades.h"
#include "event.h"

#include <iostream>

class CacheTrades_EventReceiver: public EventReceiver
{
    private:
        CacheTrades& cache_trades;

    public:
        CacheTrades_EventReceiver(const enum eReceiverId_t id, CacheTrades& cache_tradesP): EventReceiver(id), cache_trades(cache_tradesP) {}

        void CallEventHandler(const Event& event)
        {
            cout << "CacheOrders_EventReceiver.CallEventHandler()\n";
            this->cache_trades.EventHandler(event);
        }
};

#endif