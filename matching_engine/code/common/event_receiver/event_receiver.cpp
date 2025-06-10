#include "event_receiver.h"

using namespace std;

EventReceiver::EventReceiver()
{
    this->id = RECEIVER_ID_INVALID;
};

EventReceiver::EventReceiver(const enum eReceiverId_t id, const function<void(Event)> callback)
{
    this->id = id;
    this->callback = callback;
}

EventReceiver::~EventReceiver()
{
    this->events.clear();
}

void EventReceiver::AddEvent(const enum eEventId_t event_id)
{
    this->events.insert(event_id);
}

void EventReceiver::RemoveEvent(const enum eEventId_t event_id)
{
    this->events.erase(event_id);
}

bool EventReceiver::HaveEvents() const
{
    return (!this->events.empty());
}

bool EventReceiver::ContainsEvent(const enum eEventId_t event_id) const
{
    return this->events.find(event_id) != this->events.end();
}

unordered_set<enum eEventId_t> EventReceiver::GetEvents() const
{
    return this->events;
}

enum eReceiverId_t EventReceiver::GetId() const
{
    return this->id;
}

function<void(Event)> EventReceiver::GetCallback() const
{
    return this->callback;
}

bool EventReceiver::operator<(const EventReceiver &event_receiver2) const
{
    return this->id < event_receiver2.id;
}