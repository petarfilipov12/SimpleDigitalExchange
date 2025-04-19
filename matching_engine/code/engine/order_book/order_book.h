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

        Return_Type AddOrder(Order order);

        Return_Type CancelOrder(Order order);

        Return_Type CancelOrderById(int id);

        Return_Type GetBidFirst(Order *pOrder);

        Return_Type GetAskFirst(Order *pOrder);

        Return_Type GetL2Book(json *l2_book);

        void PrintOrderBook();

};

#endif