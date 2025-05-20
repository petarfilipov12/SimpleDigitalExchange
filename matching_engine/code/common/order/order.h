#ifndef ORDER_H
#define ORDER_H

#include <string>
#include<nlohmann/json.hpp>
using json = nlohmann::json;

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

        Order(string price, float quantity, int id, enum eOrderSide_t order_side, enum eOrderType_t order_type, bool status);

        json ConvertOrderToJson();

        static string Convert_OrderSide_String(enum eOrderSide_t order_side);

        static string Convert_OrderType_String(enum eOrderType_t order_type);

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

Order ConvertJsonToOrder(json *j_data);

#endif