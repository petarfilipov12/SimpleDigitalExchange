#include "event.h"

Event::Event()
{
    this->event_id = EVENT_ID_INVALID;
    this->responce_data = nullptr;
}

Event::Event(const enum eEventId_t event_id, const json json_data, const json *responce_data)
{
    this->event_id = event_id;
    this->json_data = json_data;
    this->responce_data = (json*)responce_data;
}

Event::Event(const Event &event2)
{
    this->event_id = event2.event_id;
    this->json_data = event2.json_data;
    this->responce_data = event2.responce_data;
}

enum eEventId_t Event::GetEventId() const
{
    return this->event_id;
}

json Event::GetJsonData() const
{
    return this->json_data;
}

json* Event::GetResponceDataPtr() const
{
    return this->responce_data;
}
