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
    RECEIVER_ID_CACHE_TRADES,

    RECEIVER_ID_INVALID
};

class EventReceiver
{
private:
    enum eReceiverId_t id;
    unordered_set<enum eEventId_t> events;
public:
    EventReceiver();

    EventReceiver(const enum eReceiverId_t id);

    ~EventReceiver();

    void AddEvent(const enum eEventId_t event_id);

    void RemoveEvent(const enum eEventId_t event_id);

    bool HaveEvents() const;

    bool ContainsEvent(const enum eEventId_t event_id) const;

    unordered_set<enum eEventId_t> GetEvents() const;

    enum eReceiverId_t GetId() const;

    /*virtual*/ void CallEventHandler(const Event& event) const {}

    bool operator<(const EventReceiver &event_receiver2) const;
};

#endif