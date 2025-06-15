#include "taker_order_book.h"

#include <algorithm>
#include <iterator>

TakerOrderBook::TakerOrderBook() {}
TakerOrderBook::~TakerOrderBook() {}

bool TakerOrderBook::ExistsTakerOrder(const Order& order) const
{
    this->taker_book_lock.lock();
    bool ret = this->taker_orders.find(order) != this->taker_orders.end();
    this->taker_book_lock.unlock();

    return ret;
}

bool TakerOrderBook::ExistsTakerOrderById(const int id) const
{
    return this->ExistsTakerOrder(Order(id));
}

returnType TakerOrderBook::AddTakerOrder(const Order& order)
{
    returnType ret = RET_ORDER_NOT_EXISTS;

    this->taker_book_lock.lock();
    if (this->taker_orders.find(order) == this->taker_orders.end())
    {
        this->taker_orders_queue.push_back(order);
        this->taker_orders.insert(order);

        ret = RET_OK;
    }
    this->taker_book_lock.unlock();

    return ret;
}

returnType TakerOrderBook::CancelTakerOrderById(const int id, Order *pOrder_o)
{
    returnType ret = RET_ORDER_NOT_EXISTS;

    this->taker_book_lock.lock();
    std::unordered_set<Order, Order::HashFunc>::iterator pOrder = this->taker_orders.find(Order(id));

    if (pOrder != this->taker_orders.end())
    {
        if(pOrder_o != nullptr)
        {
            *pOrder_o = *pOrder;
        }
        std::list<Order>::iterator pListOrder = find(this->taker_orders_queue.begin(), this->taker_orders_queue.end(), *pOrder);

        if (pListOrder != this->taker_orders_queue.end())
        {
            this->taker_orders_queue.erase(pListOrder);
        }

        this->taker_orders.erase(pOrder);

        ret = RET_OK;
    }
    this->taker_book_lock.unlock();

    return ret;
}

returnType TakerOrderBook::CancelTakerOrder(const Order& order, Order *pOrder)
{
    return this->CancelTakerOrderById(order.id, pOrder);
}

returnType TakerOrderBook::GetAt(const int index, Order **pOrder)
{
    returnType ret = RET_BOOK_EMPTY;
    std::list<Order>::iterator iter;

    this->taker_book_lock.lock();
    if (!this->taker_orders_queue.empty())
    {
        if(index < this->taker_orders_queue.size())
        {
            
            iter = this->taker_orders_queue.begin();
            advance(iter, index);

            *pOrder = &(*iter);

            ret = RET_OK;
        }
        else
        {
            ret = RET_INVALID;
        }
    }
    this->taker_book_lock.unlock();

    return ret;
}

returnType TakerOrderBook::GetFirst(Order **pOrder)
{
    returnType ret = RET_BOOK_EMPTY;

    this->taker_book_lock.lock();
    if (!this->taker_orders_queue.empty())
    {
        *pOrder = &(this->taker_orders_queue.front());

        ret = RET_OK;
    }
    this->taker_book_lock.unlock();

    return ret;
}

returnType TakerOrderBook::PopFirst()
{
    returnType ret = RET_BOOK_EMPTY;

    this->taker_book_lock.lock();
    if (!this->taker_orders_queue.empty())
    {
        this->taker_orders.erase(this->taker_orders_queue.front());
        this->taker_orders_queue.pop_front();

        ret = RET_OK;
    }
    this->taker_book_lock.unlock();

    return ret;
}