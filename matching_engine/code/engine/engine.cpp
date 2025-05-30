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
    json j_data = order.ConvertOrderToJson();
    Event event(EVENT_ID_ORDER_ADDED, j_data, nullptr);

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

    if(RET_OK != ret)
    {
        event = Event(EVENT_ID_ADD_ORDER_FAILLED, j_data, nullptr);
    }
    
    this->event_bus->Send(event);

    return ret;
}

Return_Type Engine::CancelOrder(Order order)
{
    Event event(EVENT_ID_CANCEL_ORDER_FAILED, order.ConvertOrderToJson(), nullptr);
    Order *pOrder;
    Return_Type ret = RET_NOT_OK;

    if (order.order_type == ORDER_TYPE_MARKET)
    {
        ret = this->market_book.CancelMarketOrder(order, pOrder);
    }
    else if (order.order_type == ORDER_TYPE_LIMIT)
    {
        ret = this->order_book.CancelOrder(order, pOrder);
    }
    else
    {
        // Error
    }

    if(RET_OK == ret)
    {
        event = Event(EVENT_ID_ORDER_CANCELED, pOrder->ConvertOrderToJson(), nullptr);
    }
        
    this->event_bus->Send(event);

    return ret;
}

Return_Type Engine::CancelOrderById(int id)
{
    Event event(EVENT_ID_CANCEL_ORDER_FAILED, {{"order_id", id}}, nullptr);
    Order *pOrder;
    Return_Type ret = RET_NOT_OK;

    if (this->market_book.ExistsMarketOrderById(id))
    {
        ret = this->market_book.CancelMarketOrderById(id, pOrder);
    }
    else
    {
        ret = this->order_book.CancelOrderById(id, pOrder);
    }

    if(RET_OK == ret)
    {
        event = Event(EVENT_ID_ORDER_CANCELED, pOrder->ConvertOrderToJson(), nullptr);
    }

    this->event_bus->Send(event);

    return ret;
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
            j_data["bid_order"] = bid_order->ConvertOrderToJson();
            j_data["ask_order"] = ask_order->ConvertOrderToJson();

            ask_order->filled += quantity;
            if(ask_order->filled >= ask_order->quantity)
            {
                this->order_book.CancelOrderById(ask_order->id, nullptr);
            }

            bid_order->filled += quantity;
            if(bid_order->filled >= bid_order->quantity)
            {
                this->order_book.CancelOrderById(bid_order->id, nullptr);
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

        j_data["ask_order"] = book_order->ConvertOrderToJson();
        j_data["bid_order"] = market_order->ConvertOrderToJson();
    }
    else if (market_order->order_side == ORDER_SIDE_SELL)
    {
        if (RET_OK != this->order_book.GetBidFirst(&book_order))
        {
            return;
        }

        j_data["ask_order"] = market_order->ConvertOrderToJson();
        j_data["bid_order"] = book_order->ConvertOrderToJson();
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
        this->order_book.CancelOrderById(book_order->id, nullptr);
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