#ifndef CACHE_CANDLES_EVENT_RECEIVER_H
#define CACHE_CANDLES_EVENT_RECEIVER_H

#include "types.h"

#include "event_receiver.h"
#include "cache_candles.h"
#include "event.h"

#include <iostream>

class CacheCandles_EventReceiver: public EventReceiver
{
    private:
    CacheCandles& cache_candles;

    public:
    CacheCandles_EventReceiver(const enum eReceiverId_t id, CacheCandles& cache_candlesP): EventReceiver(id), cache_candles(cache_candlesP) {}

        void CallEventHandler(const Event& event)
        {
            cout << "CacheCandles_EventReceiver.CallEventHandler()\n";
            this->cache_candles.EventHandler(event);
        }
};

#endif