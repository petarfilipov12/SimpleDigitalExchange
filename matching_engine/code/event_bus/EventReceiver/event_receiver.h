#ifndef EVENT_RECEIVER_H
#define EVENT_RECEIVER_H

#include <unordered_set>
#include <functional>

#include "event.h"

using namespace std;

class EventReceiver
{
private:
    int id;
    unordered_set<int> events;
    function<void(Event)> callback;
public:
    EventReceiver();

    EventReceiver(int id, function<void(Event)> callback);

    ~EventReceiver();

    void AddEvent(int event_id);

    void RemoveEvent(int event_id);

    bool HaveEvents() const;

    bool ContainsEvent(int event_id);

    unordered_set<int> GetEvents() const;

    int GetId() const;

    function<void(Event)> GetCallback() const;

    bool operator<(const EventReceiver &event_receiver2) const;
};

#endif