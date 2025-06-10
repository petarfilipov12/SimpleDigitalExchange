#include "engine.h"

#include "globals.h"

Engine::Engine(EventBus *event_bus)
{
    this->event_bus = event_bus;
}

Engine::~Engine()
{
    // this->taker_orders_queue.clear();
    // this->taker_orders.clear();

    // this->order_book.~OrderBook();
}

bool Engine::ExistsOrder(Order order) const
{
    bool ret = this->taker_book.ExistsTakerOrder(order);

    if (!ret)
    {
        ret = this->order_book.ExistsOrder(order);
    }

    return ret;
}

bool Engine::ExistsOrderId(int id) const
{
    return this->ExistsOrder(Order(id));
}

ReturnType Engine::AddOrder(Order order)
{
    enum eEventId_t event_id = EVENT_ID_ADD_ORDER_FAILLED;
    ReturnType ret = RET_NOT_OK;

    ret = this->taker_book.AddTakerOrder(order);

    if(RET_OK == ret)
    {
        order.SetCurrentTimestamp();
        event_id = EVENT_ID_TAKER_ORDER_ADDED;
    }
    
    this->event_bus->Send(Event(event_id, order.ConvertOrderToJson(), nullptr));

    return ret;
}

ReturnType Engine::CancelOrder(Order order)
{
    enum eEventId_t event_id = EVENT_ID_CANCEL_ORDER_FAILED;
    json j_data = order.ConvertOrderToJson();
    ReturnType ret = RET_NOT_OK;
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
        
    this->event_bus->Send(Event(event_id, j_data, nullptr));

    return ret;
}

ReturnType Engine::CancelOrderById(int id)
{
    return this->CancelOrder(Order(id));
}

ReturnType Engine::AddToOrderBook(Order *pTakerOrder)
{
    ReturnType ret = this->order_book.AddOrder(*pTakerOrder);

    if(RET_OK == ret)
    {
        this->event_bus->Send(Event(EVENT_ID_MAKER_ORDER_ADDED, pTakerOrder->ConvertOrderToJson(), nullptr));
    }

    return ret;
}

ReturnType Engine::MatchTakerOrder(Order *pTakerOrder)
{
    ReturnType ret = RET_NOT_OK;
    Order *bookOrder;
    json j_data;
    float quantity;
    float bookOrder_quantity;

    if ((ORDER_TYPE_MARKET == pTakerOrder->order_type) || (ORDER_TYPE_LIMIT == pTakerOrder->order_type))
    {
        while (pTakerOrder->filled < pTakerOrder->quantity)
        {
            quantity = (pTakerOrder->quantity - pTakerOrder->filled);

            if (pTakerOrder->order_side == ORDER_SIDE_BUY)
            {
                ret = this->order_book.GetAskFirst(&bookOrder);
                if (RET_BOOK_EMPTY == ret)
                {
                    if(ORDER_TYPE_LIMIT == pTakerOrder->order_type)
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

                if ( (ORDER_TYPE_LIMIT == pTakerOrder->order_type) && (stof(pTakerOrder->price) < stof(bookOrder->price)) )
                {
                    this->AddToOrderBook(pTakerOrder);
                    break;
                }
            }
            else if (pTakerOrder->order_side == ORDER_SIDE_SELL)
            {
                ret = this->order_book.GetBidFirst(&bookOrder);
                if (RET_BOOK_EMPTY == ret)
                {
                    if(ORDER_TYPE_LIMIT == pTakerOrder->order_type)
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

                if ( (ORDER_TYPE_LIMIT == pTakerOrder->order_type) && (stof(pTakerOrder->price) > stof(bookOrder->price)) )
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

            j_data["taker_order"] = pTakerOrder->ConvertOrderToJson();
            j_data["book_order"] = bookOrder->ConvertOrderToJson();
            j_data["price"] = bookOrder->price;
            j_data["quantity"] = quantity;

            bookOrder->filled += quantity;
            if (bookOrder->filled >= bookOrder->quantity)
            {
                this->order_book.CancelOrderById(bookOrder->id, nullptr);
            }

            this->event_bus->Send(Event(EVENT_ID_ORDER_FILLED, j_data, nullptr));

            pTakerOrder->filled += quantity;
            j_data = {};
        }
    }

    return RET_OK;
}

void Engine::Cyclic()
{
    Order *takerOrder;
    ReturnType ret = RET_NOT_OK;
    int i = 1;

    if (RET_OK != this->taker_book.GetFirst(&takerOrder))
    {
        return;
    }

    ret = this->MatchTakerOrder(takerOrder);

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

            ret = this->MatchTakerOrder(takerOrder);
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

/******************************/
/*Event_Handler Implementation*/
/******************************/
static void Engine_EventHandler_AddOrder(Event event)
{
    ReturnType ret = RET_NOT_OK;
    json j_data = event.GetJsonData();

    ret = engine.AddOrder(Order::ConvertJsonToOrder(j_data));

    if(nullptr != event.GetResponceDataPtr())
    {
        (*event.GetResponceDataPtr())["data"] = j_data;
        (*event.GetResponceDataPtr())["error"] = ret;
    }

}

static void Engine_EventHandler_CancelOrder(Event event)
{
    ReturnType ret;

    ret = engine.CancelOrderById(event.GetJsonData()["order_id"]);

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
            Engine_EventHandler_AddOrder(event);
            break;
        case EVENT_ID_CANCEL_ORDER:
            Engine_EventHandler_CancelOrder(event);
            break;
        default:
            break;
    }

}