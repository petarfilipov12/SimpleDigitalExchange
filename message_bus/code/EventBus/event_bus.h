#ifndef EVENT_BUS_H
#define EVENT_BUS_H

#include <queue>
#include <set>
#include <map>
#include <functional>
#include "event.h"
#include "event_receiver.h"

using namespace std;

class EventBus{
    private:
        map< int, EventReceiver > event_receivers;
        map< int, set<int> > events_to_receivers_map;
        queue<Event_t> event_queue;

        //void Cyclic(void);
    
    public:
        EventBus();
        ~EventBus();

        void Subscribe(int receiver_id, int event_id, function<void(Event_t)> handler_func);

        void Unsubscribe(int receiver_id, int event_id);

        void Cyclic(void);

        void Send(Event_t event);

        void run(void);
};

#endif