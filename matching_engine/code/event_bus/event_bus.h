#ifndef EVENT_BUS_H
#define EVENT_BUS_H

#include <queue>
#include <unordered_set>
#include <map>
#include <functional>
#include <mutex>

#include "types.h"
#include "event.h"
#include "event_receiver.h"

using namespace std;

class EventBus
{
private:
    map<enum eReceiverId_t, EventReceiver> event_receivers;
    map<enum eEventId_t, unordered_set<enum eReceiverId_t> > events_to_receivers_map;
    queue<Event> event_queue;

    mutable mutex receivers_lock;
    mutable mutex event_queue_lock;

    void Cyclic(void);

public:
    EventBus();

    ~EventBus();

    ReturnType AddReceiver(enum eReceiverId_t receiver_id, function<void(Event)> handler_func);

    ReturnType RemoveReceiver(enum eReceiverId_t receiver_id);

    ReturnType Subscribe(enum eReceiverId_t receiver_id, enum eEventId_t event_id);

    ReturnType Unsubscribe(enum eReceiverId_t receiver_id, enum eEventId_t event_id);

    void Send(Event event);

    void run(void);
};

#endif