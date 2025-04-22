#include "order_book.h"

#include <iostream>

using namespace std;

OrderBook::OrderBook() {}

OrderBook::~OrderBook()
{
    // this->bid_book.~Book();
    // this->ask_book.~Book();
}

bool OrderBook::ExistsOrder(Order order) const
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

bool OrderBook::ExistsOrderId(int id) const
{
    Order order(id);
    bool ret = this->bid_book.ExistsOrder(order);

    if (!ret)
    {
        ret = this->ask_book.ExistsOrder(order);
    }

    return ret;
}

Return_Type OrderBook::AddOrder(Order order)
{
    Return_Type ret = RET_NOT_OK;

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

Return_Type OrderBook::CancelOrder(Order order)
{
    Return_Type ret = RET_ORDER_NOT_EXISTS;

    if ((RET_OK == this->bid_book.CancelOrder(order)) ||
        (RET_OK == this->ask_book.CancelOrder(order)))
    {
        ret = RET_OK;
    }

    return ret;
}

Return_Type OrderBook::CancelOrderById(int id)
{
    return this->CancelOrder(Order(id));
}

Return_Type OrderBook::GetBidFirst(Order **pOrder)
{
    return this->bid_book.GetFirst(pOrder);
}

Return_Type OrderBook::GetAskFirst(Order **pOrder)
{
    return this->ask_book.GetFirst(pOrder);
}

Return_Type OrderBook::L2Book_OrderPatialyFilled(Order *pOrder, float quantity)
{
    Return_Type ret = RET_NOT_OK;

    if (pOrder->order_side == ORDER_SIDE_BUY)
    {
        ret = this->bid_book.L2Book_OrderPatialyFilled(pOrder, quantity);
    }
    else if (pOrder->order_side == ORDER_SIDE_SELL)
    {
        ret = this->ask_book.L2Book_OrderPatialyFilled(pOrder, quantity);
    }
    else
    {
        // Nothing to do
    }

    return ret;
}

Return_Type OrderBook::GetL2Book(json *l2_order_book)
{
    Return_Type ret;
    map<string, float, greater<string> > *bid_l2_book;
    map<string, float, less<string> > *ask_l2_book;

    (*l2_order_book)["ask"] = {};
    (*l2_order_book)["bid"] = {};

    ret = this->ask_book.GetL2Book(&ask_l2_book);
    if(RET_OK == ret)
    {
        (*l2_order_book)["ask"] = *ask_l2_book;
    }

    ret = this->bid_book.GetL2Book(&bid_l2_book);
    if(RET_OK == ret)
    {
        (*l2_order_book)["bid"] = *bid_l2_book;
    }

    return RET_OK;
}

void OrderBook::PrintOrderBook()
{
    cout << "bid_book:" << endl;
    this->bid_book.PrintBook();

    cout << "------------" << endl;

    cout << "ask_book:" << endl;
    this->ask_book.PrintBook();
}