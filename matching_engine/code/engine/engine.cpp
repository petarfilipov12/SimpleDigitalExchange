#include "engine.h"

#include <thread>

Engine::Engine(EventBus& event_busP, const std::string& symbol): event_bus(event_busP)
{
    //this->event_bus = (EventBus *)event_bus;
    this->symbol = symbol;
}

Engine::~Engine()
{
    // this->taker_orders_queue.clear();
    // this->taker_orders.clear();

    // this->order_book.~OrderBook();
}

bool Engine::ExistsOrder(const Order& order) const
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

    if(RET_OK == ret)
    {
        order.SetCurrentTimestamp();
        event_id = EVENT_ID_TAKER_ORDER_ADDED;
    }
    
    this->event_bus.Send(Event(event_id, order.ConvertOrderToJson(), nullptr));

    return ret;
}

returnType Engine::CancelOrder(const Order& order)
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

returnType Engine::AddToOrderBook(Order& pTakerOrder)
{
    returnType ret = this->order_book.AddOrder(pTakerOrder);

    if(RET_OK == ret)
    {
        this->event_bus.Send(Event(EVENT_ID_MAKER_ORDER_ADDED, pTakerOrder.ConvertOrderToJson(), nullptr));
    }

    return ret;
}

returnType Engine::MatchTakerOrder(Order& pTakerOrder)
{
    returnType ret = RET_NOT_OK;
    Order *bookOrder;
    json j_data;
    float quantity;
    float bookOrder_quantity;

    if ((ORDER_TYPE_MARKET == pTakerOrder.order_type) || (ORDER_TYPE_LIMIT == pTakerOrder.order_type))
    {
        while (pTakerOrder.filled < pTakerOrder.quantity)
        {
            quantity = (pTakerOrder.quantity - pTakerOrder.filled);

            if (pTakerOrder.order_side == ORDER_SIDE_BUY)
            {
                ret = this->order_book.GetAskFirst(&bookOrder);
                if (RET_BOOK_EMPTY == ret)
                {
                    if(ORDER_TYPE_LIMIT == pTakerOrder.order_type)
                    {
                        this->AddToOrderBook(pTakerOrder);
                        break;
                    }
                    else //Market order
                    {
                        return RET_BOOK_EMPTY;
                    }
                }
                else if(RET_OK != ret)
                {
                    return RET_NOT_OK;
                }

                if ( (ORDER_TYPE_LIMIT == pTakerOrder.order_type) && (stof(pTakerOrder.price) < stof(bookOrder->price)) )
                {
                    this->AddToOrderBook(pTakerOrder);
                    break;
                }
            }
            else if (pTakerOrder.order_side == ORDER_SIDE_SELL)
            {
                ret = this->order_book.GetBidFirst(&bookOrder);
                if (RET_BOOK_EMPTY == ret)
                {
                    if(ORDER_TYPE_LIMIT == pTakerOrder.order_type)
                    {
                        this->AddToOrderBook(pTakerOrder);
                        break;
                    }
                    else //Market order
                    {
                        return RET_BOOK_EMPTY;
                    }
                }
                else if(RET_OK != ret)
                {
                    
                    return RET_NOT_OK;
                }

                if ( (ORDER_TYPE_LIMIT == pTakerOrder.order_type) && (stof(pTakerOrder.price) > stof(bookOrder->price)) )
                {
                    this->AddToOrderBook(pTakerOrder);
                    break;
                }
            }
            else
            {
                break;
            }

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
            if (bookOrder->filled >= bookOrder->quantity)
            {
                this->order_book.CancelOrderById(bookOrder->id, nullptr);
            }

            this->event_bus.Send(Event(EVENT_ID_ORDER_FILLED, j_data, nullptr));

            pTakerOrder.filled += quantity;
            j_data = {};
        }
    }

    return RET_OK;
}

void Engine::Cyclic()
{
    Order *takerOrder;
    returnType ret = RET_NOT_OK;
    int i = 1;

    if (RET_OK != this->taker_book.GetFirst(&takerOrder))
    {
        return;
    }

    ret = this->MatchTakerOrder(*takerOrder);

    if(RET_OK == ret)
    {
        this->taker_book.PopFirst();
    }
    else if(RET_BOOK_EMPTY == ret)
    {
        for(i=1; RET_BOOK_EMPTY == ret; i++)
        {
            if (RET_OK != this->taker_book.GetAt(i, &takerOrder))
            {
                return;
            }

            ret = this->MatchTakerOrder(*takerOrder);
        }

        if(RET_OK == ret)
        {
            this->taker_book.CancelTakerOrder(*takerOrder, nullptr);
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
/**************************/
/*Init Func implementation*/
/**************************/
void Engine::init(receiverId_t receiver_id)
{
    std::thread thread_engine([this]{this->run();});
    thread_engine.detach();

    EventReceiver event_receiver = EventReceiver(
        receiver_id, 
        std::bind(&Engine::EventHandler, this, std::placeholders::_1),
        std::bind(&Engine::Filter, this, std::placeholders::_1)
    );
    
    this->event_bus.AddReceiver(event_receiver);

    this->event_bus.Subscribe(receiver_id, EVENT_ID_ADD_ORDER);
    this->event_bus.Subscribe(receiver_id, EVENT_ID_CANCEL_ORDER);
}

/******************************/
/*Event_Handler Implementation*/
/******************************/
void Engine::EventHandler_AddOrder(Event& event)
{
    returnType ret = RET_NOT_OK;
    json j_data = event.GetJsonData();

    ret = this->AddOrder(Order::ConvertJsonToOrder(j_data));

    if(nullptr != event.GetResponceDataPtr())
    {
        (*event.GetResponceDataPtr())["data"] = j_data;
        (*event.GetResponceDataPtr())["error"] = ret;
    }

}

void Engine::EventHandler_CancelOrder(Event& event)
{
    returnType ret;

    ret = this->CancelOrderById(event.GetJsonData()["order_id"]);

    if(nullptr != event.GetResponceDataPtr())
    {
        (*event.GetResponceDataPtr())["data"] = event.GetJsonData();
        (*event.GetResponceDataPtr())["error"] = ret;
    }
}

void Engine::EventHandler(Event event)
{
    switch(event.GetEventId())
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
returnType Engine::Filter(Event& event)
{
    returnType ret = RET_NOT_OK;

    if(this->symbol == event.GetJsonData()["symbol"])
    {
        ret = RET_OK;
    }

    return ret;
}