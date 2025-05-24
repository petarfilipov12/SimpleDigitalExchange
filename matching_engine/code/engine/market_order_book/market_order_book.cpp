#include "market_order_book.h"

#include <algorithm>

MarketOrderBook::MarketOrderBook() {}
MarketOrderBook::~MarketOrderBook() {}

bool MarketOrderBook::ExistsMarketOrder(Order order) const
{
    this->market_book_lock.lock();
    bool ret = this->market_orders.find(order) != this->market_orders.end();
    this->market_book_lock.unlock();

    return ret;
}

bool MarketOrderBook::ExistsMarketOrderById(int id) const
{
    return this->ExistsMarketOrder(Order(id));
}

Return_Type MarketOrderBook::AddMarketOrder(Order order)
{
    Return_Type ret = RET_ORDER_EXISTS;

    this->market_book_lock.lock();
    if (this->market_orders.find(order) == this->market_orders.end())
    {
        this->market_orders_queue.push_back(order);
        this->market_orders.insert(order);

        ret = RET_OK;
    }
    this->market_book_lock.unlock();

    return ret;
}

Return_Type MarketOrderBook::CancelMarketOrderById(int id, Order *pOrder_o)
{
    Return_Type ret = RET_ORDER_NOT_EXISTS;

    this->market_book_lock.lock();
    unordered_set<Order, Order::HashFunc>::iterator pOrder = this->market_orders.find(Order(id));

    if (pOrder != this->market_orders.end())
    {
        if(pOrder_o != nullptr)
        {
            *pOrder_o = *pOrder;
        }
        list<Order>::iterator pListOrder = find(this->market_orders_queue.begin(), this->market_orders_queue.end(), *pOrder);

        if (pListOrder != this->market_orders_queue.end())
        {
            this->market_orders_queue.erase(pListOrder);
        }

        this->market_orders.erase(pOrder);

        ret = RET_OK;
    }
    this->market_book_lock.unlock();

    return ret;
}

Return_Type MarketOrderBook::CancelMarketOrder(Order order, Order *pOrder)
{
    return this->CancelMarketOrder(order.id, pOrder);
}

Return_Type MarketOrderBook::GetFirst(Order **pOrder)
{
    Return_Type ret = RET_BOOK_EMPTY;

    this->market_book_lock.lock();
    if (!this->market_orders_queue.empty())
    {
        *pOrder = &(this->market_orders_queue.front());

        ret = RET_OK;
    }
    this->market_book_lock.unlock();

    return ret;
}

Return_Type MarketOrderBook::PopFirst()
{
    Return_Type ret = RET_BOOK_EMPTY;

    this->market_book_lock.lock();
    if (!this->market_orders_queue.empty())
    {
        this->market_orders.erase(this->market_orders_queue.front());
        this->market_orders_queue.pop_front();

        ret = RET_OK;
    }
    this->market_book_lock.unlock();

    return ret;
}