#ifndef ORDER_H
#define ORDER_H

#include <string>

using namespace std;

enum OrderSide{
    ORDER_SIDE_BUY,
    ORDER_SIDE_SELL,
    ORDER_SIDE_INVALID
};

enum OrderType{
    ORDER_TYPE_MARKET,
    ORDER_TYPE_LIMIT,
    ORDER_TYPE_INVALID
};

class Order{
    public:
        string price;
        int id;
        enum OrderSide order_side;
        enum OrderType order_type;
    
        Order();

        Order(int id);

        Order(string price, int id, enum OrderSide order_side, enum OrderType order_type);

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