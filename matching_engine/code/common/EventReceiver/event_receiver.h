#ifndef EVENT_RECEIVER_H
#define EVENT_RECEIVER_H

#include <unordered_set>
#include <functional>

#include "event.h"

using namespace std;

enum eReceiverId_t
{
    RECEIVER_ID_ENGINE,
    RECEIVER_ID_EVENT_LOGGER,
    RECEIVER_ID_CACHE_ORDERS,
    RECEIVER_ID_CACHE_ORDER_BOOK_L2,
    RECEIVER_ID_CACHE_CANDLES,

    RECEIVER_ID_INVALID
};

class EventReceiver
{
private:
    enum eReceiverId_t id;
    unordered_set<enum eEventId_t> events;
    function<void(Event)> callback;
public:
    EventReceiver();

    EventReceiver(enum eReceiverId_t id, function<void(Event)> callback);

    ~EventReceiver();

    void AddEvent(enum eEventId_t event_id);

    void RemoveEvent(enum eEventId_t event_id);

    bool HaveEvents() const;

    bool ContainsEvent(enum eEventId_t event_id);

    unordered_set<enum eEventId_t> GetEvents() const;

    enum eReceiverId_t GetId() const;

    function<void(Event)> GetCallback() const;

    bool operator<(const EventReceiver &event_receiver2) const;
};

#endif