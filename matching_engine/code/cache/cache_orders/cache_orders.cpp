#include "cache_orders.h"

CacheOrders::CacheOrders(const std::string& symbol): Cache(symbol)
{

}

CacheOrders::~CacheOrders() {}

returnType CacheOrders::OrderAdded(const Order& order)
{
    returnType ret = RET_ORDER_EXISTS;

    this->order_lock.lock();
    if (this->orders.find(order.id) == this->orders.end())
    {
        this->orders[order.id] = order;

        ret = RET_OK;
    }
    this->order_lock.unlock();

    return ret;
}

returnType CacheOrders::OrderCanceled(const int order_id)
{
    returnType ret = RET_ORDER_NOT_EXISTS;

    this->order_lock.lock();
    if (this->orders.find(order_id) != this->orders.end())
    {
        this->orders[order_id].status = false;

        ret = RET_OK;
    }
    this->order_lock.unlock();

    return ret;
}

returnType CacheOrders::OrderChange(const int order_id, const float quantity)
{
    returnType ret = RET_ORDER_NOT_EXISTS;
    std::unordered_map<int, Order>::iterator order_itter;

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

returnType CacheOrders::OrderFilled(const int taker_order_id, const int book_order_id, const float quantity)
{
    this->OrderChange(taker_order_id, quantity);
    this->OrderChange(book_order_id, quantity);

    return RET_OK;
}

returnType CacheOrders::GetOrder(const int order_id, Order& pOrder)
{
    returnType ret = RET_ORDER_NOT_EXISTS;
    std::unordered_map<int, Order>::iterator order_itter;

    this->order_lock.lock();
    order_itter = this->orders.find(order_id);
    if (order_itter != this->orders.end())
    {
        pOrder = order_itter->second;

        ret = RET_OK;
    }
    this->order_lock.unlock();

    return ret;
}
/**************************/
/*Init Func implementation*/
/**************************/
void CacheOrders::init(EventBus& event_bus, receiverId_t receiver_id)
{
    EventReceiver event_receiver = EventReceiver(
        receiver_id, 
        std::bind(&CacheOrders::EventHandler, this, std::placeholders::_1),
        std::bind(&CacheOrders::Filter, this, std::placeholders::_1)
    );

    Cache::init(event_bus, event_receiver, {
        EVENT_ID_TAKER_ORDER_ADDED,
        EVENT_ID_TAKER_ORDER_CANCELED,
        EVENT_ID_MAKER_ORDER_CANCELED,
        EVENT_ID_ORDER_FILLED,
        EVENT_ID_GET_ORDER
    });
}

/******************************/
/*Event_Handler Implementation*/
/******************************/
void CacheOrders::EventHandler_OrderAdded(Event& event)
{
    this->OrderAdded(Order::ConvertJsonToOrder(event.GetJsonData()));
}

void CacheOrders::EventHandler_OrderCanceled(Event& event)
{
    this->OrderCanceled(event.GetJsonData()["order_id"]);
}

void CacheOrders::EventHandler_OrderFilled(Event& event)
{
    this->OrderFilled(
        event.GetJsonData()["taker_order"]["order_id"],
        event.GetJsonData()["book_order"]["order_id"],
        event.GetJsonData()["quantity"]
    );
}

void CacheOrders::EventHandler_GetOrder(Event& event)
{
    Order order;
    returnType ret = RET_NOT_OK;

    if(nullptr != event.GetResponceDataPtr())
    {
        ret = this->GetOrder(event.GetJsonData()["order_id"], order);

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
            this->EventHandler_OrderAdded(event);
            break;
        case EVENT_ID_TAKER_ORDER_CANCELED:
        case EVENT_ID_MAKER_ORDER_CANCELED:
            this->EventHandler_OrderCanceled(event);
            break;
        case EVENT_ID_ORDER_FILLED:
            this->EventHandler_OrderFilled(event);
            break;
        case EVENT_ID_GET_ORDER:
            this->EventHandler_GetOrder(event);
            break;
        default:
            break;
    }

}