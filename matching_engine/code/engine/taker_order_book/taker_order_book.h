#ifndef TAKER_ORDER_BOOK_H
#define TAKER_ORDER_BOOK_H

#include "return_type.h"

#include <list>

#include "base_book.h"

class TakerOrderBook : public BaseBook<std::list<Order>>{
    protected:
        void AddOrderToBook(const Order &order) override;

        void EraseOrderFromBook(const Order &order) override;

        Order &GetFirstBookOrder() override;
    
    public:
        TakerOrderBook();
        ~TakerOrderBook();

        returnType GetAt(const int index, Order **pOrder);

        returnType PopFirst();
};

#endif