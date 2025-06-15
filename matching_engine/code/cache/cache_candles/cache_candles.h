#ifndef CACHE_CANDLES_H
#define CACHE_CANDLES_H

#include "return_type.h"

#include <vector>
#include <string>
#include <mutex>

#include "candle.h"
#include "event.h"
#include "event_bus.h"

#include "json.h"


class CacheCandles
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

        void InitFunc();

        void Cyclic();

        void EventHandler_OrderFilled(Event& event);

        void EventHandler_GetCandles(Event& event);
    
    public:
        CacheCandles();
        ~CacheCandles();

        returnType OrderFilled(const std::string& price_s);

        returnType GetCandles(int limit, json& data)const;
        
        void run();

        void EventHandler(Event event);

        void init(EventBus& event_bus);
};

#endif