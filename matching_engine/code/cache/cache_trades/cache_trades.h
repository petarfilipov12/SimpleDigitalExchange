#ifndef CACHE_TRADES_H
#define CACHE_TRADES_H

#include "types.h"

#include <vector>
#include <string>
#include <mutex>

#include "trade.h"

using namespace std;

#include<nlohmann/json.hpp>
using json = nlohmann::json;

using namespace trade;

class CacheTrades
{
    private:
        vector<sTrade> trades;

        mutable mutex trades_lock;
    
    public:
        CacheTrades();
        ~CacheTrades();

        ReturnType OrderFilled(string price, float quantity);

        ReturnType GetTrades(int limit, json* data)const;
        
};

#endif