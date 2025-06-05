#include "cache_orders.h"

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
