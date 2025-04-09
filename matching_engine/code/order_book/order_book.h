#ifndef ORDER_BOOK_H
#define ORDER_BOOK_H

#include "book.h"
#include "order.h"

using namespace std;

class OrderBook{
    private:
        Book<greater<float> > bid_book;
        Book<less<float> > ask_book;
    public:
        OrderBook(){}

        void AddOrder(Order order){
            if(order.order_side == BUY){
                this->bid_book.AddOrder(order);
            }
            else if(order.order_side == SELL){
                this->ask_book.AddOrder(order);
            }
            else{
                //Nothing to do
            }
        }

        void CancelOrder(Order order){
            if(this->bid_book.ExistsOrder(order)){
                this->bid_book.CancelOrder(order);
            }

            if(this->ask_book.ExistsOrder(order)){
                this->ask_book.CancelOrder(order);
            }
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