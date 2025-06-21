#include "order.h"

Order::Order()
{
    this->symbol = "";
    this->price = "0.0";
    this->quantity = 0.0;
    this->filled = 0.0;
    this->id = 0;
    this->order_side = ORDER_SIDE_INVALID;
    this->order_type = ORDER_TYPE_INVALID;
    this->status = false;
    this->order_added_timestamp = 0;
}

Order::Order(const int id)
{
    this->symbol = "";
    this->price = "0.0";
    this->quantity = 0.0;
    this->filled = 0.0;
    this->id = id;
    this->order_side = ORDER_SIDE_INVALID;
    this->order_type = ORDER_TYPE_INVALID;
    this->status = false;
    this->order_added_timestamp = 0;
}

Order::Order(
    const std::string& symbol,
    const std::string& price,
    const float quantity,
    const float filled,
    const int id,
    const orderSide_t order_side,
    const orderType_t order_type,
    const bool status,
    const time_t order_added_timestamp
)
{
    this->symbol = symbol;
    this->price = price;
    this->quantity = quantity;
    this->filled = filled;
    this->id = id;
    this->order_side = order_side;
    this->order_type = order_type;
    this->status = status;
    this->order_added_timestamp = order_added_timestamp;
}

void Order::SetCurrentTimestamp()
{
    this->order_added_timestamp = time(nullptr);
}

json Order::ConvertOrderToJson()const
{
    json j_data;

    j_data["symbol"] = this->symbol;
    j_data["order_id"] = this->id;
    j_data["price"] = this->price;
    j_data["quantity"] = this->quantity;
    j_data["filled"] = this->filled;
    j_data["order_side"] = this->order_side;
    j_data["order_type"] = this->order_type;
    j_data["status"] = this->status;
    j_data["order_added_timestamp"] = this->order_added_timestamp;
    
    return j_data;
}

// bool operator<(Order order2) const{
//     return this->id < order2.id;
// }

Order &Order::operator=(const Order &order2)
{
    this->symbol = order2.symbol;
    this->price = order2.price;
    this->quantity = order2.quantity;
    this->filled = order2.filled;
    this->id = order2.id;
    this->order_side = order2.order_side;
    this->order_type = order2.order_type;
    this->status = order2.status;
    this->order_added_timestamp = order2.order_added_timestamp;

    return *this;
}

bool Order::operator==(const Order &order2) const
{
    return this->id == order2.id;
}

Order Order::ConvertJsonToOrder(const json &j_data){
    return Order(
        j_data["symbol"],
        j_data["price"],
        j_data["quantity"],
        j_data["filled"],
        j_data["order_id"],
        static_cast<orderSide_t>(j_data["order_side"]),
        static_cast<orderType_t>(j_data["order_type"]),
        j_data["status"],
        j_data["order_added_timestamp"]
    );
}