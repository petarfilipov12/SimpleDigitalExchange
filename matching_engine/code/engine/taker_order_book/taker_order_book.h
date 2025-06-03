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

        Return_Type AddTakerOrder(Order order);

        Return_Type CancelTakerOrder(Order order, Order *pOrder);

        Return_Type CancelTakerOrderById(int id, Order *pOrder);

        Return_Type GetAt(int index, Order **pOrder);

        Return_Type GetFirst(Order **pOrder);

        Return_Type PopFirst();
};

#endif