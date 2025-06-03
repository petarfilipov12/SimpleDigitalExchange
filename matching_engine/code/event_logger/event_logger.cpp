#include "event_logger.h"

#include <iostream>
#include <unordered_map>

#include "globals.h"
#include "event_receiver_id.h"

using namespace std;

unordered_map<enum eEventId_t, string> eventIdToStringMap = {
    {EVENT_ID_ADD_ORDER, "EVENT_ID_ADD_ORDER"},
    {EVENT_ID_CANCEL_ORDER, "EVENT_ID_CANCEL_ORDER"},

    {EVENT_ID_TAKER_ORDER_ADDED, "EVENT_ID_TAKER_ORDER_ADDED"},
    {EVENT_ID_MAKER_ORDER_ADDED, "EVENT_ID_MAKER_ORDER_ADDED"},
    {EVENT_ID_ADD_ORDER_FAILLED, "EVENT_ID_ADD_ORDER_FAILLED"},

    {EVENT_ID_TAKER_ORDER_CANCELED, "EVENT_ID_TAKER_ORDER_CANCELED"},
    {EVENT_ID_MAKER_ORDER_CANCELED, "EVENT_ID_MAKER_ORDER_CANCELED"},
    {EVENT_ID_CANCEL_ORDER_FAILED, "EVENT_ID_CANCEL_ORDER_FAILED"},

    {EVENT_ID_ORDER_FILLED, "EVENT_ID_ORDER_FILLED"},

    {EVENT_ID_GET_ORDER, "EVENT_ID_GET_ORDER"},
    {EVENT_ID_GET_ORDER_BOOK, "EVENT_ID_GET_ORDER_BOOK"},

    {EVENT_ID_GET_CANDLES, "EVENT_ID_GET_CANDLES"},

    {EVENT_ID_INVALID, "EVENT_ID_INVALID"}
};

void EventLogger_EventHandler(Event event)
{
    cout << "event_logger: event_id=" << eventIdToStringMap[event.GetEventId()] << ", data=" << event.GetJsonData() << endl;
}

void EventLogger_Subscribe()
{
    int event_id;

    for(event_id = 0; event_id < EVENT_ID_INVALID; event_id++)
    {
        event_bus.Subscribe(RECEIVER_ID_EVENT_LOGGER, static_cast<enum eEventId_t>(event_id));
    }
}