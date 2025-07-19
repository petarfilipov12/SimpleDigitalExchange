#ifndef TAKER_ORDER_BOOK_H
#define TAKER_ORDER_BOOK_H

#include <list>
#include <unordered_map>
#include <mutex>

#include "return_type.h"
#include "order.h"

class TakerOrderBook{
    private:
        std::list<Order> taker_orders_queue;
        std::unordered_map<Order, bool, Order::HashFunc> taker_orders;

        mutable std::mutex taker_orders_queue_mtx;
        mutable std::mutex taker_orders_mtx;
    
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

        returnType ReleaseTakerOrder(Order& order);
};

#endif