#include "taker_order_book.h"

#include <algorithm>
#include <iterator>

TakerOrderBook::TakerOrderBook() {}
TakerOrderBook::~TakerOrderBook() {}

void TakerOrderBook::AddOrderToBook(const Order &order)
{
    this->book.push_back(order);
}

void TakerOrderBook::EraseOrderFromBook(const Order &order)
{
    auto pListOrder = find(this->book.begin(), this->book.end(), order);
    if (pListOrder != this->book.end())
    {
        this->book.erase(pListOrder);
    }
}

Order &TakerOrderBook::GetFirstBookOrder()
{
    return this->book.front();
}

returnType TakerOrderBook::GetAt(const int index, Order **pOrder)
{
    returnType ret = RET_BOOK_EMPTY;
    std::list<Order>::iterator iter;

    std::lock_guard<std::mutex> orders_lock(this->orders_mtx); // avoid deadlock
    std::lock_guard<std::mutex> book_lock(this->book_mtx);
    if (!this->book.empty())
    {
        if (index < this->book.size())
        {
            iter = this->book.begin();
            advance(iter, index);
            Order &order = *iter;

            *pOrder = &order;

            this->orders[order] = true;

            ret = RET_OK;
        }
        else
        {
            ret = RET_INVALID;
        }
    }

    return ret;
}

returnType TakerOrderBook::PopFirst()
{
    returnType ret = RET_BOOK_EMPTY;

    std::lock_guard<std::mutex> orders_lock(this->orders_mtx); // avoid deadlock
    std::lock_guard<std::mutex> book_lock(this->book_mtx);
    if (!this->book.empty())
    {
        this->orders.erase(this->book.front());
        this->book.pop_front();

        ret = RET_OK;
    }

    return ret;
}