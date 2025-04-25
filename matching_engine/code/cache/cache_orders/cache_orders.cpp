#include "cache_orders.h"

using namespace std;

CacheOrders::CacheOrders() {}
CacheOrders::~CacheOrders() {}

Return_Type CacheOrders::OrderAdded(Order order)
{
    Return_Type ret = RET_ORDER_EXISTS;

    this->order_lock.lock();
    if (this->orders.find(order.id) == this->orders.end())
    {
        this->orders[order.id] = order;

        ret = RET_OK;
    }
    this->order_lock.unlock();

    return ret;
}

Return_Type CacheOrders::OrderCanceled(int order_id)
{
    Return_Type ret = RET_ORDER_NOT_EXISTS;

    this->order_lock.lock();
    if (this->orders.find(order_id) != this->orders.end())
    {
        this->orders[order_id].status = false;

        ret = RET_OK;
    }
    this->order_lock.unlock();

    return ret;
}

Return_Type CacheOrders::OrderChange(int order_id, float quantity)
{
    Return_Type ret = RET_ORDER_NOT_EXISTS;
    unordered_map<int, Order>::iterator order_itter;

    this->order_lock.lock();
    order_itter = this->orders.find(order_id);
    if (order_itter != this->orders.end())
    {
        order_itter->second.filled += quantity;

        ret = RET_OK;
    }
    this->order_lock.unlock();

    return ret;
}

Return_Type CacheOrders::OrderFilled(int bid_order_id, int ask_order_id, float quantity)
{
    this->OrderChange(bid_order_id, quantity);
    this->OrderChange(ask_order_id, quantity);

    return RET_OK;
}

Return_Type CacheOrders::GetOrder(int order_id, Order *pOrder)
{
    Return_Type ret = RET_ORDER_NOT_EXISTS;
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
