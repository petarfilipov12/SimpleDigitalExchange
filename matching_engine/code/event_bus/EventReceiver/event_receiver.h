#ifndef EVENT_RECEIVER_H
#define EVENT_RECEIVER_H

#include <unordered_set>
#include <functional>

#include "event.h"

using namespace std;

#define RECEIVER_ID_INVALID             0
#define RECEIVER_ID_ENGINE              1
#define RECEIVER_ID_EVENT_LOGGER        2
#define RECEIVER_ID_CACHE_ORDERS        3
#define RECEIVER_ID_CACHE_ORDER_BOOK_L2 4
#define RECEIVER_ID_CACHE_CANDLES       5

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