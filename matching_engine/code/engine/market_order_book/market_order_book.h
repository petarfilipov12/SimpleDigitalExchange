#ifndef MARKET_ORDER_BOOK_H
#define MARKET_ORDER_BOOK_H

#include <list>
#include <unordered_set>
#include <mutex>

#include "types.h"
#include "order.h"

class MarketOrderBook{
    private:
        list<Order> market_orders_queue;
        unordered_set<Order, Order::HashFunc> market_orders;

        mutable mutex market_book_lock;
    
    public:
        MarketOrderBook();
        ~MarketOrderBook();

        bool ExistsMarketOrder(Order order) const;

        bool ExistsMarketOrderById(int id) const;

        Return_Type AddMarketOrder(Order order);

        Return_Type CancelMarketOrder(Order order);

        Return_Type CancelMarketOrderById(int id);

        Return_Type GetFirst(Order *pOrder) const;

        Return_Type PopFirst();
};

#endif