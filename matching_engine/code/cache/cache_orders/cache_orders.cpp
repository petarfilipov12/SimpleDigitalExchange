#include "cache_orders.h"

#include "globals.h"

using namespace std;

CacheOrders::CacheOrders() {}
CacheOrders::~CacheOrders() {}

ReturnType CacheOrders::OrderAdded(Order order)
{
    ReturnType ret = RET_ORDER_EXISTS;

    this->order_lock.lock();
    if (this->orders.find(order.id) == this->orders.end())
    {
        this->orders[order.id] = order;

        ret = RET_OK;
    }
    this->order_lock.unlock();

    return ret;
}

ReturnType CacheOrders::OrderCanceled(int order_id)
{
    ReturnType ret = RET_ORDER_NOT_EXISTS;

    this->order_lock.lock();
    if (this->orders.find(order_id) != this->orders.end())
    {
        this->orders[order_id].status = false;

        ret = RET_OK;
    }
    this->order_lock.unlock();

    return ret;
}

ReturnType CacheOrders::OrderChange(int order_id, float quantity)
{
    ReturnType ret = RET_ORDER_NOT_EXISTS;
    unordered_map<int, Order>::iterator order_itter;

    this->order_lock.lock();
    order_itter = this->orders.find(order_id);
    if (order_itter != this->orders.end())
    {
        order_itter->second.filled += quantity;
        order_itter->second.status = (bool)(order_itter->second.filled < order_itter->second.quantity);

        ret = RET_OK;
    }
    this->order_lock.unlock();

    return ret;
}

ReturnType CacheOrders::OrderFilled(int taker_order_id, int book_order_id, float quantity)
{
    this->OrderChange(taker_order_id, quantity);
    this->OrderChange(book_order_id, quantity);

    return RET_OK;
}

ReturnType CacheOrders::GetOrder(int order_id, Order *pOrder)
{
    ReturnType ret = RET_ORDER_NOT_EXISTS;
    unordered_map<int, Order>::iterator order_itter;

    this->order_lock.lock();
    order_itter = this->orders.find(order_id);
    if (order_itter != this->orders.end())
    {
        *pOrder = order_itter->second;

        ret = RET_OK;
    }
    this->order_lock.unlock();

    return ret;
}

/******************************/
/*Event_Handler Implementation*/
/******************************/
static inline void CacheOrders_EventHandler_OrderAdded(Event event)
{
    cache_orders.OrderAdded(Order::ConvertJsonToOrder(event.GetJsonData()));
}

static inline void CacheOrders_EventHandler_OrderCanceled(Event event)
{
    cache_orders.OrderCanceled(event.GetJsonData()["order_id"]);
}

static inline void CacheOrders_EventHandler_OrderFilled(Event event)
{
    cache_orders.OrderFilled(
        event.GetJsonData()["taker_order"]["order_id"],
        event.GetJsonData()["book_order"]["order_id"],
        event.GetJsonData()["quantity"]
    );
}

static inline void CacheOrders_EventHandler_GetOrder(Event event)
{
    Order order;
    ReturnType ret = RET_NOT_OK;

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

void CacheOrders::EventHandler(Event event)
{
    switch(event.GetEventId())
    {
        case EVENT_ID_TAKER_ORDER_ADDED:
            CacheOrders_EventHandler_OrderAdded(event);
            break;
        case EVENT_ID_TAKER_ORDER_CANCELED:
        case EVENT_ID_MAKER_ORDER_CANCELED:
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