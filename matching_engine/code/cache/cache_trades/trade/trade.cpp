#include "trade.h"

using namespace trade;

void trade::to_json(json& j, const struct sTrade t)
{
    j = {
        {"price", t.price},
        {"amount", t.amount}
    };
}