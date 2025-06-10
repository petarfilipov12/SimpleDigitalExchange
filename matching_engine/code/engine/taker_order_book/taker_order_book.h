#ifndef TAKER_ORDER_BOOK_H
#define TAKER_ORDER_BOOK_H

#include <list>
#include <unordered_set>
#include <mutex>

#include "types.h"
#include "order.h"

class TakerOrderBook{
    private:
        list<Order> taker_orders_queue;
        unordered_set<Order, Order::HashFunc> taker_orders;

        mutable mutex taker_book_lock;
    
    public:
        TakerOrderBook();
        ~TakerOrderBook();

        bool ExistsTakerOrder(const Order& order) const;

        bool ExistsTakerOrderById(const int id) const;

        ReturnType AddTakerOrder(const Order& order);

        ReturnType CancelTakerOrder(const Order& order, Order *pOrder);

        ReturnType CancelTakerOrderById(const int id, Order *pOrder);

        ReturnType GetAt(const int index, Order **pOrder);

        ReturnType GetFirst(Order **pOrder);

        ReturnType PopFirst();
};

#endif