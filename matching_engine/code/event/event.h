#ifndef EVENT_H
#define EVENT_H

#include<nlohmann/json.hpp>
using json = nlohmann::json;

enum EventIds{
    EVENT_ID_ADD_ORDER,
    EVENT_ID_ORDER_FILL,
    EVENT_ID_INVALID
};

class Event{
    private:
        enum EventIds event_id;
        json json_data;

    public:
        Event();

        Event(enum EventIds event_id, json json_data);

        Event(const Event& event2);

        enum EventIds GetEventId() const;

        json GetJsonData() const;
};

#endif