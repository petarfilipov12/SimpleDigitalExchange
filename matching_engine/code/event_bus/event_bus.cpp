#include "event_bus.h"
#include <thread>



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

returnType EventBus::AddReceiver(const enum eReceiverId_t receiver_id, const std::function<void(Event)> handler_func)
{
    returnType ret = RET_RECEIVER_EXISTS;

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

returnType EventBus::RemoveReceiver(const enum eReceiverId_t receiver_id)
{
    returnType ret = RET_RECEIVER_NOT_EXISTS;

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

returnType EventBus::Subscribe(const enum eReceiverId_t receiver_id, const eventId_t event_id)
{
    returnType ret = RET_RECEIVER_NOT_EXISTS;

    if(event_id < EVENT_ID_INVALID)
    {
        this->receivers_lock.lock();
        if (this->event_receivers.find(receiver_id) != this->event_receivers.end())
        {
            this->event_receivers[receiver_id].AddEvent(event_id);
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

returnType EventBus::Unsubscribe(const enum eReceiverId_t receiver_id, const eventId_t event_id)
{
    returnType ret = RET_RECEIVER_NOT_EXISTS;

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

void EventBus::Send(const Event& event)
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
                std::thread t(this->event_receivers[receiver_id].GetCallback(), event);
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

/**************************/
/*Init Func implementation*/
/**************************/
void EventBus::init()
{
    std::thread thread_event_bus([this]{this->run();});
    thread_event_bus.detach();
}