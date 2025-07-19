#ifndef CACHE_H
#define CACHE_H

#include "return_type.h"

#include <string>
#include <vector>

#include "event.h"
#include "event_bus.h"
#include "event_receiver.h"

class Cache
{
    protected:
        std::string symbol;
        EventReceiver event_receiver;

        Cache(const std::string& symbol, const EventReceiver& event_receiver);

        ~Cache();

        void init(EventBus& event_bus, const std::vector<eventId_t>& events_to_subscribe);
    
    public:
        virtual void EventHandler(Event event) = 0;

        virtual returnType Filter(Event& event);
};

#endif