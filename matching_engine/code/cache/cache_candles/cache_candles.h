#ifndef CACHE_CANDLES_H
#define CACHE_CANDLES_H

#include "types.h"

#include <vector>
#include <string>
#include <mutex>

#include "candle.h"

using namespace std;

#include<nlohmann/json.hpp>
using json = nlohmann::json;

using namespace candle;

class CacheCandles
{
    private:
        vector<Candle> candles;
        Candle current_candle;
        float current_high;
        float current_low;
        time_t current_timestamp;

        const time_t interval = 5*60;

        mutable mutex candles_lock;
        mutable mutex current_candle_lock;
    
    public:
        CacheCandles();
        ~CacheCandles();

        Return_Type OrderFilled(string price_s);

        Return_Type GetCandles(int limit, json* data);

        void Init();

        void Cyclic();
        
        void run();
};

#endif