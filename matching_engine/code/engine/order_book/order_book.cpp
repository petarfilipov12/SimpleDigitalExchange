#include "order_book.h"

OrderBook::OrderBook() {}

OrderBook::~OrderBook()
{
    // this->bid_book.~Book();
    // this->ask_book.~Book();
}

bool OrderBook::ExistsOrder(const Order& order) const
{
    bool ret = false;

    if (order.order_side == ORDER_SIDE_BUY)
    {
        ret = this->bid_book.ExistsOrder(order);
    }
    else if (order.order_side == ORDER_SIDE_SELL)
    {
        ret = this->ask_book.ExistsOrder(order);
    }
    else
    {
    }

    return ret;
}

bool OrderBook::ExistsOrderId(const int id) const
{
    Order order(id);
    bool ret = this->bid_book.ExistsOrder(order);

    if (!ret)
    {
        ret = this->ask_book.ExistsOrder(order);
    }

    return ret;
}

returnType OrderBook::AddOrder(const Order& order)
{
    returnType ret = RET_NOT_OK;

    if (order.order_side == ORDER_SIDE_BUY)
    {
        ret = this->bid_book.AddOrder(order);
    }
    else if (order.order_side == ORDER_SIDE_SELL)
    {
        ret = this->ask_book.AddOrder(order);
    }
    else
    {
        // Nothing to do
    }

    return ret;
}

returnType OrderBook::CancelOrder(const Order& order, Order *pOrder)
{
    return this->CancelOrderById(order.id, pOrder);
}

returnType OrderBook::CancelOrderById(const int id, Order *pOrder)
{
    returnType ret = RET_ORDER_NOT_EXISTS;

    if ((RET_OK == this->bid_book.CancelOrderById(id, pOrder)) ||
        (RET_OK == this->ask_book.CancelOrderById(id, pOrder)))
    {
        ret = RET_OK;
    }

    return ret;
}

returnType OrderBook::GetBidFirst(Order **pOrder)
{
    return this->bid_book.GetFirst(pOrder);
}

returnType OrderBook::GetAskFirst(Order **pOrder)
{
    return this->ask_book.GetFirst(pOrder);
}
