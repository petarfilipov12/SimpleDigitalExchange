#ifndef EVENT_H
#define EVENT_H

#include "types.h"

#include<nlohmann/json.hpp>
using json = nlohmann::json;

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

    EVENT_ID_INVALID
};


class Event{
    private:
        enum eEventId_t event_id;
        json json_data;
        json *responce_data;

    public:
        Event();

        Event(enum eEventId_t event_id, json json_data, json *responce_data);

        Event(const Event& event2);

        enum eEventId_t GetEventId() const;

        json GetJsonData() const;

        json* GetResponceDataPtr() const;
};

#endif