#ifndef ORDER_H
#define ORDER_H

#include <iostream>
#include <functional>

#include "order_enums.h"

using namespace std;

class Order{
    public:
        string price;
        int id;
        enum OrderSide order_side;
        enum OrderType order_type;
    
        Order(){
            this->price = "0.0";
            this->id = 0;
            this->order_side = ORDER_SIDE_INVALID;
            this->order_type = ORDER_TYPE_INVALID;
        }

        Order(int id){
            this->price = "0.0";
            this->id = id;
            this->order_side = ORDER_SIDE_INVALID;
            this->order_type = ORDER_TYPE_INVALID;
        }

        Order(string price, int id, enum OrderSide order_side, enum OrderType order_type){
            this->price = price;
            this->id = id;
            this->order_side = order_side;
            this->order_type = order_type;
        }

        // bool operator<(Order order2) const{
        //     return this->id < order2.id;
        // }

        Order& operator=(const Order& order2){
            this->price = order2.price;
            this->id = order2.id;
            this->order_side = order2.order_side;
            this->order_type = order2.order_type;

            return *this;
        }

        bool operator==(const Order& order2) const{
            return this->id == order2.id;
        }

        class HashFunc{
            public:
                size_t operator()(const Order& order) const{
                    return hash<int>{}(order.id);
                }
        };

        void PrintOrder(){
            string s_order_side, s_order_type;

            if(this->order_side == ORDER_SIDE_BUY){
                s_order_side = "ORDER_SIDE_BUY";
            }
            else if(this->order_side == ORDER_SIDE_SELL){
                s_order_side = "ORDER_SIDE_SELL";
            }
            else if(this->order_side == ORDER_SIDE_INVALID){
                s_order_side = "ORDER_SIDE_INVALID";
            }
            else{
                s_order_side = "NA";
            }

            if(this->order_type == ORDER_TYPE_MARKET){
                s_order_type = "ORDER_TYPE_MARKET";
            }
            else if(this->order_type == ORDER_TYPE_LIMIT){
                s_order_type = "ORDER_TYPE_LIMIT";
            }
            else if(this->order_type == ORDER_TYPE_INVALID){
                s_order_type = "ORDER_TYPE_INVALID";
            }
            else{
                s_order_type = "NA";
            }

            cout << "id=" << this->id << ", price=" << this->price << ", side=" << s_order_side << ", type=" << s_order_type << endl;
        }
};

#endif