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
    map<int, EventReceiver> event_receivers;
    map<int, unordered_set<int> > events_to_receivers_map;
    queue<Event> event_queue;

    mutable mutex receivers_lock;
    mutable mutex event_queue_lock;

    // void Cyclic(void);

public:
    EventBus();

    ~EventBus();

    Return_Type AddReceiver(int receiver_id, function<void(Event)> handler_func);

    Return_Type RemoveReceiver(int receiver_id);

    Return_Type Subscribe(int receiver_id, int event_id);

    Return_Type Unsubscribe(int receiver_id, int event_id);

    void Send(Event event);

    void Cyclic(void);

    void run(void);
};

#endif