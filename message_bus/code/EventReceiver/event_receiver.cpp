#include "event_receiver.h"
EventReceiver::EventReceiver(){};

EventReceiver::EventReceiver(int id, function<void(Event_t)> callback)
{
    this->id = id;
    this->callback = callback;
}
EventReceiver::~EventReceiver()
{
    this->event_list.clear();
}

void EventReceiver::AddEvent(int event_id)
{
    this->event_list.insert(event_id);
}

void EventReceiver::RemoveEvent(int event_id)
{
    this->event_list.erase(event_id);
}

bool EventReceiver::HaveEvents() const
{
    return (!this->event_list.empty());
}

int EventReceiver::GetId() const
{
    return this->id;
}

void EventReceiver::SetCallback(function<void(Event_t)> callback)
{
    this->callback = callback;
}

void EventReceiver::Callback(Event_t event) const
{
    this->callback(event);
}

bool EventReceiver::operator<(const EventReceiver &right) const
{
    return this->id < right.id;
}