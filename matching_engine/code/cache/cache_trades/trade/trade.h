#ifndef TRADE_H
#define TRADE_H

#include "return_type.h"

#include <string>
#include <ctime>



#include "json.h"



namespace trade
{
    struct sTrade
    {
        std::string price;
        float quantity;
        time_t timestamp;
    };
    
    void to_json(json& j, const struct sTrade t);
}

#endif