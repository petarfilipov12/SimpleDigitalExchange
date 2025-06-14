#ifndef CACHE_TRADES_H
#define CACHE_TRADES_H

#include "types.h"

#include <vector>
#include <string>
#include <mutex>

#include "trade.h"
#include "event.h"
#include "event_bus.h"

using namespace std;

#include<nlohmann/json.hpp>
using json = nlohmann::json;

using namespace trade;

class CacheTrades
{
    private:
        vector<sTrade> trades;

        mutable mutex trades_lock;

        void EventHandler_OrderFilled(Event& event);

        void EventHandler_GetTrades(Event& event);
    
    public:
        CacheTrades();
        ~CacheTrades();

        ReturnType OrderFilled(const string& price, const float quantity);

        ReturnType GetTrades(int limit, json& data)const;

        void EventHandler(Event event);

        void init(EventBus& event_bus);
        
};

#endif