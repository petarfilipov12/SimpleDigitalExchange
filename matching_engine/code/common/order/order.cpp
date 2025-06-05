#include "order.h"

#include <iostream>

using namespace std;

Order::Order()
{
    this->price = "0.0";
    this->quantity = 0.0;
    this->filled = 0.0;
    this->id = 0;
    this->order_side = ORDER_SIDE_INVALID;
    this->order_type = ORDER_TYPE_INVALID;
    this->status = false;
}

Order::Order(int id)
{
    this->price = "0.0";
    this->quantity = 0.0;
    this->filled = 0.0;
    this->id = id;
    this->order_side = ORDER_SIDE_INVALID;
    this->order_type = ORDER_TYPE_INVALID;
    this->status = false;
}

Order::Order(
    string price,
    float quantity,
    float filled,
    int id,
    enum eOrderSide_t order_side,
    enum eOrderType_t order_type,
    bool status)
{
    this->price = price;
    this->quantity = quantity;
    this->filled = filled;
    this->id = id;
    this->order_side = order_side;
    this->order_type = order_type;
    this->status = status;
}

json Order::ConvertOrderToJson()
{
    json j_data;

    j_data["order_id"] = this->id;
    j_data["price"] = this->price;
    j_data["quantity"] = this->quantity;
    j_data["filled"] = this->filled;
    j_data["order_side"] = this->order_side;
    j_data["order_type"] = this->order_type;
    j_data["status"] = this->status;
    
    return j_data;
}

// bool operator<(Order order2) const{
//     return this->id < order2.id;
// }

Order &Order::operator=(const Order &order2)
{
    this->price = order2.price;
    this->quantity = order2.quantity;
    this->filled = order2.filled;
    this->id = order2.id;
    this->order_side = order2.order_side;
    this->order_type = order2.order_type;
    this->status = order2.status;

    return *this;
}

bool Order::operator==(const Order &order2) const
{
    return this->id == order2.id;
}