#include "event_bus.h"
#include <thread>



EventBus::EventBus()
{
    std::thread thread_event_bus([this]{this->run();});
    thread_event_bus.detach();
}

EventBus::~EventBus() {}

returnType EventBus::AddReceiver(EventReceiver* event_receiver)
{
    receiverId_t receiver_id = event_receiver->GetId();
    returnType ret = RET_RECEIVER_EXISTS;

    std::lock_guard<std::mutex> receivers_lock(this->receivers_mtx);
    if (this->event_receivers.find(receiver_id) == this->event_receivers.end())
    {
        this->event_receivers[receiver_id] = event_receiver;

        ret = RET_OK;
    }

    return ret;
}

returnType EventBus::RemoveReceiver(const receiverId_t receiver_id)
{
    returnType ret = RET_RECEIVER_NOT_EXISTS;

    std::lock_guard<std::mutex> receivers_lock(this->receivers_mtx);
    if (this->event_receivers.find(receiver_id) != this->event_receivers.end())
    {
        for (auto event_id : this->event_receivers[receiver_id]->GetEvents())
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

    return ret;
}

returnType EventBus::Subscribe(const receiverId_t receiver_id, const eventId_t event_id)
{
    returnType ret = RET_RECEIVER_NOT_EXISTS;

    if(event_id < EVENT_ID_INVALID)
    {
        std::lock_guard<std::mutex> receivers_lock(this->receivers_mtx);
        if (this->event_receivers.find(receiver_id) != this->event_receivers.end())
        {
            this->event_receivers[receiver_id]->AddEvent(event_id);
            this->events_to_receivers_map[event_id].insert(receiver_id);

            ret = RET_OK;
        }
    }
    else
    {
        ret = RET_EVENT_ID_INVALID;
    }

    return ret;
}

returnType EventBus::Unsubscribe(const receiverId_t receiver_id, const eventId_t event_id)
{
    returnType ret = RET_RECEIVER_NOT_EXISTS;

    if(event_id < EVENT_ID_INVALID)
    {
        std::lock_guard<std::mutex> receivers_lock(this->receivers_mtx);
        if (this->event_receivers.find(receiver_id) != this->event_receivers.end())
        {
            this->event_receivers[receiver_id]->RemoveEvent(event_id);

            if (this->events_to_receivers_map[event_id].erase(receiver_id))
            {
                if (this->events_to_receivers_map[event_id].empty())
                {
                    this->events_to_receivers_map.erase(event_id);
                }
            }

            ret = RET_OK;
        }
    }
    else
    {
        ret = RET_EVENT_ID_INVALID;
    }

    return ret;
}

void EventBus::Send(const Event& event)
{
    std::lock_guard<std::mutex> event_queue_lock(this->event_queue_mtx);
    this->event_queue.push(event);
}

void EventBus::Cyclic(void)
{
    Event event;
    std::function<returnType(Event&)> filter;
    bool flag = true;

    std::unique_lock<std::mutex> event_queue_lock(this->event_queue_mtx);
    if (!this->event_queue.empty())
    {
        event = this->event_queue.front();
        this->event_queue.pop();
    }
    event_queue_lock.unlock();

    if (EVENT_ID_INVALID != event.GetEventId())
    {
        std::lock_guard<std::mutex> receivers_lock(this->receivers_mtx);
        if (this->events_to_receivers_map.find(event.GetEventId()) != this->events_to_receivers_map.end())
        {
            for (auto receiver_id : this->events_to_receivers_map[event.GetEventId()])
            {
                if(RET_OK == this->event_receivers[receiver_id]->Filter(event))
                {
                    std::thread t(this->event_receivers[receiver_id]->GetCallback(), event);
                    t.detach();
                }
            }
        }
    }
}

void EventBus::run(void)
{
    while (true)
    {
        this->Cyclic();
    }
}