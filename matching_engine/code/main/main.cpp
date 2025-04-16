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

#include "rest_server.h"
#include "rest_server_endpoit_handlers.h"


using json = nlohmann::json;
using namespace std;

EventBus event_bus;
Engine engine(&event_bus);
RestServer rest_server("../../server_certs/cert.pem", "../../server_certs/key.pem");

void Init_EventBus()
{
    thread thread_event_bus([]{event_bus.run();});
    thread_event_bus.detach();
}

void Init_Engine()
{
    thread thread_engine([]{engine.run();});
    thread_engine.detach();

    event_bus.AddReceiver(RECEIVER_ID_ENGINE, EngineEventHandler_HandlerAddOrder);
    event_bus.Subscribe(RECEIVER_ID_ENGINE, EVENT_ID_ADD_ORDER);
}

void Init_EventLogger()
{
    event_bus.AddReceiver(RECEIVER_ID_EVENT_LOGGER, EventLogger_EventHandler);
    EventLogger_Subscribe();
}

void Init_RestServer()
{
    rest_server.SetUrlPath("/add_order", RestServerHandler_HandlerAddOrder);

    thread thread_rest_server([]{rest_server.run();});
    thread_rest_server.detach();
}

int main(void){
    Init_EventBus();
    Init_Engine();
    Init_EventLogger();
    Init_RestServer();

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

    return 0;
}
