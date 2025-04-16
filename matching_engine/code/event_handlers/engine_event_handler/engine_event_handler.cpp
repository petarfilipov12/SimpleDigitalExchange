#include "engine_event_handler.h"

#include "globals.h"

using namespace std;

static void Engine_EventHandler_AddOrder(Event event)
{
    string price = event.GetJsonData()["price"];
    int order_id = event.GetJsonData()["order_id"];
    int order_side = event.GetJsonData()["order_side"];
    int order_type = event.GetJsonData()["order_type"];

    engine.AddOrder(Order(price, order_id, order_side, order_type));
}

static void Engine_EventHandler_CancelOrder(Event event)
{
    engine.CancelOrderById(event.GetJsonData()["order_id"]);
}

void Engine_EventHandler(Event event)
{
    switch(event.GetEventId())
    {
        case EVENT_ID_ADD_ORDER:
            Engine_EventHandler_AddOrder(event);
            break;
        case EVENT_ID_CANCEL_ORDER:
            Engine_EventHandler_CancelOrder(event);
            break;
        default:
            break;
    }

}