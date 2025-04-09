#include "event_bus.h"

using namespace std;

EventBus::EventBus(){}

EventBus::~EventBus(){
    this->event_receivers.clear();
    this->events_to_receivers_map.clear();

    while(!this->event_queue.empty()){
        this->event_queue.pop();
    }
}

void EventBus::Subscribe(int receiver_id, int event_id, function<void(Event_t)> handler_func)
{
    EventReceiver receiver;

    if (this->event_receivers.find(receiver_id) != this->event_receivers.end())
    {
        receiver = this->event_receivers[receiver_id];
    }
    else
    {
        receiver = EventReceiver(receiver_id, handler_func);
    }

    receiver.AddEvent(event_id);
    this->event_receivers[receiver_id] = receiver;

    if (this->events_to_receivers_map.find(event_id) == this->events_to_receivers_map.end())
    {
        this->events_to_receivers_map[event_id] = set<int>();
    }
    this->events_to_receivers_map[event_id].insert(receiver_id);
}

void EventBus::Unsubscribe(int receiver_id, int event_id)
{
    if (this->event_receivers.find(receiver_id) != this->event_receivers.end())
    {
        this->event_receivers[receiver_id].RemoveEvent(event_id);

        if (!this->event_receivers[receiver_id].HaveEvents())
        {
            this->event_receivers.erase(receiver_id);
        }
    }

    if (this->events_to_receivers_map.find(event_id) != this->events_to_receivers_map.end())
    {
        this->events_to_receivers_map[event_id].erase(receiver_id);
    }
}

void EventBus::Send(Event_t event)
{
    this->event_queue.push(event);
}

void EventBus::Cyclic(void)
{
    if (!this->event_queue.empty())
    {
        Event_t event = this->event_queue.front();

        if (this->events_to_receivers_map.find(event.event_id) != this->events_to_receivers_map.end())
        {
            for (int receiver_id : this->events_to_receivers_map[event.event_id])
            {
                this->event_receivers[receiver_id].Callback(event);
            }
        }

        this->event_queue.pop();
    }
}

void EventBus::run(void)
{
    while (true)
    {
        this->Cyclic();
    }
}
