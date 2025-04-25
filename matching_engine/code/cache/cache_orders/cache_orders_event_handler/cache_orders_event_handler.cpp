#include "cache_orders_event_handler.h"

#include "types.h"

#include "globals.h"
#include "order.h"

void CacheOrders_EventHandler(Event event);

static void CacheOrders_EventHandler_OrderAdded(Event event)
{
    cache_orders.OrderAdded(Order::ConvertJsonToOrder(event.GetJsonData()));
}

static void CacheOrders_EventHandler_OrderCanceled(Event event)
{
    cache_orders.OrderAdded(event.GetJsonData()["order_id"]);
}

static void CacheOrders_EventHandler_OrderFilled(Event event)
{
    cache_orders.OrderFilled(
        event.GetJsonData()["bid_order_id"],
        event.GetJsonData()["ask_order_id"],
        event.GetJsonData()["quantity"]
    );
}

static void CacheOrders_EventHandler_GetOrder(Event event)
{
    Order order;
    Return_Type ret = RET_NOT_OK;

    if(nullptr != event.GetResponceDataPtr())
    {
        // (*event.GetResponceDataPtr())["error"] = RET_NOT_OK;
        // (*event.GetResponceDataPtr())["data"] = {};

        ret = cache_orders.GetOrder(event.GetJsonData()["order_id"], &order);
        (*event.GetResponceDataPtr())["error"] = ret;
        (*event.GetResponceDataPtr())["data"] = order.ConvertOrderToJson();
    }
}

void CacheOrders_EventHandler(Event event)
{
    switch(event.GetEventId())
    {
        case EVENT_ID_ORDER_ADDED:
            CacheOrders_EventHandler_OrderAdded(event);
            break;
        case EVENT_ID_ORDER_CANCELED:
            CacheOrders_EventHandler_OrderCanceled(event);
            break;
        case EVENT_ID_ORDER_FILLED:
            CacheOrders_EventHandler_OrderFilled(event);
            break;
        case EVENT_ID_GET_ORDER:
            CacheOrders_EventHandler_GetOrder(event);
            break;
        default:
            break;
    }

}
