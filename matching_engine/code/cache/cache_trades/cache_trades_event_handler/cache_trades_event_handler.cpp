#include "cache_trades_event_handler.h"

#include "types.h"

#include "globals.h"

static void CacheTrades_EventHandler_OrderFilled(Event event)
{
    cache_trades.OrderFilled(event.GetJsonData()["price"], event.GetJsonData()["quantity"]);
}

static void CacheTrades_EventHandler_GetTrades(Event event)
{
    json trades;
    ReturnType ret = RET_NOT_OK;

    if(nullptr != event.GetResponceDataPtr())
    {
        ret = cache_trades.GetTrades(event.GetJsonData()["limit"], &trades);

        if(RET_OK == ret)
        {
            (*event.GetResponceDataPtr())["data"] = trades;
        }

        (*event.GetResponceDataPtr())["error"] = ret;
    }
}

void CacheTrades_EventHandler(Event event)
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