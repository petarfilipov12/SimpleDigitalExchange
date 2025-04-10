#ifndef ENGINE_H
#define ENGINE_H

#include <iostream>

#include <list>
#include <unordered_set>
#include <algorithm>

#include "order_book.h"
#include "order.h"

using namespace std;

class Engine{
    private:
        list<Order> market_orders_queue;
        unordered_set<Order, Order::HashFunc> market_orders;
        OrderBook order_book;
    
    public:

        Engine();

        ~Engine();

        bool ExistsMarketOrder(Order order) const;

        bool ExistsOrder(Order order) const;

        bool ExistsOrderId(int id) const;

        void AddMarketOrder(Order order);

        void AddOrder(Order order);

        void CancelMarketOrder(Order order);

        void CancelOrder(Order order);

        void CancelOrderById(int id);

        OrderBook GetOrderBook() const;

        void MatchOrderBook();

        void MatchMarketOrder();

        void Cyclic();

        void run();
};

#endif