#ifndef BOOK_H
#define BOOK_H

#include <map>
#include <list>
#include <unordered_map>
#include <algorithm>
#include <mutex>

#include "return_type.h"
#include "order.h"



template <typename Comparator> class Book{
    private:
        std::map<std::string, std::list<Order>, Comparator > book;
        std::unordered_map<Order, bool, Order::HashFunc> orders;

        mutable std::mutex book_mtx;
        mutable std::mutex orders_mtx;
    
    public:
        Book(){}

        ~Book(){}

        bool ExistsOrder(const Order& order) const{
            std::lock_guard<std::mutex> orders_lock(this->orders_mtx);

            return this->orders.find(order) != this->orders.end();
        }

        bool ExistsOrderId(const int id) const{
            return this->ExistsOrder(Order(id));
        }

        returnType AddOrder(const Order& order){
            returnType ret = RET_ORDER_EXISTS;

            std::lock_guard<std::mutex> orders_lock(this->orders_mtx);
            if(this->orders.find(order) == this->orders.end()){
                this->orders[order] = false;

                std::lock_guard<std::mutex> book_lock(this->book_mtx);
                this->book[order.price].push_back(order);

                ret = RET_OK;
            }

            return ret;
        }

        returnType CancelOrderById(const int id, Order *pOrder){
            returnType ret = RET_ORDER_NOT_EXISTS;

            std::lock_guard<std::mutex> orders_lock(this->orders_mtx);

            auto itter = this->orders.find(Order(id));
            if(itter != this->orders.end())
            {
                if(itter->second)
                {
                    return RET_ORDER_IN_USE;
                }

                const Order& order = itter->first;

                if(pOrder != nullptr)
                {
                    *pOrder = order;
                }
                
                std::unique_lock<std::mutex> book_lock(this->book_mtx);
                auto pBookOrder = find(this->book[order.price].begin(), this->book[order.price].end(), order);
                if(pBookOrder != this->book[order.price].end()){
                    this->book[order.price].erase(pBookOrder);

                    if(this->book[order.price].empty()){
                        this->book.erase(order.price);
                    }
                }
                book_lock.unlock();

                this->orders.erase(order);

                ret = RET_OK;
            }

            return ret;
        }

        returnType CancelOrder(const Order& order, Order *pOrder){
            return this->CancelOrderById(order.id, pOrder);
        }

        returnType GetFirst(Order **pOrder){
            returnType ret = RET_BOOK_EMPTY;

            std::lock_guard<std::mutex> orders_lock(this->orders_mtx);//avoid deadlock
            std::lock_guard<std::mutex> book_lock(this->book_mtx);
            if(!this->book.empty())
            {
                Order& order = this->book.begin()->second.front();
                *pOrder = &order;

                this->orders[order] = true;

                ret = RET_OK;
            }

            return ret;
        }

        returnType ReleaseOrder(Order& order)
        {
            returnType ret = RET_ORDER_NOT_EXISTS;

            std::lock_guard<std::mutex> orders_lock(this->orders_mtx);

            auto itter = this->orders.find(order);
            if(itter != this->orders.end()){
                itter->second = false;

                ret = RET_OK;
            }

            return ret;
        }
};

#endif