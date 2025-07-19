#include "taker_order_book.h"

#include <algorithm>
#include <iterator>

TakerOrderBook::TakerOrderBook() {}
TakerOrderBook::~TakerOrderBook() {}

bool TakerOrderBook::ExistsTakerOrder(const Order &order) const
{
    std::lock_guard<std::mutex> taker_orders_lock(this->taker_orders_mtx);

    return this->taker_orders.find(order) != this->taker_orders.end();
}

bool TakerOrderBook::ExistsTakerOrderById(const int id) const
{
    return this->ExistsTakerOrder(Order(id));
}

returnType TakerOrderBook::AddTakerOrder(const Order &order)
{
    returnType ret = RET_ORDER_NOT_EXISTS;

    std::lock_guard<std::mutex> taker_orders_lock(this->taker_orders_mtx);
    if (this->taker_orders.find(order) == this->taker_orders.end())
    {
        this->taker_orders[order] = false;

        std::lock_guard<std::mutex> taker_orders_queue_lock(this->taker_orders_queue_mtx);
        this->taker_orders_queue.push_back(order);

        ret = RET_OK;
    }

    return ret;
}

returnType TakerOrderBook::CancelTakerOrderById(const int id, Order *pOrder)
{
    returnType ret = RET_ORDER_NOT_EXISTS;

    std::lock_guard<std::mutex> taker_orders_lock(this->taker_orders_mtx);

    auto itter = this->taker_orders.find(Order(id));
    if (itter != this->taker_orders.end())
    {
        if (itter->second)
        {
            return RET_ORDER_IN_USE;
        }

        const Order &order = itter->first;
        if (pOrder != nullptr)
        {
            *pOrder = order;
        }

        std::unique_lock<std::mutex> taker_orders_queue_lock(this->taker_orders_queue_mtx);
        auto pListOrder = find(this->taker_orders_queue.begin(), this->taker_orders_queue.end(), order);
        if (pListOrder != this->taker_orders_queue.end())
        {
            this->taker_orders_queue.erase(pListOrder);
        }
        taker_orders_queue_lock.unlock();

        this->taker_orders.erase(order);

        ret = RET_OK;
    }

    return ret;
}

returnType TakerOrderBook::CancelTakerOrder(const Order &order, Order *pOrder)
{
    return this->CancelTakerOrderById(order.id, pOrder);
}

returnType TakerOrderBook::GetAt(const int index, Order **pOrder)
{
    returnType ret = RET_BOOK_EMPTY;
    std::list<Order>::iterator iter;

    std::lock_guard<std::mutex> taker_orders_lock(this->taker_orders_mtx); // avoid deadlock
    std::lock_guard<std::mutex> taker_orders_queue_lock(this->taker_orders_queue_mtx);
    if (!this->taker_orders_queue.empty())
    {
        if (index < this->taker_orders_queue.size())
        {
            iter = this->taker_orders_queue.begin();
            advance(iter, index);
            Order &order = *iter;

            *pOrder = &order;

            this->taker_orders[order] = true;

            ret = RET_OK;
        }
        else
        {
            ret = RET_INVALID;
        }
    }

    return ret;
}

returnType TakerOrderBook::GetFirst(Order **pOrder)
{
    returnType ret = RET_BOOK_EMPTY;

    std::lock_guard<std::mutex> taker_orders_lock(this->taker_orders_mtx); // avoid deadlock
    std::lock_guard<std::mutex> taker_orders_queue_lock(this->taker_orders_queue_mtx);
    if (!this->taker_orders_queue.empty())
    {
        Order &order = this->taker_orders_queue.front();
        *pOrder = &order;

        this->taker_orders[order] = true;

        ret = RET_OK;
    }

    return ret;
}

returnType TakerOrderBook::PopFirst()
{
    returnType ret = RET_BOOK_EMPTY;

    std::lock_guard<std::mutex> taker_orders_lock(this->taker_orders_mtx); // avoid deadlock
    std::lock_guard<std::mutex> taker_orders_queue_lock(this->taker_orders_queue_mtx);
    if (!this->taker_orders_queue.empty())
    {
        this->taker_orders.erase(this->taker_orders_queue.front());
        this->taker_orders_queue.pop_front();

        ret = RET_OK;
    }

    return ret;
}

returnType TakerOrderBook::ReleaseTakerOrder(Order &order)
{
    returnType ret = RET_ORDER_NOT_EXISTS;

    std::lock_guard<std::mutex> taker_orders_lock(this->taker_orders_mtx);

    auto itter = this->taker_orders.find(order);
    if (itter != this->taker_orders.end())
    {
        itter->second = false;

        ret = RET_OK;
    }

    return ret;
}