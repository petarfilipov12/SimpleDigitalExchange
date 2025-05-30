#ifndef CANDLE_H
#define CANDLE_H

#include "types.h"

#include <string>

using namespace std;

#include<nlohmann/json.hpp>
using json = nlohmann::json;


namespace candle
{
    class Candle
    {
        public:
            string high;
            string open;
            string close;
            string low;
        
            Candle();

            Candle(string price);

            Candle(string high, string open, string close, string low);

            ~Candle();

            json ConvertCandleToJson();
    };

    void to_json(json &j, const Candle c);
}

#endif