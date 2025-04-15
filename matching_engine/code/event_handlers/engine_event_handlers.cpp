#include "engine_event_handlers.h"

#include "globals.h"

using namespace std;

void EngineEventHandler_HandlerAddOrder(Event event)
{
    if (event.GetEventId() == EVENT_ID_ADD_ORDER)
    {
        //cout << "JSONDATA: " << event.GetJsonData();
        string price = event.GetJsonData()["price"];
        int order_id = event.GetJsonData()["order_id"];
        enum OrderSide order_side = event.GetJsonData()["order_side"];
        enum OrderType order_type = event.GetJsonData()["order_type"];

        engine.AddOrder(Order(price, order_id, order_side, order_type));
    }
}