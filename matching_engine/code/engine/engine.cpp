#include "engine.h"

Engine::Engine(EventBus *event_bus)
{
    this->event_bus = event_bus;
}

Engine::~Engine()
{
    // this->market_orders_queue.clear();
    // this->market_orders.clear();

    // this->order_book.~OrderBook();
}

bool Engine::ExistsOrder(Order order) const
{
    bool ret = false;

    if (order.order_type == ORDER_TYPE_MARKET)
    {
        ret = this->market_book.ExistsMarketOrder(order);
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
    bool ret = this->market_book.ExistsMarketOrderById(id);

    if (!ret)
    {
        ret = this->order_book.ExistsOrderId(id);
    }

    return ret;
}

Return_Type Engine::AddOrder(Order order)
{
    Return_Type ret = RET_NOT_OK;
    json j_data;

    if (order.order_type == ORDER_TYPE_MARKET)
    {
        ret = this->market_book.AddMarketOrder(order);
    }
    else if (order.order_type == ORDER_TYPE_LIMIT)
    {
        ret = this->order_book.AddOrder(order);
    }
    else
    {
        // Error
    }

    j_data = order.ConvertOrderToJson();

    if(RET_OK != ret)
    {
        Event event(EVENT_ID_ADD_ORDER_FAILLED, j_data, nullptr);
        this->event_bus->Send(event);
    }
    else
    {
        Event event(EVENT_ID_ORDER_ADDED, j_data, nullptr);
        this->event_bus->Send(event);
    }

    return ret;
}

Return_Type Engine::CancelOrder(Order order)
{
    Return_Type ret = RET_NOT_OK;
    json j_data;

    if (order.order_type == ORDER_TYPE_MARKET)
    {
        ret = this->market_book.CancelMarketOrder(order);
    }
    else if (order.order_type == ORDER_TYPE_LIMIT)
    {
        ret = this->order_book.CancelOrder(order);
    }
    else
    {
        // Error
    }

    j_data = order.ConvertOrderToJson();
    if(RET_OK != ret)
    {
        Event event(EVENT_ID_CANCEL_ORDER_FAILED, j_data, nullptr);
        this->event_bus->Send(event);
    }
    else
    {
        Event event(EVENT_ID_ORDER_CANCELED, j_data, nullptr);
        this->event_bus->Send(event);
    }

    return ret;
}

Return_Type Engine::CancelOrderById(int id)
{
    Return_Type ret = RET_NOT_OK;
    json j_data;

    if (this->market_book.ExistsMarketOrderById(id))
    {
        ret = this->market_book.CancelMarketOrderById(id);
    }
    else
    {
        ret = this->order_book.CancelOrderById(id);
    }

    j_data = {{"order_id", id}};
    if(RET_OK != ret)
    {
        Event event(EVENT_ID_CANCEL_ORDER_FAILED, j_data, nullptr);
        this->event_bus->Send(event);
    }
    else
    {
        Event event(EVENT_ID_ORDER_CANCELED, j_data, nullptr);
        this->event_bus->Send(event);
    }

    return ret;
}

Return_Type Engine::GetOrderBook(json *l2_book)
{
    return this->order_book.GetL2Book(l2_book);
}

void Engine::PrintOrderBook()
{
    this->order_book.PrintOrderBook();
}

void Engine::MatchOrderBook()
{
    json j_data;
    Order *bid_order;
    Order *ask_order;
    float quantity;
    float bid_quantity;

    if (
        (RET_OK == this->order_book.GetBidFirst(&bid_order)) &&
        (RET_OK == this->order_book.GetAskFirst(&ask_order)))
    {
        if (stof(bid_order->price) >= stof(ask_order->price))
        {
            quantity = (ask_order->quantity - ask_order->filled);
            bid_quantity = (bid_order->quantity - bid_order->filled);
            if(bid_quantity < quantity)
            {
                quantity = bid_quantity;
            }

            j_data["price"] = ask_order->price;
            j_data["quantity"] = quantity;
            j_data["bid_order_id"] = bid_order->id;
            j_data["ask_order_id"] = ask_order->id;

            ask_order->filled += quantity;
            if(ask_order->filled >= ask_order->quantity)
            {
                this->order_book.CancelOrderById(ask_order->id);
            }
            else
            {
                this->order_book.L2Book_OrderPatialyFilled(ask_order, quantity);
            }

            bid_order->filled += quantity;
            if(bid_order->filled >= bid_order->quantity)
            {
                this->order_book.CancelOrderById(bid_order->id);
            }
            else
            {
                this->order_book.L2Book_OrderPatialyFilled(bid_order, quantity);
            }

            Event event(EVENT_ID_ORDER_FILLED, j_data, nullptr);
            this->event_bus->Send(event);
        }
    }
}

void Engine::MatchMarketOrder()
{
    Order *market_order;
    Order *book_order;
    json j_data;
    float quantity;
    float book_order_quantity;

    if (RET_OK != this->market_book.GetFirst(&market_order))
    {
        return;
    }

    if (market_order->order_side == ORDER_SIDE_BUY)
    {
        if (RET_OK != this->order_book.GetAskFirst(&book_order))
        {
            return;
        }

        j_data["ask_order_id"] = book_order->id;
        j_data["bid_order_id"] = market_order->id;
    }
    else if (market_order->order_side == ORDER_SIDE_SELL)
    {
        if (RET_OK != this->order_book.GetBidFirst(&book_order))
        {
            return;
        }

        j_data["ask_order_id"] = market_order->id;
        j_data["bid_order_id"] = book_order->id;
    }
    else
    {
        this->market_book.PopFirst();

        return;
    }
    quantity = (market_order->quantity - market_order->filled);
    book_order_quantity = (book_order->quantity - book_order->filled);
    if (book_order_quantity < quantity)
    {
        quantity = book_order_quantity;
    }

    j_data["price"] = book_order->price;
    j_data["quantity"] = quantity;

    book_order->filled += quantity;
    if (book_order->filled >= book_order->quantity)
    {
        this->order_book.CancelOrderById(book_order->id);
    }
    else
    {
        this->order_book.L2Book_OrderPatialyFilled(book_order, quantity);
    }

    market_order->filled += quantity;
    if (market_order->filled >= market_order->quantity)
    {
        this->market_book.PopFirst();
    }
    
    Event event(EVENT_ID_ORDER_FILLED, j_data, nullptr);
    this->event_bus->Send(event);
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