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

        bool ExistsTakerOrder(Order order) const;

        bool ExistsTakerOrderById(int id) const;

        ReturnType AddTakerOrder(Order order);

        ReturnType CancelTakerOrder(Order order, Order *pOrder);

        ReturnType CancelTakerOrderById(int id, Order *pOrder);

        ReturnType GetAt(int index, Order **pOrder);

        ReturnType GetFirst(Order **pOrder);

        ReturnType PopFirst();
};

#endif