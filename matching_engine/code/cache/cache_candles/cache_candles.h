#ifndef CACHE_CANDLES_H
#define CACHE_CANDLES_H

#include "types.h"

#include <vector>
#include <string>
#include <mutex>

#include "current_candle.h"

using namespace std;

#include<nlohmann/json.hpp>
using json = nlohmann::json;

using namespace candle;

class CacheCandles
{
    private:
        vector<Candle> candles;
        CurrentCandle current_candle;

        mutable mutex candles_lock;
        mutable mutex current_candle_lock;
    
    public:
        CacheCandles();
        ~CacheCandles();

        Return_Type OrderFilled(string price_s);

        Return_Type GetCandles(int limit, json* data);

        void Cyclic();
        
        void run();
};

#endif