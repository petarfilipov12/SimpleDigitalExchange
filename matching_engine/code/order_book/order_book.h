#ifndef ORDER_BOOK_H
#define ORDER_BOOK_H

#include <iostream>

#include "book.h"
#include "order.h"

using namespace std;

class OrderBook{
    private:
        Book<greater<string> > bid_book;
        Book<less<string> > ask_book;
    public:
        OrderBook(){}
        
        ~OrderBook(){
            // this->bid_book.~Book();
            // this->ask_book.~Book();
        }

        bool ExistsOrder(Order order) const{
            bool ret = false;

            if(order.order_side == ORDER_SIDE_BUY){
                ret = this->bid_book.ExistsOrder(order);
            }
            else if(order.order_side == ORDER_SIDE_SELL){
                ret = this->ask_book.ExistsOrder(order);
            }
            else{
                
            }

            return ret;
        }

        bool ExistsOrderId(int id) const{
            Order order(id);
            bool ret = this->bid_book.ExistsOrder(order);

            if(!ret){
                ret = this->ask_book.ExistsOrder(order);
            }

            return ret;
        }

        void AddOrder(Order order){
            if(order.order_side == ORDER_SIDE_BUY){
                this->bid_book.AddOrder(order);
            }
            else if(order.order_side == ORDER_SIDE_SELL){
                this->ask_book.AddOrder(order);
            }
            else{
                //Nothing to do
            }
        }

        void CancelOrder(Order order){
            this->bid_book.CancelOrder(order);
            this->ask_book.CancelOrder(order);
        }

        void CancelOrderById(int id){
            this->CancelOrder(Order(id));
        }

        Order GetBidFirst(){
            return this->bid_book.GetFirst();
        }

        Order GetAskFirst(){
            return this->ask_book.GetFirst();
        }

        void PrintOrderBook(){
            cout << "bid_book:" << endl;
            this->bid_book.PrintBook();

            cout << "------------" << endl;

            cout << "ask_book:" << endl;
            this->ask_book.PrintBook();

        }

};

#endif