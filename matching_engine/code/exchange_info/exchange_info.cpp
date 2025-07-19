#include "exchange_info.h"

void ExchangeInfo::EventHandler_AddSymbol(Event &event)
{
    this->AddSymbol({event.GetDataIn()["symbol"]});
}

void ExchangeInfo::EventHandler_GetExchangeInfo(Event &event)
{
    json exchange_info;
    returnType ret = RET_NOT_OK;

    if (nullptr != event.GetDataOut())
    {
        ret = this->GetExchangeInfo(exchange_info);

        if (RET_OK == ret)
        {
            (*event.GetDataOut())["data"] = exchange_info;
        }

        (*event.GetDataOut())["error"] = ret;
    }
}

ExchangeInfo::ExchangeInfo(EventBus& event_bus, receiverId_t receiver_id)
{
    this->event_receiver = EventReceiver(
        receiver_id,
        std::bind(&ExchangeInfo::EventHandler, this, std::placeholders::_1),
        nullptr);

    event_bus.AddReceiver(&this->event_receiver);

    event_bus.Subscribe(receiver_id, EVENT_ID_ADD_SYMBOL);
    event_bus.Subscribe(receiver_id, EVENT_ID_GET_EXCHANGE_INFO);
}

ExchangeInfo::~ExchangeInfo() {}

returnType ExchangeInfo::AddSymbol(const symbol_data::SymbolData &symbol_data)
{
    this->symbols_data[symbol_data.symbol] = symbol_data;

    return RET_OK;
}

returnType ExchangeInfo::GetExchangeInfo(json &data) const
{
    data = this->symbols_data;

    return RET_OK;
}

void ExchangeInfo::EventHandler(Event event)
{
    switch (event.GetEventId())
    {
    case EVENT_ID_ADD_SYMBOL:
        this->EventHandler_AddSymbol(event);
        break;
    case EVENT_ID_GET_EXCHANGE_INFO:
        this->EventHandler_GetExchangeInfo(event);
        break;
    default:
        break;
    }
}