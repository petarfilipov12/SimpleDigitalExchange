#include "engine.h"

#include <thread>

Engine::Engine(const std::string &symbol, EventBus &event_bus, receiverId_t receiver_id) : event_bus(event_bus), symbol(symbol)
{
    this->taker_order_ptr = nullptr;
    this->book_order_ptr = nullptr;

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
    bool flag = false;

    //Canceling book order is more likely, so check this first
    std::shared_lock<std::shared_mutex> book_order_ptr_lock(this->book_order_ptr_mtx);
    if ((this->book_order_ptr != nullptr) && (order == *(this->book_order_ptr)))
    {
        book_order_ptr_lock.unlock();
        flag = true;
    }
    else
    {
        ret = this->order_book.CancelOrder(order, pOrder);
        book_order_ptr_lock.unlock();

        if (RET_OK == ret)
        {
            event_id = EVENT_ID_MAKER_ORDER_CANCELED;
            j_data = pOrder->ConvertOrderToJson();
            flag = true;
        }
    }

    if ((!flag) && (RET_ORDER_NOT_EXISTS == ret))
    {
        std::shared_lock<std::shared_mutex> taker_order_ptr_lock(this->taker_order_ptr_mtx);
        if ((this->taker_order_ptr == nullptr) || (order != *(this->taker_order_ptr)))
        {
            ret = this->taker_book.CancelTakerOrder(order, pOrder);
            taker_order_ptr_lock.unlock();

            if (RET_OK == ret)
            {
                event_id = EVENT_ID_MAKER_ORDER_CANCELED;
                j_data = pOrder->ConvertOrderToJson();
            }
        }
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
    json j_data;
    float quantity;
    float bookOrder_quantity;

    std::unique_lock<std::shared_mutex> book_order_ptr_lock(this->book_order_ptr_mtx, std::defer_lock);

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
        book_order_ptr_lock.lock();
        ret = this->order_book.GetAskFirst(&(this->book_order_ptr));
        if (RET_BOOK_EMPTY == ret)
        {
            this->book_order_ptr = nullptr;
            book_order_ptr_lock.unlock();

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
            this->book_order_ptr = nullptr;
            book_order_ptr_lock.unlock();

            return RET_NOT_OK;
        }

        if ((ORDER_TYPE_LIMIT == pTakerOrder.order_type) && (stof(pTakerOrder.price) < stof(this->book_order_ptr->price)))
        {
            this->book_order_ptr = nullptr;
            book_order_ptr_lock.unlock();

            this->AddToOrderBook(pTakerOrder);
            return RET_TAKER_ORDER_ADDED_TO_BOOK;
        }
    }
    else if (pTakerOrder.order_side == ORDER_SIDE_SELL)
    {
        book_order_ptr_lock.lock();
        ret = this->order_book.GetBidFirst(&(this->book_order_ptr));
        if (RET_BOOK_EMPTY == ret)
        {
            this->book_order_ptr = nullptr;
            book_order_ptr_lock.unlock();

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
            this->book_order_ptr = nullptr;
            book_order_ptr_lock.unlock();

            return RET_NOT_OK;
        }

        if ((ORDER_TYPE_LIMIT == pTakerOrder.order_type) && (stof(pTakerOrder.price) > stof(this->book_order_ptr->price)))
        {
            this->book_order_ptr = nullptr;
            book_order_ptr_lock.unlock();

            this->AddToOrderBook(pTakerOrder);
            return RET_TAKER_ORDER_ADDED_TO_BOOK;
        }
    }
    else
    {
        return RET_NOT_OK;
    }

    quantity = (pTakerOrder.quantity - pTakerOrder.filled);

    bookOrder_quantity = (this->book_order_ptr->quantity - this->book_order_ptr->filled);
    if (bookOrder_quantity < quantity)
    {
        quantity = bookOrder_quantity;
    }

    j_data["symbol"] = this->symbol;
    j_data["taker_order"] = pTakerOrder.ConvertOrderToJson();
    j_data["book_order"] = this->book_order_ptr->ConvertOrderToJson();
    j_data["price"] = this->book_order_ptr->price;
    j_data["quantity"] = quantity;

    this->book_order_ptr->filled += quantity;
    if (this->book_order_ptr->filled >= this->book_order_ptr->quantity)
    {
        this->order_book.CancelOrderById(this->book_order_ptr->id, nullptr);
    }
    this->book_order_ptr = nullptr;
    book_order_ptr_lock.unlock();

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
    returnType ret = RET_NOT_OK;
    int i = 1;

    std::unique_lock<std::shared_mutex> taker_order_ptr_lock(this->taker_order_ptr_mtx);
    if (RET_OK != this->taker_book.GetFirst(&(this->taker_order_ptr)))
    {
        this->taker_order_ptr = nullptr;
        return;
    }

    ret = this->MatchTakerOrder(*(this->taker_order_ptr));
    this->taker_order_ptr = nullptr;
    taker_order_ptr_lock.unlock();

    if ( (RET_TAKER_ORDER_ADDED_TO_BOOK == ret) || (RET_TAKER_ORDER_FILLED == ret) )
    {
        this->taker_book.PopFirst();
    }
    else if (RET_BOOK_EMPTY == ret)
    {
        taker_order_ptr_lock.lock();
        for (i = 1; RET_BOOK_EMPTY == ret; i++)
        {
            if (RET_OK != this->taker_book.GetAt(i, &(this->taker_order_ptr)))
            {
                this->taker_order_ptr = nullptr;
                return;
            }

            if( (RET_TAKER_ORDER_ADDED_TO_BOOK == ret) || (RET_TAKER_ORDER_FILLED == ret) )
            {
                this->taker_book.CancelTakerOrderById(this->taker_order_ptr->id, nullptr);
            }
        }

        this->taker_order_ptr = nullptr;
        taker_order_ptr_lock.unlock();
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