#include "cache_candles.h"

#include <algorithm>
#include <unistd.h>
#include <ctime>
#include <thread>

CacheCandles::CacheCandles(const std::string& symbol): Cache(symbol)
{

}

CacheCandles::~CacheCandles() {}

returnType CacheCandles::OrderFilled(const std::string& price_s)
{
    const float price_f = stof(price_s);

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

returnType CacheCandles::GetCandles(int limit, json& data)const
{
    std::vector<candle::Candle> temp;
    std::vector<candle::Candle>::size_type candles_size;
    candle::Candle temp_candle;

    this->candles_lock.lock();
    candles_size = this->candles.size();
    this->candles_lock.unlock();

    this->current_candle_lock.lock();
    temp_candle = this->current_candle;
    this->current_candle_lock.unlock();

    if ((candles_size > 0) || (!temp_candle.IsEmpty()))
    {
        if (candles_size > 0)
        {
            if (limit > candles_size)
            {
                limit = candles_size;
            }

            this->candles_lock.lock();
            temp = std::vector<candle::Candle>((this->candles.end() - limit), this->candles.end());
            this->candles_lock.unlock();
        }
        
        if (temp_candle.IsEmpty())
        {
            temp_candle = candle::Candle(temp.back().close, this->current_timestamp);
        }
        else
        {
            if (temp_candle.timestamp != this->current_timestamp)
            {
                temp_candle.timestamp = this->current_timestamp;
            }
        }

        temp.push_back(temp_candle);

        if (temp.size() > 1)
        {
            reverse(temp.begin(), temp.end());
        }

        data = temp;
    }

    return RET_OK;
}

void CacheCandles::InitFunc()
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
    candle::Candle candle;
    time_t next_timestamp = this->current_timestamp + this->interval;

    this->current_candle_lock.lock();
    if (this->current_candle.IsEmpty())
    {
        this->current_candle = candle::Candle(next_timestamp);
        this->current_candle_lock.unlock();

        this->candles_lock.lock();
        if(!this->candles.empty())
        {
            this->candles.push_back(candle::Candle(this->candles.back().close, this->current_timestamp));
        }
        this->candles_lock.unlock();
    }
    else
    {
        candle = this->current_candle;
        this->current_candle = candle::Candle(next_timestamp);
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
    this->InitFunc();

    while (true)
    {
        sleep(this->interval);
        this->Cyclic();
    }
}
/**************************/
/*Init Func implementation*/
/**************************/
void CacheCandles::init(EventBus& event_bus, receiverId_t receiver_id)
{
    std::thread thread_cache_candles([this]{this->run();});
    thread_cache_candles.detach();

    EventReceiver event_receiver = EventReceiver(
        receiver_id, 
        std::bind(&CacheCandles::EventHandler, this, std::placeholders::_1),
        std::bind(&CacheCandles::Filter, this, std::placeholders::_1)
    );
    
    Cache::init(event_bus, event_receiver, {EVENT_ID_ORDER_FILLED, EVENT_ID_GET_CANDLES});
}

/******************************/
/*Event_Handler Implementation*/
/******************************/
void CacheCandles::EventHandler_OrderFilled(Event& event)
{
    this->OrderFilled(event.GetJsonData()["price"]);
}

void CacheCandles::EventHandler_GetCandles(Event& event)
{
    json candles;
    returnType ret = RET_NOT_OK;

    if(nullptr != event.GetResponceDataPtr())
    {
        ret = this->GetCandles(event.GetJsonData()["limit"], candles);

        if(RET_OK == ret)
        {
            (*event.GetResponceDataPtr())["data"] = candles;
        }

        (*event.GetResponceDataPtr())["error"] = ret;
    }
}

void CacheCandles::EventHandler(Event event)
{
    switch(event.GetEventId())
    {
        case EVENT_ID_ORDER_FILLED:
            this->EventHandler_OrderFilled(event);
            break;
        case EVENT_ID_GET_CANDLES:
            this->EventHandler_GetCandles(event);
            break;
        default:
            break;
    }
}