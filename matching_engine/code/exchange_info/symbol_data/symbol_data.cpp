#include "symbol_data.h"

void symbol_data::to_json(json &j, const struct SymbolData s_data)
{
    j = {
        {"symbol", s_data.symbol}};
}
