#ifndef CACHE_TRADES_H
#define CACHE_TRADES_H

#include "return_type.h"

#include "cache.h"

#include <mutex>

#include "trade.h"
#include "json.h"


class CacheTrades: public Cache
{
    private:
        std::vector<trade::sTrade> trades;

        mutable std::mutex trades_lock;


        void EventHandler_OrderFilled(Event& event);

        void EventHandler_GetTrades(Event& event);
    
    public:
        CacheTrades(const std::string& symbol);
        ~CacheTrades();

        returnType OrderFilled(const std::string& price, const float quantity);

        returnType GetTrades(int limit, json& data)const;

        void init(EventBus& event_bus, receiverId_t receiver_id);

        void EventHandler(Event event);
};

#endif