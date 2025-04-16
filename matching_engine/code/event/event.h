#ifndef EVENT_H
#define EVENT_H

#include<nlohmann/json.hpp>
using json = nlohmann::json;


#define EVENT_ID_ADD_ORDER              0u
#define EVENT_ID_CANCEL_ORDER           1u

#define EVENT_ID_ORDER_ADDED            2u
#define EVENT_ID_ADD_ORDER_FAILLED      3u

#define EVENT_ID_ORDER_CANCELED         4u
#define EVENT_ID_CANCEL_ORDER_FAILED    5u

#define EVENT_ID_ORDER_FILLED           6u

#define EVENT_ID_INVALID                7u


class Event{
    private:
        int event_id;
        json json_data;

    public:
        Event();

        Event(int event_id, json json_data);

        Event(const Event& event2);

        int GetEventId() const;

        json GetJsonData() const;
};

#endif