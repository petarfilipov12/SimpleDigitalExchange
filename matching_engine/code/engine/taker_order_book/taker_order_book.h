#ifndef TAKER_ORDER_BOOK_H
#define TAKER_ORDER_BOOK_H

#include <list>
#include <unordered_set>
#include <mutex>

#include "return_type.h"
#include "order.h"

class TakerOrderBook{
    private:
        std::list<Order> taker_orders_queue;
        std::unordered_set<Order, Order::HashFunc> taker_orders;

        mutable std::mutex taker_book_lock;
    
    public:
        TakerOrderBook();
        ~TakerOrderBook();

        bool ExistsTakerOrder(const Order& order) const;

        bool ExistsTakerOrderById(const int id) const;

        returnType AddTakerOrder(const Order& order);

        returnType CancelTakerOrder(const Order& order, Order *pOrder);

        returnType CancelTakerOrderById(const int id, Order *pOrder);

        returnType GetAt(const int index, Order **pOrder);

        returnType GetFirst(Order **pOrder);

        returnType PopFirst();
};

#endif