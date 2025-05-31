#include "cache_candles.h"

#include <algorithm>
#include <unistd.h>

CacheCandles::CacheCandles() {}
CacheCandles::~CacheCandles() {}

Return_Type CacheCandles::OrderFilled(string price_s)
{
    float price_f = stof(price_s);

    this->current_candle_lock.lock();
    if (this->current_candle.IsEmpty())
    {
        this->current_candle.high_s = price_s;
        this->current_candle.high_f = price_f;

        this->current_candle.open_s = price_s;

        this->current_candle.close_s = price_s;

        this->current_candle.low_s = price_s;
        this->current_candle.low_f = price_f;
    }
    else
    {
        this->current_candle.close_s = price_s;

        if (this->current_candle.high_f < price_f)
        {
            this->current_candle.high_s = price_s;
            this->current_candle.high_f = price_f;
        }
        else if (this->current_candle.low_f > price_f)
        {
            this->current_candle.low_s = price_s;
            this->current_candle.low_f = price_f;
        }
    }
    this->current_candle_lock.unlock();

    return RET_OK;
}

Return_Type CacheCandles::GetCandles(int limit, json *data)
{
    vector<Candle> temp;
    vector<Candle>::size_type candles_size;

    this->candles_lock.lock();
    candles_size = this->candles.size();
    this->candles_lock.unlock();

    if (limit > candles_size)
    {
        limit = candles_size;
    }

    this->candles_lock.lock();
    temp = vector<Candle>((this->candles.end() - limit), this->candles.end());
    this->candles_lock.unlock();

    reverse(temp.begin(), temp.end());

    *data = temp;

    return RET_OK;
}

void CacheCandles::Cyclic()
{
    this->current_candle_lock.lock();
    if (this->current_candle.IsEmpty())
    {
        this->current_candle_lock.unlock();

        this->candles_lock.lock();
        if(!this->candles.empty())
        {
            string last_close = this->candles.back().close;
            this->candles.push_back(Candle(last_close));
        }
        this->candles_lock.unlock();
    }
    else
    {
        Candle candle = this->current_candle.GetCandle();
        this->current_candle_lock.unlock();
        
        this->candles_lock.lock();
        this->candles.push_back(candle);
        this->candles_lock.unlock();
    }

    this->current_candle.InitCurrentCandle();
}

void CacheCandles::run()
{
    while (true)
    {
        sleep(1);
        this->Cyclic();
    }
}