#include "engine_event_handler.h"

#include "globals.h"

using namespace std;
using json = nlohmann::json;

#include <iostream>

static void Engine_EventHandler_AddOrder(Event event)
{
    Return_Type ret;

    ret = engine.AddOrder(Order(
        event.GetJsonData()["price"],
        event.GetJsonData()["order_id"],
        static_cast<enum eOrderSide_t>(event.GetJsonData()["order_side"]),
        static_cast<enum eOrderType_t>(event.GetJsonData()["order_type"])
    ));

    if(nullptr != event.GetResponceDataPtr())
    {

        (*event.GetResponceDataPtr())["error"] = ret;
        (*event.GetResponceDataPtr())["data"] = event.GetJsonData();
    }

}

static void Engine_EventHandler_CancelOrder(Event event)
{
    Return_Type ret;

    ret = engine.CancelOrderById(event.GetJsonData()["order_id"]);

    if(nullptr != event.GetResponceDataPtr())
    {
        (*event.GetResponceDataPtr())["error"] = ret;
        (*event.GetResponceDataPtr())["data"] = event.GetJsonData();
    }
}

static void Engine_EventHandler_GetOrderBook(Event event)
{
    if(nullptr != event.GetResponceDataPtr())
    {
        (*event.GetResponceDataPtr())["error"] = RET_NOT_OK;
        (*event.GetResponceDataPtr())["data"] = {};

        (*event.GetResponceDataPtr())["error"] = engine.GetOrderBook(&((*event.GetResponceDataPtr())["data"]));
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
        case EVENT_ID_GET_ORDER_BOOK:
            Engine_EventHandler_GetOrderBook(event);
            break;
        default:
            break;
    }

}