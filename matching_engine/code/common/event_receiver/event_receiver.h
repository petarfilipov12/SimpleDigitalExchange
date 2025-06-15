#ifndef EVENT_RECEIVER_H
#define EVENT_RECEIVER_H

#include <unordered_set>
#include <functional>

#include "event.h"

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
    std::unordered_set<eventId_t> events;
    std::function<void(Event)> callback;
public:
    EventReceiver();

    EventReceiver(const enum eReceiverId_t id, const std::function<void(Event)> callback);

    ~EventReceiver();

    void AddEvent(const eventId_t event_id);

    void RemoveEvent(const eventId_t event_id);

    bool HaveEvents() const;

    bool ContainsEvent(const eventId_t event_id) const;

    std::unordered_set<eventId_t> GetEvents() const;

    enum eReceiverId_t GetId() const;

    std::function<void(Event)> GetCallback() const;

    bool operator<(const EventReceiver &event_receiver2) const;
};

#endif