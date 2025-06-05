#include "candle.h"

using namespace candle;

Candle::Candle() 
{
    this->high = "";
    this->open = "";
    this->close = "";
    this->low = "";

    this->timestamp = 0;
}

Candle::Candle(time_t timestamp) 
{
    this->high = "";
    this->open = "";
    this->close = "";
    this->low = "";

    this->timestamp = timestamp;
}

Candle::Candle(string price, time_t timestamp)
{
    this->high = price;
    this->open = price;
    this->close = price;
    this->low = price;

    this->timestamp = timestamp;
}

Candle::Candle(string high, string open, string close, string low, time_t timestamp)
{
    this->high = high;
    this->open = open;
    this->close = close;
    this->low = low;

    this->timestamp = timestamp;
}

Candle::~Candle() {}

bool Candle::IsEmpty()const
{
    return this->open.empty();
}

json Candle::ConvertCandleToJson()const
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