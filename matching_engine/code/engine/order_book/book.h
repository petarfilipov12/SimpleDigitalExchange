#ifndef BOOK_H
#define BOOK_H

#include "return_type.h"

#include <map>
#include <list>

#include "base_book.h"

template <typename Comparator>
class Book : public BaseBook<std::map<std::string, std::list<Order>, Comparator>>
{
protected:
    void AddOrderToBook(const Order &order) override
    {
        this->book[order.price].push_back(order);
    }

    void EraseOrderFromBook(const Order &order) override
    {
        auto pBookOrder = find(this->book[order.price].begin(), this->book[order.price].end(), order);
        if (pBookOrder != this->book[order.price].end())
        {
            this->book[order.price].erase(pBookOrder);

            if (this->book[order.price].empty())
            {
                this->book.erase(order.price);
            }
        }
    }

    Order &GetFirstBookOrder() override
    {
        return this->book.begin()->second.front();
    }

public:
    Book() {}
    ~Book() {}
};

#endif