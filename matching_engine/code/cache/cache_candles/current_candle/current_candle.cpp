#include "current_candle.h"

CurrentCandle::CurrentCandle()
{
    this->InitCurrentCandle();
}

CurrentCandle::~CurrentCandle() {}

void CurrentCandle::InitCurrentCandle()
{
    this->high_s = "0.0";
    this->high_f = 0.0;

    this->open_s = "0.0";

    this->close_s = "0.0";

    this->low_s = "0.0";
    this->low_f = 0.0;
}

bool CurrentCandle::IsEmpty()
{
    return this->high_f <= 0.0;
}

Candle CurrentCandle::GetCandle()
{
    return Candle(this->high_s, this->open_s, this->close_s, this->low_s);
}