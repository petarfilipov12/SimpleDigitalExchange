#ifndef CACHE_CANDLES_H
#define CACHE_CANDLES_H

#include "return_type.h"
#include "cache.h"

#include <vector>
#include <string>
#include <mutex>

#include "candle.h"
#include "event.h"
#include "event_bus.h"

#include "json.h"


class CacheCandles: public Cache
{
    private:
        std::vector<candle::Candle> candles;
        candle::Candle current_candle;
        float current_high;
        float current_low;

        time_t current_timestamp;

        const time_t interval = 5*60;

        mutable std::mutex candles_lock;
        mutable std::mutex current_candle_lock;

        returnType OrderFilled(const std::string& price_s);

        returnType GetCandles(int limit, json& data)const;

        void InitFunc();

        void Cyclic();

        void run();

        void EventHandler_OrderFilled(Event& event);

        void EventHandler_GetCandles(Event& event);

        void EventHandler(Event event);
    
    public:
        CacheCandles(const std::string& symbol, EventBus& event_bus, receiverId_t receiver_id);
        ~CacheCandles();
};

#endif