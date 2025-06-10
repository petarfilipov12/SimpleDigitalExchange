#ifndef ORDER_H
#define ORDER_H

#include <string>
#include<nlohmann/json.hpp>
using json = nlohmann::json;

#include <iostream>

#include <ctime>

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
        time_t order_added_timestamp;
    
        Order();

        Order(const int id);

        Order(
            const string& price,
            const float quantity,
            const float filled,
            const int id,
            const enum eOrderSide_t order_side,
            const enum eOrderType_t order_type,
            const bool status,
            const time_t order_added_timestamp
        );

        void SetCurrentTimestamp();

        json ConvertOrderToJson()const;

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

        static Order ConvertJsonToOrder(const json &j_data);
};

#endif