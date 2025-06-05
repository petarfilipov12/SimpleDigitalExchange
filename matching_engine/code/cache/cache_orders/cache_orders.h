#ifndef CACHE_ORDERS_H
#define CACHE_ORDERS_H

#include "types.h"
#include <unordered_map>
#include <mutex>
#include "order.h"

using namespace std;

class CacheOrders
{
    private:
        unordered_map<int, Order> orders;

        mutable mutex order_lock;

        ReturnType OrderChange(int order_id, float quantity);
    
    public:
        CacheOrders();
        ~CacheOrders();

        ReturnType OrderAdded(Order order);

        ReturnType OrderCanceled(int order_id);

        ReturnType OrderFilled(int taker_order_id, int book_order_id, float quantity);

        ReturnType GetOrder(int order_id, Order *pOrder);
};

#endif