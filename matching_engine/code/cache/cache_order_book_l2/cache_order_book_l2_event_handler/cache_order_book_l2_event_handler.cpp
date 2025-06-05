#include "cache_order_book_l2_event_handler.h"

#include "types.h"

#include "globals.h"
#include "order.h"

static void CacheOrderBookL2_EventHandler_OrderAdded(Event event)
{
    json j_data = event.GetJsonData();
    cache_order_book_l2.OrderAdded(ConvertJsonToOrder(j_data));
}

static void CacheOrderBookL2_EventHandler_OrderCanceled(Event event)
{
    json j_data = event.GetJsonData();
    cache_order_book_l2.OrderCanceled(ConvertJsonToOrder(j_data));
}

static void CacheOrderBookL2_EventHandler_OrderFilled(Event event)
{
    cache_order_book_l2.OrderFilled(
        event.GetJsonData()["price"], event.GetJsonData()["quantity"],
        event.GetJsonData()["book_order"]["order_side"]
    );
}

static void CacheOrderBookL2_EventHandler_GetOrderBookL2(Event event)
{
    json l2_book;
    Return_Type ret = RET_NOT_OK;

    if(nullptr != event.GetResponceDataPtr())
    {
        ret = cache_order_book_l2.GetOrderBookL2(&l2_book);

        if(RET_OK == ret)
        {
            (*event.GetResponceDataPtr())["data"] = l2_book;
        }

        (*event.GetResponceDataPtr())["error"] = ret;
    }
}

void CacheOrderBookL2_EventHandler(Event event)
{
    switch(event.GetEventId())
    {
        case EVENT_ID_MAKER_ORDER_ADDED:
            CacheOrderBookL2_EventHandler_OrderAdded(event);
            break;
        case EVENT_ID_MAKER_ORDER_CANCELED:
            CacheOrderBookL2_EventHandler_OrderCanceled(event);
            break;
        case EVENT_ID_ORDER_FILLED:
            CacheOrderBookL2_EventHandler_OrderFilled(event);
            break;
        case EVENT_ID_GET_ORDER_BOOK:
            CacheOrderBookL2_EventHandler_GetOrderBookL2(event);
            break;
        default:
            break;
    }
}