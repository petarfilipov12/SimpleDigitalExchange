#ifndef EVENT_H
#define EVENT_H

#include "return_type.h"

#include <memory>

#include "json.h"


enum eEventId_t
{
    EVENT_ID_ADD_ORDER,
    EVENT_ID_CANCEL_ORDER,

    EVENT_ID_TAKER_ORDER_ADDED,
    EVENT_ID_MAKER_ORDER_ADDED,
    EVENT_ID_ADD_ORDER_FAILLED,

    EVENT_ID_TAKER_ORDER_CANCELED,
    EVENT_ID_MAKER_ORDER_CANCELED,
    EVENT_ID_CANCEL_ORDER_FAILED,

    EVENT_ID_ORDER_FILLED,

    EVENT_ID_GET_ORDER,
    EVENT_ID_GET_ORDER_BOOK,

    EVENT_ID_GET_CANDLES,

    EVENT_ID_GET_TRADES,

    EVENT_ID_ADD_SYMBOL,

    EVENT_ID_GET_EXCHANGE_INFO,

    EVENT_ID_INVALID
};

typedef enum eEventId_t eventId_t;


class Event{
    private:
        eventId_t event_id;
        json data_in;
        std::shared_ptr<json> data_out;

    public:
        Event();

        Event(const eventId_t event_id, const json& data_in, const std::shared_ptr<json>& data_out);

        Event(const Event& event2);

        eventId_t GetEventId() const;

        json GetDataIn() const;

        std::shared_ptr<json> GetDataOut() const;
};

#endif