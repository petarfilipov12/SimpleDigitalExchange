#ifndef ORDER_H
#define ORDER_H

#include <functional>

using namespace std;

enum OrderSide{
    BUY,
    SELL,
    INVALID
};

class Order{
    public:
        float price;
        int id;
        enum OrderSide order_side;
    
        Order(){
            this->price = 0.0;
            this->id = 0;
            this->order_side = INVALID;
        }

        Order(int id){
            this->price = 0.0;
            this->id = id;
            this->order_side = INVALID;
        }

        Order(float price, int id, enum OrderSide order_side){
            this->price = price;
            this->id = id;
            this->order_side = order_side;
        }

        // bool operator<(Order order2) const{
        //     return this->id < order2.id;
        // }

        bool operator==(Order order2) const{
            return this->id == order2.id;
        }

        class HashFunc{
            public:
                size_t operator()(const Order& order) const{
                    return hash<int>{}(order.id);
                }
        };

        void PrintOrder(){
            cout << this->price << ", id=" << this->id << endl;
        }
};

#endif