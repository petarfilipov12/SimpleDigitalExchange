#include "candle.h"

candle::Candle::Candle() 
{
    this->high = "";
    this->open = "";
    this->close = "";
    this->low = "";

    this->timestamp = 0;
}

candle::Candle::Candle(const time_t timestamp) 
{
    this->high = "";
    this->open = "";
    this->close = "";
    this->low = "";

    this->timestamp = timestamp;
}

candle::Candle::Candle(const std::string& price, const time_t timestamp)
{
    this->high = price;
    this->open = price;
    this->close = price;
    this->low = price;

    this->timestamp = timestamp;
}

candle::Candle::Candle(const std::string& high, const std::string& open, const std::string& close, const std::string& low, const time_t timestamp)
{
    this->high = high;
    this->open = open;
    this->close = close;
    this->low = low;

    this->timestamp = timestamp;
}

candle::Candle::~Candle() {}

bool candle::Candle::IsEmpty()const
{
    return this->open.empty();
}

json candle::Candle::ConvertCandleToJson()const
{
    json j_candle = {
        {"high", this->high},
        {"open", this->open},
        {"close", this->close},
        {"low", this->low},
        {"timestamp", this->timestamp},
    };

    return j_candle;
}

void candle::to_json(json &j, const Candle c)
{
    j = c.ConvertCandleToJson();
}