#include "engine.h"

#include <thread>

Engine::Engine(const std::string &symbol, EventBus &event_bus, receiverId_t receiver_id) : event_bus(event_bus), symbol(symbol)
{
    std::thread thread_engine([this]
                              { this->run(); });
    thread_engine.detach();

    this->event_receiver = EventReceiver(
        receiver_id,
        std::bind(&Engine::EventHandler, this, std::placeholders::_1),
        std::bind(&Engine::Filter, this, std::placeholders::_1));

    this->event_bus.AddReceiver(&this->event_receiver);

    this->event_bus.Subscribe(receiver_id, EVENT_ID_ADD_ORDER);
    this->event_bus.Subscribe(receiver_id, EVENT_ID_CANCEL_ORDER);
}

Engine::~Engine() {}

bool Engine::ExistsOrder(const Order &order) const
{
    bool ret = this->taker_book.ExistsTakerOrder(order);

    if (!ret)
    {
        ret = this->order_book.ExistsOrder(order);
    }

    return ret;
}

bool Engine::ExistsOrderId(const int id) const
{
    return this->ExistsOrder(Order(id));
}

returnType Engine::AddOrder(Order order)
{
    eventId_t event_id = EVENT_ID_ADD_ORDER_FAILLED;
    returnType ret = RET_NOT_OK;

    ret = this->taker_book.AddTakerOrder(order);

    if (RET_OK == ret)
    {
        order.SetCurrentTimestamp();
        event_id = EVENT_ID_TAKER_ORDER_ADDED;
    }

    this->event_bus.Send(Event(event_id, order.ConvertOrderToJson(), nullptr));

    return ret;
}

returnType Engine::CancelOrder(const Order &order)
{
    eventId_t event_id = EVENT_ID_CANCEL_ORDER_FAILED;
    json j_data = order.ConvertOrderToJson();
    returnType ret = RET_NOT_OK;
    Order *pOrder;
    
    ret = this->taker_book.CancelTakerOrder(order, pOrder);

    if(RET_OK == ret)
    {
        event_id = EVENT_ID_TAKER_ORDER_CANCELED;
        j_data = pOrder->ConvertOrderToJson();
    }
    else if(RET_ORDER_NOT_EXISTS == ret)
    {
        ret = this->order_book.CancelOrder(order, pOrder);

        if(RET_OK == ret)
        {
            event_id = EVENT_ID_MAKER_ORDER_CANCELED;
            j_data = pOrder->ConvertOrderToJson();
        }
    }
    else
    {
        //Nothing to do
    }
        
    this->event_bus.Send(Event(event_id, j_data, nullptr));

    return ret;
}

returnType Engine::CancelOrderById(const int id)
{
    return this->CancelOrder(Order(id));
}

returnType Engine::AddToOrderBook(Order &pTakerOrder)
{
    returnType ret = this->order_book.AddOrder(pTakerOrder);

    if (RET_OK == ret)
    {
        this->event_bus.Send(Event(EVENT_ID_MAKER_ORDER_ADDED, pTakerOrder.ConvertOrderToJson(), nullptr));
    }

    return ret;
}

returnType Engine::MatchTakerOrder(Order &pTakerOrder)
{
    returnType ret = RET_NOT_OK;
    Order *bookOrder;
    std::function<returnType(Order&)> release_func;
    json j_data;
    float quantity;
    float bookOrder_quantity;

    bool diff;
    int book_order_id;

    if ((ORDER_TYPE_MARKET != pTakerOrder.order_type) && (ORDER_TYPE_LIMIT != pTakerOrder.order_type))
    {
        return RET_NOT_OK;
    }

    if (pTakerOrder.filled >= pTakerOrder.quantity)
    {
        return RET_TAKER_ORDER_FILLED;
    }

    if (pTakerOrder.order_side == ORDER_SIDE_BUY)
    {
        ret = this->order_book.GetAskFirst(&bookOrder);
        if (RET_BOOK_EMPTY == ret)
        {
            if (ORDER_TYPE_LIMIT == pTakerOrder.order_type)
            {
                this->AddToOrderBook(pTakerOrder);
                return RET_TAKER_ORDER_ADDED_TO_BOOK;
            }
            else // Market order
            {
                return RET_BOOK_EMPTY;
            }
        }
        else if (RET_OK != ret)
        {
            return RET_NOT_OK;
        }

        release_func = [this](Order& order) -> returnType{return this->order_book.ReleaseAskOrder(order);};

        if ((ORDER_TYPE_LIMIT == pTakerOrder.order_type) && (stof(pTakerOrder.price) < stof(bookOrder->price)))
        {
            this->AddToOrderBook(pTakerOrder);
            release_func(*bookOrder);
            return RET_TAKER_ORDER_ADDED_TO_BOOK;
        }
    }
    else if (pTakerOrder.order_side == ORDER_SIDE_SELL)
    {
        ret = this->order_book.GetBidFirst(&(bookOrder));
        if (RET_BOOK_EMPTY == ret)
        {

            if (ORDER_TYPE_LIMIT == pTakerOrder.order_type)
            {
                this->AddToOrderBook(pTakerOrder);
                return RET_TAKER_ORDER_ADDED_TO_BOOK;
            }
            else // Market order
            {
                return RET_BOOK_EMPTY;
            }
        }
        else if (RET_OK != ret)
        {
            return RET_NOT_OK;
        }

        release_func = [this](Order& order) -> returnType{return this->order_book.ReleaseBidOrder(order);};

        if ((ORDER_TYPE_LIMIT == pTakerOrder.order_type) && (stof(pTakerOrder.price) > stof(bookOrder->price)))
        {
            this->AddToOrderBook(pTakerOrder);
            release_func(*bookOrder);
            return RET_TAKER_ORDER_ADDED_TO_BOOK;
        }
    }
    else
    {
        return RET_NOT_OK;
    }

    quantity = (pTakerOrder.quantity - pTakerOrder.filled);

    bookOrder_quantity = (bookOrder->quantity - bookOrder->filled);
    if (bookOrder_quantity < quantity)
    {
        quantity = bookOrder_quantity;
    }

    j_data["symbol"] = this->symbol;
    j_data["taker_order"] = pTakerOrder.ConvertOrderToJson();
    j_data["book_order"] = bookOrder->ConvertOrderToJson();
    j_data["price"] = bookOrder->price;
    j_data["quantity"] = quantity;

    bookOrder->filled += quantity;

    diff = bookOrder->filled >= bookOrder->quantity;
    book_order_id = bookOrder->id;

    release_func(*bookOrder);

    if (diff)
    {
        this->order_book.CancelOrderById(book_order_id, nullptr);
    }

    this->event_bus.Send(Event(EVENT_ID_ORDER_FILLED, j_data, nullptr));

    pTakerOrder.filled += quantity;
    j_data = {};

    if (pTakerOrder.filled >= pTakerOrder.quantity)
    {
        return RET_TAKER_ORDER_FILLED;
    }

    return RET_OK;
}

void Engine::Cyclic()
{
    Order *takerOrder;
    int takerOrder_id;
    returnType ret = RET_NOT_OK;
    int i = 1;

    if (RET_OK != this->taker_book.GetFirst(&takerOrder))
    {
        return;
    }

    ret = this->MatchTakerOrder(*takerOrder);
    this->taker_book.ReleaseTakerOrder(*takerOrder);

    if ( (RET_TAKER_ORDER_ADDED_TO_BOOK == ret) || (RET_TAKER_ORDER_FILLED == ret) )
    {
        this->taker_book.PopFirst();
    }
    else if (RET_BOOK_EMPTY == ret)
    {
        for (i = 1; RET_BOOK_EMPTY == ret; i++)
        {
            if (RET_OK != this->taker_book.GetAt(i, &takerOrder))
            {
                return;
            }

            takerOrder_id = takerOrder->id;
            this->taker_book.ReleaseTakerOrder(*takerOrder);

            if( (RET_TAKER_ORDER_ADDED_TO_BOOK == ret) || (RET_TAKER_ORDER_FILLED == ret) )
            {
                this->taker_book.CancelTakerOrderById(takerOrder_id, nullptr);
            }
        }
    }
}

void Engine::run()
{
    while (true)
    {
        this->Cyclic();
    }
}

/******************************/
/*Event_Handler Implementation*/
/******************************/
void Engine::EventHandler_AddOrder(Event &event)
{
    returnType ret = RET_NOT_OK;
    json j_data = event.GetDataIn();

    ret = this->AddOrder(Order::ConvertJsonToOrder(j_data));

    if (nullptr != event.GetDataOut())
    {
        (*event.GetDataOut())["data"] = j_data;
        (*event.GetDataOut())["error"] = ret;
    }
}

void Engine::EventHandler_CancelOrder(Event &event)
{
    returnType ret;

    ret = this->CancelOrderById(event.GetDataIn()["order_id"]);

    if (nullptr != event.GetDataOut())
    {
        (*event.GetDataOut())["data"] = event.GetDataIn();
        (*event.GetDataOut())["error"] = ret;
    }
}

void Engine::EventHandler(Event event)
{
    switch (event.GetEventId())
    {
    case EVENT_ID_ADD_ORDER:
        this->EventHandler_AddOrder(event);
        break;
    case EVENT_ID_CANCEL_ORDER:
        this->EventHandler_CancelOrder(event);
        break;
    default:
        break;
    }
}

/***********************/
/*Filter Implementation*/
/***********************/
returnType Engine::Filter(Event &event)
{
    returnType ret = RET_NOT_OK;

    if (this->symbol == event.GetDataIn()["symbol"])
    {
        ret = RET_OK;
    }

    return ret;
}