#include "cache_orders_event_handler.h"

#include "types.h"

#include "globals.h"
#include "order.h"

static void CacheOrders_EventHandler_OrderAdded(Event event)
{
    json j_data = event.GetJsonData();
    cache_orders.OrderAdded(ConvertJsonToOrder(j_data));
}

static void CacheOrders_EventHandler_OrderCanceled(Event event)
{
    cache_orders.OrderCanceled(event.GetJsonData()["order_id"]);
}

static void CacheOrders_EventHandler_OrderFilled(Event event)
{
    cache_orders.OrderFilled(
        event.GetJsonData()["bid_order"]["order_id"],
        event.GetJsonData()["ask_order"]["order_id"],
        event.GetJsonData()["quantity"]
    );
}

static void CacheOrders_EventHandler_GetOrder(Event event)
{
    Order order;
    Return_Type ret = RET_NOT_OK;

    if(nullptr != event.GetResponceDataPtr())
    {
        ret = cache_orders.GetOrder(event.GetJsonData()["order_id"], &order);

        if(RET_OK == ret)
        {
            (*event.GetResponceDataPtr())["data"] = order.ConvertOrderToJson();
        }

        (*event.GetResponceDataPtr())["error"] = ret;
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
