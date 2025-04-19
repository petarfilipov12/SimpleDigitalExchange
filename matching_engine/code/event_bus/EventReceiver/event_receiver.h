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
    unordered_set<enum eEventId_t> events;
    function<void(Event)> callback;
public:
    EventReceiver();

    EventReceiver(int id, function<void(Event)> callback);

    ~EventReceiver();

    void AddEvent(enum eEventId_t event_id);

    void RemoveEvent(enum eEventId_t event_id);

    bool HaveEvents() const;

    bool ContainsEvent(enum eEventId_t event_id);

    unordered_set<enum eEventId_t> GetEvents() const;

    int GetId() const;

    function<void(Event)> GetCallback() const;

    bool operator<(const EventReceiver &event_receiver2) const;
};

#endif