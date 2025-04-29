#include "cache_order_book_l2_event_handler.h"

#include "types.h"

#include "globals.h"
#include "order.h"

static void CacheOrderBookL2_EventHandler_OrderAdded(Event event)
{
    cache_order_book_l2.OrderAdded(Order::ConvertJsonToOrder(event.GetJsonData()));
}

static void CacheOrderBookL2_EventHandler_OrderCanceled(Event event)
{
    cache_order_book_l2.OrderCanceled(event.GetJsonData()["order_id"]);
}

static void CacheOrderBookL2_EventHandler_OrderFilled(Event event)
{
    cache_order_book_l2.OrderFilled(
        event.GetJsonData()["bid_order_id"],
        event.GetJsonData()["ask_order_id"],
        event.GetJsonData()["quantity"]
    );
}

static void CacheOrderBookL2_EventHandler_GetOrderBookL2(Event event)
{
    cache_order_book_l2.GetOrderBookL2(event.GetResponceDataPtr());
}

void CacheOrderBookL2_EventHandler(Event event)
{
    switch(event.GetEventId())
    {
        case EVENT_ID_ORDER_ADDED:
            CacheOrderBookL2_EventHandler_OrderAdded(event);
            break;
        case EVENT_ID_ORDER_CANCELED:
            CacheOrderBookL2_EventHandler_OrderCanceled(event);
            break;
        case EVENT_ID_ORDER_FILLED:
            CacheOrderBookL2_EventHandler_OrderFilled(event);
            break;
        case EVENT_ID_GET_ORDER:
        CacheOrderBookL2_EventHandler_GetOrderBookL2(event);
            break;
        default:
            break;
    }
}