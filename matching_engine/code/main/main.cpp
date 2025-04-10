#include <iostream>
#include<nlohmann/json.hpp>

#include "engine.h"
#include "order.h"

#include "event.h"
#include "event_bus.h"

using json = nlohmann::json;
using namespace std;

void TestEngine(){
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
}



void handler(Event event){
    cout << "handler1 " << event.GetJsonData() << endl;
}

void handler2(Event event){
    cout << "handler2 " << event.GetJsonData() << endl;
}

void handler3(Event event){
    cout << "handler3 " << event.GetJsonData() << endl;
}

void TestMessageBus(){
    EventBus event_bus;

    cout << "HI: " << endl;

    event_bus.Subscribe(1, EVENT_ID_ADD_ORDER, handler);
    event_bus.Subscribe(2, EVENT_ID_ADD_ORDER, handler2);
    event_bus.Subscribe(3, EVENT_ID_ORDER_FILL, handler3);

    json j_data = {{"p", 1}, {"q", 2}};
    Event event(EVENT_ID_ADD_ORDER, j_data);

    event_bus.Send(event);

    event_bus.Cyclic();


    cout << "BYE: " << endl;
}


int main(void){
    TestMessageBus();

    return 0;
}
