#include "event_receiver.h"

using namespace std;

EventReceiver::EventReceiver()
{
    this->id = 0;
};

EventReceiver::EventReceiver(int id, function<void(Event)> callback)
{
    this->id = id;
    this->callback = callback;
}

EventReceiver::~EventReceiver()
{
    this->events.clear();
}

void EventReceiver::AddEvent(int event_id)
{
    this->events.insert(event_id);
}

void EventReceiver::RemoveEvent(int event_id)
{
    this->events.erase(event_id);
}

bool EventReceiver::HaveEvents() const
{
    return (!this->events.empty());
}

bool EventReceiver::ContainsEvent(int event_id)
{
    return this->events.find(event_id) != this->events.end();
}

unordered_set<int> EventReceiver::GetEvents() const
{
    return this->events;
}

int EventReceiver::GetId() const
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