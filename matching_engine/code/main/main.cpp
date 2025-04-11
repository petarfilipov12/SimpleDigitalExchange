#include <iostream>
#include <nlohmann/json.hpp>
#include <thread>

#include "globals.h"

#include "engine.h"
#include "order.h"

#include "event.h"
#include "event_bus.h"

#include "engine_event_handlers.h"
#include "event_receiver_id.h"
#include "event_logger.h"


using json = nlohmann::json;
using namespace std;

EventBus event_bus;
Engine engine(&event_bus);

void StartEventBus()
{
    event_bus.run();
}

void StartEngine()
{
    engine.run();
}

int main(void){
    thread thread_event_bus(StartEventBus);
    thread thread_engine(StartEngine);

    event_bus.AddReceiver(RECEIVER_ID_ENGINE, EngineEventHandler_HandlerAddOrder);
    event_bus.AddReceiver(RECEIVER_ID_EVENT_LOGGER, EventLogger_EventHandler);
    EventLogger_Subscribe();

    event_bus.Subscribe(RECEIVER_ID_ENGINE, EVENT_ID_ADD_ORDER);

    while(true)
    {
        json j_data;
        string price;
        int order_id;
        int order_side;
        int order_type;


        cout << "NEW ORDER:" << endl;

        cout << "price: ";
        cin >> price;

        cout << "order_id: ";
        cin >> order_id;

        cout << "order_side: ";
        cin >> order_side;

        cout << "order_type: ";
        cin >> order_type;


        j_data["price"] = price;
        j_data["order_id"] = order_id;
        j_data["order_side"] = static_cast<enum OrderSide>(order_side);
        j_data["order_type"] = static_cast<enum OrderType>(order_type);

        Event event(EVENT_ID_ADD_ORDER, j_data);

        event_bus.Send(event);
    }
    
    thread_event_bus.join();
    thread_engine.join();

    return 0;
}
