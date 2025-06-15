#include "event_receiver.h"



EventReceiver::EventReceiver()
{
    this->id = RECEIVER_ID_INVALID;
};

EventReceiver::EventReceiver(const enum eReceiverId_t id, const std::function<void(Event)> callback)
{
    this->id = id;
    this->callback = callback;
}

EventReceiver::~EventReceiver()
{
    this->events.clear();
}

void EventReceiver::AddEvent(const eventId_t event_id)
{
    this->events.insert(event_id);
}

void EventReceiver::RemoveEvent(const eventId_t event_id)
{
    this->events.erase(event_id);
}

bool EventReceiver::HaveEvents() const
{
    return (!this->events.empty());
}

bool EventReceiver::ContainsEvent(const eventId_t event_id) const
{
    return this->events.find(event_id) != this->events.end();
}

std::unordered_set<eventId_t> EventReceiver::GetEvents() const
{
    return this->events;
}

enum eReceiverId_t EventReceiver::GetId() const
{
    return this->id;
}

std::function<void(Event)> EventReceiver::GetCallback() const
{
    return this->callback;
}

bool EventReceiver::operator<(const EventReceiver &event_receiver2) const
{
    return this->id < event_receiver2.id;
}