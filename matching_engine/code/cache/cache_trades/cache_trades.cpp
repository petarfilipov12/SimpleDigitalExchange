#include "cache_trades.h"

#include <ctime>

CacheTrades::CacheTrades() {}
CacheTrades::~CacheTrades() {}

returnType CacheTrades::OrderFilled(const std::string& price, const float quantity)
{
    this->trades_lock.lock();
    this->trades.push_back((struct trade::sTrade){price, quantity, time(nullptr)});
    this->trades_lock.unlock();
    
    return RET_OK;
}

returnType CacheTrades::GetTrades(int limit, json& data)const
{
    std::vector<trade::sTrade> temp;
    std::vector<trade::sTrade>::size_type trades_size;

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
        temp = std::vector<trade::sTrade>((this->trades.end() - limit), this->trades.end());
        this->trades_lock.unlock();

        reverse(temp.begin(), temp.end());

        data = temp;
    }

    return RET_OK;
}
/**************************/
/*Init Func implementation*/
/**************************/
void CacheTrades::init(EventBus& event_bus)
{
    event_bus.AddReceiver(RECEIVER_ID_CACHE_TRADES, std::bind(&CacheTrades::EventHandler, this, std::placeholders::_1));
    
    event_bus.Subscribe(RECEIVER_ID_CACHE_TRADES, EVENT_ID_ORDER_FILLED);
    event_bus.Subscribe(RECEIVER_ID_CACHE_TRADES, EVENT_ID_GET_TRADES);
}

/******************************/
/*Event_Handler Implementation*/
/******************************/
void CacheTrades::EventHandler_OrderFilled(Event& event)
{
    this->OrderFilled(event.GetJsonData()["price"], event.GetJsonData()["quantity"]);
}

void CacheTrades::EventHandler_GetTrades(Event& event)
{
    json trades;
    returnType ret = RET_NOT_OK;

    if(nullptr != event.GetResponceDataPtr())
    {
        ret = this->GetTrades(event.GetJsonData()["limit"], trades);

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
            this->EventHandler_OrderFilled(event);
            break;
        case EVENT_ID_GET_TRADES:
            this->EventHandler_GetTrades(event);
            break;
        default:
            break;
    }
}