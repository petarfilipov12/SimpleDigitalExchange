#ifndef CACHE_TRADES_H
#define CACHE_TRADES_H

#include "return_type.h"

#include <vector>
#include <string>
#include <mutex>

#include "trade.h"
#include "event.h"
#include "event_bus.h"

#include "json.h"


class CacheTrades
{
    private:
        std::vector<trade::sTrade> trades;

        mutable std::mutex trades_lock;

        void EventHandler_OrderFilled(Event& event);

        void EventHandler_GetTrades(Event& event);
    
    public:
        CacheTrades();
        ~CacheTrades();

        returnType OrderFilled(const std::string& price, const float quantity);

        returnType GetTrades(int limit, json& data)const;

        void EventHandler(Event event);

        void init(EventBus& event_bus);
        
};

#endif