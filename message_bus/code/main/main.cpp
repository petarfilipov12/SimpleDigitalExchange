#include <iostream>
#include <set>
#include <functional>
#include<nlohmann/json.hpp>

#include "event.h"
#include "event_bus.h"

using json = nlohmann::json;
using namespace std;

void handler(Event_t event){
    cout << "handler1 " << event.json_data << endl;
}

void handler2(Event_t event){
    cout << "handler2 " << event.json_data << endl;
}

void handler3(Event_t event){
    cout << "handler3 " << event.json_data << endl;
}


int main(void){
    EventBus event_bus;

    cout << "HI: " << endl;

    event_bus.Subscribe(1, 1, handler);
    event_bus.Subscribe(2, 1, handler2);
    event_bus.Subscribe(3, 2, handler3);

    Event_t event;
    json j_data = {{"p", 1}, {"q", 2}};
    event.event_id = 3;
    event.json_data = j_data;


    event_bus.Send(event);

    event_bus.Cyclic();


    cout << "BYE: " << endl;

    

    return 0;
}
