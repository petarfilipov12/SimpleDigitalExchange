#include "engine_event_handler.h"

#include "globals.h"

using namespace std;
using json = nlohmann::json;

#include <iostream>

static void Engine_EventHandler_AddOrder(Event event)
{
    ReturnType ret = RET_NOT_OK;
    json j_data = event.GetJsonData();

    ret = engine.AddOrder(Order::ConvertJsonToOrder(j_data));

    if(nullptr != event.GetResponceDataPtr())
    {
        (*event.GetResponceDataPtr())["data"] = j_data;
        (*event.GetResponceDataPtr())["error"] = ret;
    }

}

static void Engine_EventHandler_CancelOrder(Event event)
{
    ReturnType ret;

    ret = engine.CancelOrderById(event.GetJsonData()["order_id"]);

    if(nullptr != event.GetResponceDataPtr())
    {
        (*event.GetResponceDataPtr())["data"] = event.GetJsonData();
        (*event.GetResponceDataPtr())["error"] = ret;
    }
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