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

        Return_Type OrderChange(int order_id, float quantity);
    
    public:
        CacheOrders();
        ~CacheOrders();

        Return_Type OrderAdded(Order order);

        Return_Type OrderCanceled(int order_id);

        Return_Type OrderFilled(int taker_order_id, int book_order_id, float quantity);

        Return_Type GetOrder(int order_id, Order *pOrder);
};

#endif