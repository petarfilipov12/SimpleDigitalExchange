#ifndef EVENT_LOGGER_H
#define EVENT_LOGGER_H

#include <unordered_map>

#include "event.h"
#include "event_bus.h"

#define RECEIVER_ID_EVENT_LOGGER 0u

class EventLogger
{
    private:
        std::unordered_map<eventId_t, std::string> eventIdToStringMap = {
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

            {EVENT_ID_GET_TRADES, "EVENT_ID_GET_TRADES"},

            {EVENT_ID_ADD_SYMBOL, "EVENT_ID_ADD_SYMBOL"},

            {EVENT_ID_GET_EXCHANGE_INFO, "EVENT_ID_GET_EXCHANGE_INFO"},
        
            {EVENT_ID_INVALID, "EVENT_ID_INVALID"}
        };

    public:
        EventLogger();
        ~EventLogger();

        returnType ConvertEventIdToString(eventId_t event_id, std::string& event_id_s);

        void EventHandler(Event event);

        void init(EventBus& event_bus);
};

#endif