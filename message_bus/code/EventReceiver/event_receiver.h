#ifndef EVENT_RECEIVER_H
#define EVENT_RECEIVER_H

#include <set>
#include <functional>
#include "event.h"

using namespace std;

class EventReceiver{
    private:
        int id;
        set<int> event_list;
        function<void(Event_t)> callback;

    public:
        EventReceiver();
        EventReceiver(int id, function<void(Event_t)> callback);
        ~EventReceiver();

        void AddEvent(int event_id);

        void RemoveEvent(int event_id);

        bool HaveEvents() const;

        int GetId() const;

        void SetCallback(function<void(Event_t)> callback);

        void Callback(Event_t event) const;

        bool operator< (const EventReceiver &right) const;

};

#endif