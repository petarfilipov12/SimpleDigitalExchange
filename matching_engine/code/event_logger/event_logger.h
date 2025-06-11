#ifndef EVENT_LOGGER_H
#define EVENT_LOGGER_H

#include <unordered_map>

#include "event.h"
#include "event_bus.h"

using namespace std;

class EventLogger
{
    private:
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

    public:
        EventLogger();
        ~EventLogger();

        ReturnType ConvertEventIdToString(enum eEventId_t event_id, string& event_id_s);

        static void EventHandler(Event event);

        static void init(EventBus& event_bus);
};

#endif