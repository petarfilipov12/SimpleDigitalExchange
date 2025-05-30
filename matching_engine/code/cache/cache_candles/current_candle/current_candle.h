#ifndef CURRENT_CANDLE_H
#define CURRENT_CANDLE_H

#include "types.h"

#include <string>

#include "candle.h"

using namespace std;

using namespace candle;

class CurrentCandle
{
    public:
        string high_s;
        string open_s;
        string close_s;
        string low_s;

        float high_f;
        float low_f;

        CurrentCandle();
        ~CurrentCandle();
    
        void InitCurrentCandle();

        bool IsEmpty();

        Candle GetCandle();
};

#endif