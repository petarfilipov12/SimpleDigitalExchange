#ifndef EVENT_BUS_H
#define EVENT_BUS_H

#include <queue>
#include <unordered_set>
#include <map>
#include <functional>
#include <mutex>

#include "return_type.h"
#include "event.h"
#include "event_receiver.h"



class EventBus
{
private:
    std::map<receiverId_t, EventReceiver> event_receivers;
    std::map<eventId_t, std::unordered_set<receiverId_t> > events_to_receivers_map;
    std::queue<Event> event_queue;

    mutable std::mutex receivers_lock;
    mutable std::mutex event_queue_lock;

    void Cyclic(void);

public:
    EventBus();

    ~EventBus();

    returnType AddReceiver(EventReceiver& event_receiver);

    returnType RemoveReceiver(const receiverId_t receiver_id);

    returnType Subscribe(const receiverId_t receiver_id, const eventId_t event_id);

    returnType Unsubscribe(const receiverId_t receiver_id, const eventId_t event_id);

    void Send(const Event& event);

    void run();

    void init();
};

#endif