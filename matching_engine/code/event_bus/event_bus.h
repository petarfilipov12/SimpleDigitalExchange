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

    ReturnType AddReceiver(const enum eReceiverId_t receiver_id, const function<void(Event)> handler_func);

    ReturnType RemoveReceiver(const enum eReceiverId_t receiver_id);

    ReturnType Subscribe(const enum eReceiverId_t receiver_id, const enum eEventId_t event_id);

    ReturnType Unsubscribe(const enum eReceiverId_t receiver_id, const enum eEventId_t event_id);

    void Send(const Event event);

    void run(void);
};

#endif