#ifndef CANDLE_H
#define CANDLE_H

#include "return_type.h"

#include <string>
#include <ctime>



#include "json.h"



namespace candle
{
    class Candle
    {
        public:
            std::string high;
            std::string open;
            std::string close;
            std::string low;

            time_t timestamp;
        
            Candle();

            Candle(const time_t timestamp);

            Candle(const std::string& price, const time_t timestamp);

            Candle(const std::string& high, const std::string& open, const std::string& close, const std::string& low, const time_t timestamp);

            ~Candle();

            bool IsEmpty()const;

            json ConvertCandleToJson()const;
    };

    void to_json(json &j, const Candle c);
}

#endif