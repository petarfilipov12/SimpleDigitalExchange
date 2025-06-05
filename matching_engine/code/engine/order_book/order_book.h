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

        bool ExistsOrder(Order order) const;

        bool ExistsOrderId(int id) const;

        ReturnType AddOrder(Order order);

        ReturnType CancelOrder(Order order, Order *pOrder);

        ReturnType CancelOrderById(int id, Order *pOrder);

        ReturnType GetBidFirst(Order **pOrder);

        ReturnType GetAskFirst(Order **pOrder);

};

#endif