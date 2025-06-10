#ifndef ORDER_BOOK_H
#define ORDER_BOOK_H

#include "types.h"
#include "book.h"
#include "order.h"

#include<nlohmann/json.hpp>
using json = nlohmann::json;

class OrderBook{
    private:
        Book<greater<string> > bid_book;
        Book<less<string> > ask_book;
    public:
        OrderBook();
        
        ~OrderBook();

        bool ExistsOrder(const Order& order) const;

        bool ExistsOrderId(const int id) const;

        ReturnType AddOrder(const Order& order);

        ReturnType CancelOrder(const Order& order, Order *pOrder);

        ReturnType CancelOrderById(const int id, Order *pOrder);

        ReturnType GetBidFirst(Order **pOrder);

        ReturnType GetAskFirst(Order **pOrder);

};

#endif