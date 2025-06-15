#ifndef ORDER_BOOK_H
#define ORDER_BOOK_H

#include "return_type.h"
#include "book.h"
#include "order.h"

#include "json.h"


class OrderBook{
    private:
        Book<std::greater<std::string> > bid_book;
        Book<std::less<std::string> > ask_book;
    public:
        OrderBook();
        
        ~OrderBook();

        bool ExistsOrder(const Order& order) const;

        bool ExistsOrderId(const int id) const;

        returnType AddOrder(const Order& order);

        returnType CancelOrder(const Order& order, Order *pOrder);

        returnType CancelOrderById(const int id, Order *pOrder);

        returnType GetBidFirst(Order **pOrder);

        returnType GetAskFirst(Order **pOrder);

};

#endif