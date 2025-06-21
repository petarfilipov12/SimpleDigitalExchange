#ifndef CACHE_H
#define CACHE_H

#include "return_type.h"

#include <string>
#include <vector>

#include "event.h"
#include "event_bus.h"

class Cache
{
    protected:
        std::string symbol;
    
    public:
        Cache(const std::string& symbol)
        {
            this->symbol = symbol;
        }

        ~Cache() {}

        virtual void init(EventBus& event_bus, EventReceiver& event_receiver, const std::vector<eventId_t>& events_to_subscribe)
        {
            receiverId_t receiver_id = event_receiver.GetId();
        
            event_bus.AddReceiver(event_receiver);
            
            for(int i = 0; i < events_to_subscribe.size(); i++)
            {
                event_bus.Subscribe(receiver_id, events_to_subscribe.at(i));
            }
        }

        virtual void EventHandler(Event event) = 0;

        returnType Filter(Event& event)
        {
            returnType ret = RET_NOT_OK;
        
            if(this->symbol == event.GetJsonData()["symbol"])
            {
                ret = RET_OK;
            }
        
            return ret;
        }
};

#endif