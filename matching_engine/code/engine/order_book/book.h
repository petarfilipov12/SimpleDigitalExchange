#ifndef BOOK_H
#define BOOK_H

#include <map>
#include <list>
#include <unordered_set>
#include <algorithm>
#include <mutex>

#include "return_type.h"
#include "order.h"



template <typename Comparator> class Book{
    private:
        std::map<std::string, std::list<Order>, Comparator > book;
        std::unordered_set<Order, Order::HashFunc> orders;

        mutable std::mutex book_lock;
    
    public:
        Book(){}

        ~Book(){
            // this->book.clear();
            // this->orders.clear();
        }

        bool ExistsOrder(const Order& order) const{
            this->book_lock.lock();
            bool ret = this->orders.find(order) != this->orders.end();
            this->book_lock.unlock();

            return ret;
        }

        bool ExistsOrderId(const int id) const{
            return this->ExistsOrder(Order(id));
        }

        returnType AddOrder(const Order& order){
            returnType ret = RET_ORDER_EXISTS;

            this->book_lock.lock();
            if(this->orders.find(order) == this->orders.end()){
                this->book[order.price].push_back(order);
                this->orders.insert(order);

                ret = RET_OK;
            }
            this->book_lock.unlock();

            return ret;
        }

        returnType CancelOrderById(const int id, Order *pOrder_o){
            returnType ret = RET_ORDER_NOT_EXISTS;

            this->book_lock.lock();
            std::unordered_set<Order, Order::HashFunc>::iterator pOrder = this->orders.find(Order(id));

            if(pOrder != this->orders.end()){
                if(pOrder_o != nullptr)
                {
                    *pOrder_o = *pOrder;
                }
                
                std::list<Order>::iterator pBookOrder = find(this->book[pOrder->price].begin(), this->book[pOrder->price].end(), *pOrder);

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

        returnType CancelOrder(const Order& order, Order *pOrder){
            return this->CancelOrderById(order.id, pOrder);
        }

        returnType GetFirst(Order **pOrder){
            returnType ret = RET_BOOK_EMPTY;

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