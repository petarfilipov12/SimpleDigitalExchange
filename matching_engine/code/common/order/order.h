#ifndef ORDER_H
#define ORDER_H

#include <string>
#include<nlohmann/json.hpp>
using json = nlohmann::json;

#include <iostream>

using namespace std;

enum eOrderSide_t
{
    ORDER_SIDE_BUY,
    ORDER_SIDE_SELL,
    ORDER_SIDE_INVALID
};

enum eOrderType_t
{
    ORDER_TYPE_MARKET,
    ORDER_TYPE_LIMIT,
    ORDER_TYPE_INVALID
};

class Order{
    public:
        string price;
        float quantity;
        float filled;
        int id;
        enum eOrderSide_t order_side;
        enum eOrderType_t order_type;
        bool status;
    
        Order();

        Order(int id);

        Order(string price, float quantity, float filled, int id, enum eOrderSide_t order_side, enum eOrderType_t order_type, bool status);

        json ConvertOrderToJson();

        // bool operator<(Order order2) const{
        //     return this->id < order2.id;
        // }

        Order& operator=(const Order& order2);

        bool operator==(const Order& order2) const;

        class HashFunc{
            public:
                size_t operator()(const Order& order) const
                {
                    return hash<int>{}(order.id);
                }
        };
};

inline Order ConvertJsonToOrder(json j_data){
    return Order(
        (j_data)["price"],
        (j_data)["quantity"],
        (j_data)["filled"],
        (j_data)["order_id"],
        static_cast<enum eOrderSide_t>((j_data)["order_side"]),
        static_cast<enum eOrderType_t>((j_data)["order_type"]),
        (j_data)["status"]
    );

    // cout << "J_DATA: " << j_data << "\n";
    // string price = (j_data).at("price");
    // float qty = (j_data).at("quantity");
    // int order_id = (j_data).at("order_id");
    // enum eOrderSide_t order_side = static_cast<enum eOrderSide_t>((j_data).at("order_side"));
    // enum eOrderType_t order_type = static_cast<enum eOrderType_t>((j_data).at("order_type"));
    // bool status = (j_data).at("status");

    // return Order(
    //         price,
    //         qty,
    //         order_id,
    //         order_side,
    //         order_type,
    //         status
    //     );
}

#endif