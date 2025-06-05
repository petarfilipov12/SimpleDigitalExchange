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

        ReturnType AddOrder(Order order){
            ReturnType ret = RET_ORDER_EXISTS;

            this->book_lock.lock();
            if(this->orders.find(order) == this->orders.end()){
                this->book[order.price].push_back(order);
                this->orders.insert(order);

                ret = RET_OK;
            }
            this->book_lock.unlock();

            return ret;
        }

        ReturnType CancelOrderById(int id, Order *pOrder_o){
            ReturnType ret = RET_ORDER_NOT_EXISTS;

            this->book_lock.lock();
            unordered_set<Order, Order::HashFunc>::iterator pOrder = this->orders.find(Order(id));

            if(pOrder != this->orders.end()){
                if(pOrder_o != nullptr)
                {
                    *pOrder_o = *pOrder;
                }
                
                list<Order>::iterator pBookOrder = find(this->book[pOrder->price].begin(), this->book[pOrder->price].end(), *pOrder);

                if(pBookOrder != this->book[pOrder->price].end()){
                    this->book[pOrder->price].erase(pBookOrder);

                    if(this->book[pOrder->price].empty()){
                        this->book.erase(pOrder->price);
                    }
                }

                this->orders.erase(pOrder);

                ret = RET_OK;
            }
            this->book_lock.unlock();

            return ret;
        }

        ReturnType CancelOrder(Order order, Order *pOrder){
            return this->CancelOrderById(order.id, pOrder);
        }

        ReturnType GetFirst(Order **pOrder){
            ReturnType ret = RET_BOOK_EMPTY;

            this->book_lock.lock();
            if(!this->book.empty())
            {
                *pOrder = &(this->book.begin()->second.front());

                ret = RET_OK;
            }
            this->book_lock.unlock();

            return ret;
        }
};

#endif