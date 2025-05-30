#include "candle.h"

using namespace candle;

Candle::Candle() {}

Candle::Candle(string price)
{
    this->high = price;
    this->open = price;
    this->close = price;
    this->low = price;
}

Candle::Candle(string high, string open, string close, string low)
{
    this->high = high;
    this->open = open;
    this->close = close;
    this->low = low;
}

Candle::~Candle() {}

json Candle::ConvertCandleToJson()
{
    json j_candle = {
        {"high", this->high},
        {"open", this->open},
        {"close", this->close},
        {"low", this->low},
    };

    return j_candle;
}

void candle::to_json(json &j, const Candle c)
{
    j = {
        {"high", c.high},
        {"open", c.open},
        {"close", c.close},
        {"low", c.low},
    };
}