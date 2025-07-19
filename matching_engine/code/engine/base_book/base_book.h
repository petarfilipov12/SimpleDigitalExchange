#ifndef BASE_BOOK_H
#define BASE_BOOK_H

#include <unordered_map>
#include <algorithm>
#include <mutex>

#include "return_type.h"
#include "order.h"


template <typename T>
class BaseBook{
    protected:
        T book;
        std::unordered_map<Order, bool, Order::HashFunc> orders;

        mutable std::mutex book_mtx;
        mutable std::mutex orders_mtx;

        virtual void AddOrderToBook(const Order& order) = 0;
        virtual void EraseOrderFromBook(const Order& order) = 0;
        virtual Order& GetFirstBookOrder() = 0;
    
    public:
        BaseBook(){}

        ~BaseBook(){}

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
                this->AddOrderToBook(order);

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
                this->EraseOrderFromBook(order);
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
                Order& order = this->GetFirstBookOrder();
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