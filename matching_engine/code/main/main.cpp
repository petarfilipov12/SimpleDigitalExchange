#include <iostream>
#include<nlohmann/json.hpp>

#include "order_book.h"
#include "order.h"

using json = nlohmann::json;
using namespace std;


int main(void){
    OrderBook order_book;

    cout << "HI: " << endl;

    order_book.AddOrder(Order(1.1, 1, BUY));
    order_book.AddOrder(Order(1.2, 2, BUY));
    order_book.AddOrder(Order(1.1, 3, BUY));

    order_book.AddOrder(Order(1.4, 4, SELL));
    order_book.AddOrder(Order(1.3, 5, SELL));
    order_book.AddOrder(Order(1.4, 6, SELL));

    order_book.CancelOrderById(3);
    order_book.CancelOrderById(6);

    order_book.PrintOrderBook();

    cout << "---------" << endl;
    order_book.GetBidFirst().PrintOrder();
    order_book.GetAskFirst().PrintOrder();

    return 0;
}
