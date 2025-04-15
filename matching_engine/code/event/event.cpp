#include "event.h"

Event::Event()
{
    this->event_id = EVENT_ID_INVALID;
}

Event::Event(enum EventIds event_id, json json_data)
{
    this->event_id = event_id;
    this->json_data = json_data;
}

Event::Event(const Event &event2)
{
    this->event_id = event2.event_id;
    this->json_data = event2.json_data;
}

enum EventIds Event::GetEventId() const
{
    return this->event_id;
}

json Event::GetJsonData() const
{
    return this->json_data;
}
