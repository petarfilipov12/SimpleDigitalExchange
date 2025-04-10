#include <iostream>
#include<nlohmann/json.hpp>

#include "engine.h"
#include "order.h"

using json = nlohmann::json;
using namespace std;


int main(void){
    Engine engine;

    cout << "HI: " << endl;

    engine.AddOrder(Order("1.1", 1, ORDER_SIDE_BUY, ORDER_TYPE_LIMIT));
    engine.AddOrder(Order("1.2", 2, ORDER_SIDE_BUY, ORDER_TYPE_LIMIT));
    engine.AddOrder(Order("1.1", 3, ORDER_SIDE_BUY, ORDER_TYPE_LIMIT));

    engine.AddOrder(Order("1.4", 4, ORDER_SIDE_SELL, ORDER_TYPE_LIMIT));
    engine.AddOrder(Order("1.3", 5, ORDER_SIDE_SELL, ORDER_TYPE_LIMIT));
    engine.AddOrder(Order("1.4", 6, ORDER_SIDE_SELL, ORDER_TYPE_LIMIT));

    engine.PrintOrderBook();

    cout << "##############" << endl;

    engine.CancelOrderById(2);
    engine.CancelOrderById(5);

    engine.PrintOrderBook();

    cout << "##############" << endl;
    
    engine.AddOrder(Order("0.0", 7, ORDER_SIDE_SELL, ORDER_TYPE_MARKET));
    engine.AddOrder(Order("0.0", 8, ORDER_SIDE_BUY, ORDER_TYPE_MARKET));
    
    engine.Cyclic();
    engine.Cyclic();

    engine.PrintOrderBook();

    return 0;
}
