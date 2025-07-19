#ifndef SYMBOL_DATA_H
#define SYMBOL_DATA_H

#include <string>
#include "json.h"

namespace symbol_data
{
    struct SymbolData
    {
        std::string symbol;
    };

    void to_json(json& j, const struct SymbolData& s_data);
}

#endif