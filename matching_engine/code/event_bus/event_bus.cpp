#include "event_bus.h"
#include <thread>

using namespace std;

EventBus::EventBus() {}

EventBus::~EventBus()
{
    // this->event_receivers.clear();
    // this->events_to_receivers_map.clear();

    // while (!this->event_queue.empty())
    // {
    //     this->event_queue.pop();
    // }
}

Return_Type EventBus::AddReceiver(int receiver_id, function<void(Event)> handler_func)
{
    Return_Type ret = RET_RECEIVER_EXISTS;

    this->receivers_lock.lock();
    if (this->event_receivers.find(receiver_id) == this->event_receivers.end())
    {
        EventReceiver receiver = EventReceiver(receiver_id, handler_func);
        this->event_receivers[receiver_id] = receiver;

        ret = RET_OK;
    }
    this->receivers_lock.unlock();

    return ret;
}

Return_Type EventBus::RemoveReceiver(int receiver_id)
{
    Return_Type ret = RET_RECEIVER_NOT_EXISTS;

    this->receivers_lock.lock();
    if (this->event_receivers.find(receiver_id) != this->event_receivers.end())
    {
        for (auto event_id : this->event_receivers[receiver_id].GetEvents())
        {
            if (this->events_to_receivers_map[event_id].erase(receiver_id))
            {
                if (this->events_to_receivers_map[event_id].empty())
                {
                    this->events_to_receivers_map.erase(event_id);
                }
            }
        }

        this->event_receivers.erase(receiver_id);

        ret = RET_OK;
    }
    this->receivers_lock.unlock();

    return ret;
}

Return_Type EventBus::Subscribe(int receiver_id, int event_id)
{
    Return_Type ret = RET_RECEIVER_NOT_EXISTS;

    if(event_id < EVENT_ID_INVALID)
    {
        this->receivers_lock.lock();
        if (this->event_receivers.find(receiver_id) != this->event_receivers.end())
        {
            EventReceiver receiver = this->event_receivers[receiver_id];

            receiver.AddEvent(event_id);
            this->events_to_receivers_map[event_id].insert(receiver_id);

            ret = RET_OK;
        }
        this->receivers_lock.unlock();
    }
    else
    {
        ret = RET_EVENT_ID_INVALID;
    }

    return ret;
}

Return_Type EventBus::Unsubscribe(int receiver_id, int event_id)
{
    Return_Type ret = RET_RECEIVER_NOT_EXISTS;

    if(event_id < EVENT_ID_INVALID)
    {
        this->receivers_lock.lock();
        if (this->event_receivers.find(receiver_id) != this->event_receivers.end())
        {
            this->event_receivers[receiver_id].RemoveEvent(event_id);

            if (this->events_to_receivers_map[event_id].erase(receiver_id))
            {
                if (this->events_to_receivers_map[event_id].empty())
                {
                    this->events_to_receivers_map.erase(event_id);
                }
            }

            ret = RET_OK;
        }
        this->receivers_lock.unlock();
    }
    else
    {
        ret = RET_EVENT_ID_INVALID;
    }

    return ret;
}

void EventBus::Send(Event event)
{
    this->event_queue_lock.lock();
    this->event_queue.push(event);
    this->event_queue_lock.unlock();
}

void EventBus::Cyclic(void)
{
    Event event;

    this->event_queue_lock.lock();
    if (!this->event_queue.empty())
    {
        event = this->event_queue.front();
        this->event_queue.pop();
    }
    this->event_queue_lock.unlock();

    if (EVENT_ID_INVALID != event.GetEventId())
    {
        this->receivers_lock.lock();
        if (this->events_to_receivers_map.find(event.GetEventId()) != this->events_to_receivers_map.end())
        {
            for (auto receiver_id : this->events_to_receivers_map[event.GetEventId()])
            {
                thread t(this->event_receivers[receiver_id].GetCallback(), event);
                t.detach();
            }
        }
        this->receivers_lock.unlock();
    }
}

void EventBus::run(void)
{
    while (true)
    {
        this->Cyclic();
    }
}