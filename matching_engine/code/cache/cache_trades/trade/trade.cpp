#include "trade.h"

void trade::to_json(json& j, const struct sTrade t)
{
    j = {
        {"price", t.price},
        {"quantity", t.quantity},
        {"timestamp", t.timestamp}
    };
}