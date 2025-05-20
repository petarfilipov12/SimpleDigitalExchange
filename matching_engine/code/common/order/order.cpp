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

Order::Order(string price, float quantity, int id, enum eOrderSide_t order_side, enum eOrderType_t order_type, bool status)
{
    this->price = price;
    this->quantity = quantity;
    this->filled = 0.0;
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

string Order::Convert_OrderSide_String(enum eOrderSide_t order_side)
{
    string s_order_side;

    if (order_side == ORDER_SIDE_BUY)
    {
        s_order_side = "ORDER_SIDE_BUY";
    }
    else if (order_side == ORDER_SIDE_SELL)
    {
        s_order_side = "ORDER_SIDE_SELL";
    }
    else
    {
        s_order_side = "ORDER_SIDE_INVALID";
    }

    return s_order_side;
}

string Order::Convert_OrderType_String(enum eOrderType_t order_type)
{
    string s_order_type;

    if (order_type == ORDER_TYPE_MARKET)
    {
        s_order_type = "ORDER_TYPE_MARKET";
    }
    else if (order_type == ORDER_TYPE_LIMIT)
    {
        s_order_type = "ORDER_TYPE_LIMIT";
    }
    else
    {
        s_order_type = "ORDER_TYPE_INVALID";
    }

    return s_order_type;
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

Order ConvertJsonToOrder(json *j_data)
{
    return Order(
        (*j_data)["price"],
        (*j_data)["quantity"],
        (*j_data)["order_id"],
        static_cast<enum eOrderSide_t>((*j_data)["order_side"]),
        static_cast<enum eOrderType_t>((*j_data)["order_type"]),
        (*j_data)["status"]
    );
}