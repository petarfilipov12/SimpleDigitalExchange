#ifndef TRADE_H
#define TRADE_H

#include "types.h"

#include <string>
#include <ctime>

using namespace std;

#include<nlohmann/json.hpp>
using json = nlohmann::json;


namespace trade
{
    struct sTrade
    {
        string price;
        float quantity;
        time_t timestamp;
    };
    
    void to_json(json& j, const struct sTrade t);
}

#endif