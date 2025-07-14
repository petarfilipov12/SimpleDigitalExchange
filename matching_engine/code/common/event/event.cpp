#include "event.h"

Event::Event()
{
    this->event_id = EVENT_ID_INVALID;
    this->data_in = nullptr;
    this->data_out = nullptr;
}

Event::Event(const eventId_t event_id, const json& data_in, const std::shared_ptr<json>& data_out)
{
    this->event_id = event_id;
    this->data_in = data_in;
    this->data_out = data_out;
}

Event::Event(const Event &event2)
{
    this->event_id = event2.event_id;
    this->data_in = event2.data_in;
    this->data_out = event2.data_out;
}

eventId_t Event::GetEventId() const
{
    return this->event_id;
}

json Event::GetDataIn() const
{
    return this->data_in;
}

std::shared_ptr<json> Event::GetDataOut() const
{
    return this->data_out;
}
