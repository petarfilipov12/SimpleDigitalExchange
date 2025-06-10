#include "cache_trades.h"

#include <ctime>

#include "globals.h"

CacheTrades::CacheTrades() {}
CacheTrades::~CacheTrades() {}

ReturnType CacheTrades::OrderFilled(const string& price, const float quantity)
{
    this->trades_lock.lock();
    this->trades.push_back((struct sTrade){price, quantity, time(nullptr)});
    this->trades_lock.unlock();
    
    return RET_OK;
}

ReturnType CacheTrades::GetTrades(int limit, json& data)const
{
    vector<sTrade> temp;
    vector<sTrade>::size_type trades_size;

    this->trades_lock.lock();
    trades_size = this->trades.size();
    this->trades_lock.unlock();

    if(trades_size > 0)
    {
        if (limit > trades_size)
        {
            limit = trades_size;
        }

        this->trades_lock.lock();
        temp = vector<sTrade>((this->trades.end() - limit), this->trades.end());
        this->trades_lock.unlock();

        reverse(temp.begin(), temp.end());

        data = temp;
    }

    return RET_OK;
}

/******************************/
/*Event_Handler Implementation*/
/******************************/
static inline void CacheTrades_EventHandler_OrderFilled(Event event)
{
    cache_trades.OrderFilled(event.GetJsonData()["price"], event.GetJsonData()["quantity"]);
}

static inline void CacheTrades_EventHandler_GetTrades(Event event)
{
    json trades;
    ReturnType ret = RET_NOT_OK;

    if(nullptr != event.GetResponceDataPtr())
    {
        ret = cache_trades.GetTrades(event.GetJsonData()["limit"], trades);

        if(RET_OK == ret)
        {
            (*event.GetResponceDataPtr())["data"] = trades;
        }

        (*event.GetResponceDataPtr())["error"] = ret;
    }
}

void CacheTrades::EventHandler(Event event)
{
    switch(event.GetEventId())
    {
        case EVENT_ID_ORDER_FILLED:
            CacheTrades_EventHandler_OrderFilled(event);
            break;
        case EVENT_ID_GET_TRADES:
            CacheTrades_EventHandler_GetTrades(event);
            break;
        default:
            break;
    }
}