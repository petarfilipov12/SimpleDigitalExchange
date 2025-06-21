#ifndef EVENT_RECEIVER_H
#define EVENT_RECEIVER_H

#include <unordered_set>
#include <functional>

#include "event.h"

typedef int receiverId_t;
#define RECEIVER_ID_INVALID 0xffffu;

class EventReceiver
{
private:
    receiverId_t id;
    std::unordered_set<eventId_t> events;
    std::function<void(Event)> callback;
    std::function<returnType(Event&)> filter;
public:
    EventReceiver();

    EventReceiver(const receiverId_t id, const std::function<void(Event)> callback);

    EventReceiver(
        const receiverId_t id, 
        const std::function<void(Event)> callback, 
        const std::function<returnType(Event&)> filter
    );

    ~EventReceiver();

    void AddEvent(const eventId_t event_id);

    void RemoveEvent(const eventId_t event_id);

    bool HaveEvents() const;

    bool ContainsEvent(const eventId_t event_id) const;

    std::unordered_set<eventId_t> GetEvents() const;

    receiverId_t GetId() const;

    std::function<void(Event)> GetCallback() const;

    returnType Filter(Event& event) const;

    bool operator<(const EventReceiver &event_receiver2) const;
};

#endif