#ifndef BOOK_H
#define BOOK_H

#include <map>
#include <list>
#include <unordered_set>
#include <algorithm>
#include <mutex>

#include "types.h"
#include "order.h"

using namespace std;

template <typename Comparator> class Book{
    private:
        map<string, list<Order>, Comparator > book;
        map<string, float, Comparator > book_l2;
        unordered_set<Order, Order::HashFunc> orders;

        mutable mutex book_lock;
    
    public:
        Book(){}

        ~Book(){
            // this->book.clear();
            // this->orders.clear();
        }

        bool ExistsOrder(Order order) const{
            this->book_lock.lock();
            bool ret = this->orders.find(order) != this->orders.end();
            this->book_lock.unlock();

            return ret;
        }

        bool ExistsOrderId(int id) const{
            return this->ExistsOrder(Order(id));
        }

        Return_Type AddOrder(Order order){
            Return_Type ret = RET_ORDER_EXISTS;

            this->book_lock.lock();
            if(this->orders.find(order) == this->orders.end()){
                this->book[order.price].push_back(order);
                this->orders.insert(order);

                this->book_l2[order.price] += order.quantity;

                ret = RET_OK;
            }
            this->book_lock.unlock();

            return ret;
        }

        Return_Type CancelOrder(Order order){
            Return_Type ret = RET_ORDER_NOT_EXISTS;

            this->book_lock.lock();
            unordered_set<Order, Order::HashFunc>::iterator pOrder = this->orders.find(order);

            if(pOrder != this->orders.end()){
                list<Order>::iterator pBookOrder = find(this->book[pOrder->price].begin(), this->book[pOrder->price].end(), *pOrder);

                if(pBookOrder != this->book[pOrder->price].end()){
                    this->book[pOrder->price].erase(pBookOrder);

                    if(this->book[pOrder->price].empty()){
                        this->book.erase(pOrder->price);
                        this->book_l2.erase(pOrder->price);
                    }
                    else
                    {
                        this->book_l2[pOrder->price] -= (order.quantity - order.filled);
                    }
                }

                this->orders.erase(pOrder);

                ret = RET_OK;
            }
            this->book_lock.unlock();

            return ret;
        }

        Return_Type CancelOrderById(int id){
            return this->CancelOrder(Order(id));
        }

        Return_Type GetFirst(Order **pOrder){
            Return_Type ret = RET_BOOK_EMPTY;

            this->book_lock.lock();
            if(!this->book.empty())
            {
                *pOrder = &(this->book.begin()->second.front());

                ret = RET_OK;
            }
            this->book_lock.unlock();

            return ret;
        }

        Return_Type L2Book_OrderPatialyFilled(Order *pOrder, float quantity)
        {
            Return_Type ret = RET_NOT_OK;

            if( (!this->book[pOrder->price].empty()) && (this->book_l2[pOrder->price] >= quantity) )
            {
                this->book_l2[pOrder->price] -= quantity;
                ret = RET_OK;
            }
            
            return ret;
        }

        Return_Type GetL2Book(map<string, float, Comparator > **book_l2)
        {
            Return_Type ret = RET_BOOK_EMPTY;

            this->book_lock.lock();
            if(!this->book_l2.empty())
            {
                *book_l2 = &(this->book_l2);

                ret = RET_OK;
            }
            this->book_lock.unlock();

            return ret;
        }

        void PrintBook(){
            this->book_lock.lock();
            for(auto price_level : this->book){
                for(auto order : price_level.second){
                    order.PrintOrder();
                }
            }
            this->book_lock.unlock();
        }
};

#endif