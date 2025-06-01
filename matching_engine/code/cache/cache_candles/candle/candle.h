#ifndef CANDLE_H
#define CANDLE_H

#include "types.h"

#include <string>
#include <ctime>

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

            time_t timestamp;
        
            Candle();

            Candle(time_t timestamp);

            Candle(string price, time_t timestamp);

            Candle(string high, string open, string close, string low, time_t timestamp);

            ~Candle();

            bool IsEmpty();

            json ConvertCandleToJson();
    };

    void to_json(json &j, const Candle c);
}

#endif