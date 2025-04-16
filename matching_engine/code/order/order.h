#ifndef ORDER_H
#define ORDER_H

#include <string>
#include<nlohmann/json.hpp>
using json = nlohmann::json;

using namespace std;


#define ORDER_SIDE_BUY      0u
#define ORDER_SIDE_SELL     1u
#define ORDER_SIDE_INVALID  2u



#define ORDER_TYPE_MARKET   0u
#define ORDER_TYPE_LIMIT    1u
#define ORDER_TYPE_INVALID  2u


class Order{
    public:
        string price;
        int id;
        int order_side;
        int order_type;
    
        Order();

        Order(int id);

        Order(string price, int id, int order_side, int order_type);

        json ConvertToJson();

        static string Convert_OrderSide_String(int order_side);

        static string Convert_OrderType_String(int order_type);

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

        void PrintOrder();

};

#endif