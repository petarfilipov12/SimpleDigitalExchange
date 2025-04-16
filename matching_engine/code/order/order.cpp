#include "order.h"

#include <iostream>

using namespace std;

Order::Order()
{
    this->price = "0.0";
    this->id = 0;
    this->order_side = ORDER_SIDE_INVALID;
    this->order_type = ORDER_TYPE_INVALID;
}

Order::Order(int id)
{
    this->price = "0.0";
    this->id = id;
    this->order_side = ORDER_SIDE_INVALID;
    this->order_type = ORDER_TYPE_INVALID;
}

Order::Order(string price, int id, int order_side, int order_type)
{
    this->price = price;
    this->id = id;
    this->order_side = order_side;
    this->order_type = order_type;
}

json Order::ConvertToJson()
{
    json j_data;

    j_data["order_id"] = this->id;
    j_data["price"] = this->price;
    j_data["order_side"] = this->Convert_OrderSide_String(this->order_side);
    j_data["order_type"] = this->Convert_OrderType_String(this->order_type);

    return j_data;
}

string Order::Convert_OrderSide_String(int order_side)
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

string Order::Convert_OrderType_String(int order_type)
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
    this->id = order2.id;
    this->order_side = order2.order_side;
    this->order_type = order2.order_type;

    return *this;
}

bool Order::operator==(const Order &order2) const
{
    return this->id == order2.id;
}

void Order::PrintOrder()
{
    string s_order_side, s_order_type;

    if (this->order_side == ORDER_SIDE_BUY)
    {
        s_order_side = "ORDER_SIDE_BUY";
    }
    else if (this->order_side == ORDER_SIDE_SELL)
    {
        s_order_side = "ORDER_SIDE_SELL";
    }
    else if (this->order_side == ORDER_SIDE_INVALID)
    {
        s_order_side = "ORDER_SIDE_INVALID";
    }
    else
    {
        s_order_side = "NA";
    }

    if (this->order_type == ORDER_TYPE_MARKET)
    {
        s_order_type = "ORDER_TYPE_MARKET";
    }
    else if (this->order_type == ORDER_TYPE_LIMIT)
    {
        s_order_type = "ORDER_TYPE_LIMIT";
    }
    else if (this->order_type == ORDER_TYPE_INVALID)
    {
        s_order_type = "ORDER_TYPE_INVALID";
    }
    else
    {
        s_order_type = "NA";
    }

    cout << "id=" << this->id << ", price=" << this->price << ", side=" << s_order_side << ", type=" << s_order_type << endl;
}