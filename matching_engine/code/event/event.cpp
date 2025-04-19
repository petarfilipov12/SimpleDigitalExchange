#include "event.h"

Event::Event()
{
    this->event_id = EVENT_ID_INVALID;
    this->responce = nullptr;
}

Event::Event(enum eEventId_t event_id, json json_data, Return_Type *responce)
{
    this->event_id = event_id;
    this->json_data = json_data;
    this->responce = responce;
}

Event::Event(const Event &event2)
{
    this->event_id = event2.event_id;
    this->json_data = event2.json_data;
    this->responce = event2.responce;
}

enum eEventId_t Event::GetEventId() const
{
    return this->event_id;
}

json Event::GetJsonData() const
{
    return this->json_data;
}

Return_Type* Event::GetResponcePtr() const
{
    return this->responce;
}
