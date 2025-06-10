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

            Candle(const time_t timestamp);

            Candle(const string& price, const time_t timestamp);

            Candle(const string& high, const string& open, const string& close, const string& low, const time_t timestamp);

            ~Candle();

            bool IsEmpty()const;

            json ConvertCandleToJson()const;
    };

    void to_json(json &j, const Candle c);
}

#endif