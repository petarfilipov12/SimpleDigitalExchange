#ifndef BOOK_H
#define BOOK_H

#include <map>
#include <list>
#include <unordered_set>
#include <algorithm>

#include "order.h"

using namespace std;

template <typename Comparator> class Book{
    private:
        map<float, list<Order>, Comparator > book;
        unordered_set<Order, Order::HashFunc> orders;
    
    public:
        Book(){}

        ~Book(){
            this->book.clear();
            this->orders.clear();
        }

        bool ExistsOrder(Order order){
            return this->orders.find(order) != this->orders.end();
        }

        bool ExistsOrderId(int id){
            return this->ExistsOrder(Order(id));
        }

        void AddOrder(Order order){
            if(this->orders.find(order) == this->orders.end()){
                this->book[order.price].push_back(order);
                this->orders.insert(order);
            }
        }

        void CancelOrder(Order order){
            unordered_set<Order, Order::HashFunc>::iterator pOrder = this->orders.find(order);

            if(pOrder != this->orders.end()){
                list<Order>::iterator pBookOrder = find(this->book[pOrder->price].begin(), this->book[pOrder->price].end(), *pOrder);

                if(pBookOrder != this->book[pOrder->price].end()){
                    this->book[pOrder->price].erase(pBookOrder);
                }

                this->orders.erase(pOrder);
            }
        }

        void CancelOrderById(int id){
            this->CancelOrder(Order(id));
        }

        Order GetFirst(){
            return this->book.begin()->second.front();
        }


        void PrintBook(){
            for(auto price_level : this->book){
                for(auto order : price_level.second){
                    order.PrintOrder();
                }
            }
        }
};

#endif