#include "engine.h"

Engine::Engine() {}

Engine::~Engine()
{
    // this->market_orders_queue.clear();
    // this->market_orders.clear();

    // this->order_book.~OrderBook();
}

bool Engine::ExistsMarketOrder(Order order) const
{
    return this->market_orders.find(order) != this->market_orders.end();
}

bool Engine::ExistsOrder(Order order) const
{
    bool ret = false;

    if (order.order_type == ORDER_TYPE_MARKET)
    {
        ret = this->ExistsMarketOrder(order);
    }
    else if (order.order_type == ORDER_TYPE_LIMIT)
    {
        ret = this->order_book.ExistsOrder(order);
    }
    else
    {
        // Nothing to do
    }

    return ret;
}

bool Engine::ExistsOrderId(int id) const
{
    bool ret = this->ExistsMarketOrder(Order(id));

    if (!ret)
    {
        ret = this->order_book.ExistsOrderId(id);
    }

    return ret;
}

void Engine::AddMarketOrder(Order order)
{
    if (this->market_orders.find(order) == this->market_orders.end())
    {
        this->market_orders_queue.push_back(order);
        this->market_orders.insert(order);
    }
}

void Engine::AddOrder(Order order)
{
    if (order.order_type == ORDER_TYPE_MARKET)
    {
        this->AddMarketOrder(order);
    }
    else if (order.order_type == ORDER_TYPE_LIMIT)
    {
        this->order_book.AddOrder(order);
    }
    else
    {
        // Error
    }
}

void Engine::CancelMarketOrder(Order order)
{
    unordered_set<Order, Order::HashFunc>::iterator pOrder = this->market_orders.find(order);

    if (pOrder != this->market_orders.end())
    {
        list<Order>::iterator pListOrder = find(this->market_orders_queue.begin(), this->market_orders_queue.end(), *pOrder);

        if (pListOrder != this->market_orders_queue.end())
        {
            this->market_orders_queue.erase(pListOrder);
        }

        this->market_orders.erase(pOrder);
    }
}

void Engine::CancelOrder(Order order)
{
    if (order.order_type == ORDER_TYPE_MARKET)
    {
        this->CancelMarketOrder(order);
    }
    else if (order.order_type == ORDER_TYPE_LIMIT)
    {
        this->order_book.CancelOrder(order);
    }
    else
    {
        // Error
    }
}

void Engine::CancelOrderById(int id)
{
    Order order(id);

    if (this->ExistsMarketOrder(order))
    {
        this->CancelMarketOrder(order);
    }
    else
    {
        this->order_book.CancelOrder(order);
    }
}

OrderBook Engine::GetOrderBook() const
{
    return this->order_book;
}

void Engine::MatchOrderBook()
{
    Order bid_order = this->order_book.GetBidFirst();
    Order ask_order = this->order_book.GetAskFirst();

    if (stof(bid_order.price) >= stof(ask_order.price))
    {
        cout << "BOOK Order FILL: " << endl;
        bid_order.PrintOrder();
        ask_order.PrintOrder();

        this->order_book.CancelOrderById(bid_order.id);
        this->order_book.CancelOrderById(ask_order.id);
    }
}

void Engine::MatchMarketOrder()
{
    Order market_order = this->market_orders_queue.front();
    Order book_order;

    if (market_order.order_side == ORDER_SIDE_BUY)
    {
        book_order = this->order_book.GetAskFirst();
    }
    else if (market_order.order_side == ORDER_SIDE_SELL)
    {
        book_order = this->order_book.GetBidFirst();
    }
    else
    {
        this->market_orders_queue.pop_front();

        return;
    }

    cout << "Market Order FILL: " << endl;
    market_order.PrintOrder();
    book_order.PrintOrder();

    this->order_book.CancelOrderById(book_order.id);
    this->market_orders_queue.pop_front();
}

void Engine::Cyclic()
{
    this->MatchOrderBook();
    this->MatchMarketOrder();
}

void Engine::run()
{
    while (true)
    {
        this->Cyclic();
    }
}