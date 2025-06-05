#include "cache_candles.h"

#include <algorithm>
#include <unistd.h>
#include <ctime>

CacheCandles::CacheCandles() {}
CacheCandles::~CacheCandles() {}

Return_Type CacheCandles::OrderFilled(string price_s)
{
    float price_f = stof(price_s);

    this->current_candle_lock.lock();
    if (this->current_candle.IsEmpty())
    {
        this->current_candle.high = price_s;
        this->current_high = price_f;

        this->current_candle.open = price_s;

        this->current_candle.close = price_s;

        this->current_candle.low = price_s;
        this->current_low = price_f;
    }
    else
    {
        this->current_candle.close = price_s;

        if (this->current_high < price_f)
        {
            this->current_candle.high = price_s;
            this->current_high = price_f;
        }
        else if (this->current_low > price_f)
        {
            this->current_candle.low = price_s;
            this->current_low = price_f;
        }
    }
    this->current_candle_lock.unlock();

    return RET_OK;
}

Return_Type CacheCandles::GetCandles(int limit, json *data)const
{
    vector<Candle> temp;
    vector<Candle>::size_type candles_size;
    Candle temp_candle;

    this->candles_lock.lock();
    candles_size = this->candles.size();
    this->candles_lock.unlock();

    if(candles_size > 0)
    {
        if (limit > candles_size)
        {
            limit = candles_size;
        }

        this->candles_lock.lock();
        temp = vector<Candle>((this->candles.end() - limit), this->candles.end());
        this->candles_lock.unlock();
    }

    this->current_candle_lock.lock();
    if (this->current_candle.IsEmpty())
    {
        this->current_candle_lock.unlock();

        temp_candle = Candle(temp.back().close, this->current_timestamp);
    }
    else
    {
        temp_candle = this->current_candle;
        this->current_candle_lock.unlock();

        if (temp_candle.timestamp != this->current_timestamp)
        {
            temp_candle.timestamp = this->current_timestamp;
        }
    }

    temp.push_back(temp_candle);

    reverse(temp.begin(), temp.end());

    *data = temp;

    return RET_OK;
}

void CacheCandles::Init()
{
    int sec_till_next_interval;

    this->current_timestamp = time(nullptr);

    sec_till_next_interval = this->interval - (this->current_timestamp % this->interval);

    this->current_timestamp = this->current_timestamp + sec_till_next_interval;

    sleep(sec_till_next_interval);
    this->Cyclic();
}

void CacheCandles::Cyclic()
{
    Candle candle;
    time_t next_timestamp = this->current_timestamp + this->interval;

    this->current_candle_lock.lock();
    if (this->current_candle.IsEmpty())
    {
        this->current_candle = Candle(next_timestamp);
        this->current_candle_lock.unlock();

        this->candles_lock.lock();
        if(!this->candles.empty())
        {
            this->candles.push_back(Candle(this->candles.back().close, this->current_timestamp));
        }
        this->candles_lock.unlock();
    }
    else
    {
        candle = this->current_candle;
        this->current_candle = Candle(next_timestamp);
        this->current_candle_lock.unlock();

        if(candle.timestamp != this->current_timestamp)
        {
            candle.timestamp = this->current_timestamp;
        }
        
        this->candles_lock.lock();
        this->candles.push_back(candle);
        this->candles_lock.unlock();
    }

    this->current_timestamp = next_timestamp;
}

void CacheCandles::run()
{
    this->Init();

    while (true)
    {
        sleep(this->interval);
        this->Cyclic();
    }
}