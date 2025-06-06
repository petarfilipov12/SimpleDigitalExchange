#ifndef TRADE_H
#define TRADE_H

#include "types.h"

#include <string>

using namespace std;

#include<nlohmann/json.hpp>
using json = nlohmann::json;


namespace trade
{
    struct sTrade
    {
        string price;
        float amount;
    };
    
    void to_json(json& j, const struct sTrade t);
}

#endif