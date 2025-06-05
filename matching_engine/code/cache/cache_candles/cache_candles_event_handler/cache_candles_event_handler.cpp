#include "cache_candles_event_handler.h"

#include "types.h"

#include "globals.h"

static void CacheCandles_EventHandler_OrderFilled(Event event)
{
    cache_candles.OrderFilled(event.GetJsonData()["price"]);
}

static void CacheCandles_EventHandler_GetCandles(Event event)
{
    json candles;
    ReturnType ret = RET_NOT_OK;

    if(nullptr != event.GetResponceDataPtr())
    {
        ret = cache_candles.GetCandles(event.GetJsonData()["limit"], &candles);

        if(RET_OK == ret)
        {
            (*event.GetResponceDataPtr())["data"] = candles;
        }

        (*event.GetResponceDataPtr())["error"] = ret;
    }
}

void CacheCandles_EventHandler(Event event)
{
    switch(event.GetEventId())
    {
        case EVENT_ID_ORDER_FILLED:
            CacheCandles_EventHandler_OrderFilled(event);
            break;
        case EVENT_ID_GET_CANDLES:
            CacheCandles_EventHandler_GetCandles(event);
            break;
        default:
            break;
    }
}