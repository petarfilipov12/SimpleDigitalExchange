#ifndef EXCHANGE_INFO_H
#define EXCHANGE_INFO_H

#include "return_type.h"

#include <string>
#include <map>

#include "event.h"
#include "event_bus.h"

namespace symbol_data
{
    struct SymbolData
    {
        std::string symbol;
    };

    void to_json(json& j, const struct SymbolData s_data)
    {
        j = {
            {"symbol", s_data.symbol}
        };
    }
}

class ExchangeInfo
{   
    private:
        std::map<std::string, symbol_data::SymbolData> symbols_data;

        void EventHandler_AddSymbol(Event &event)
        {
            this->AddSymbol({event.GetJsonData()["symbol"]});
        }

        void EventHandler_GetExchangeInfo(Event &event)
        {
            json exchange_info;
            returnType ret = RET_NOT_OK;

            if (nullptr != event.GetResponceDataPtr())
            {
                ret = this->GetExchangeInfo(exchange_info);

                if (RET_OK == ret)
                {
                    (*event.GetResponceDataPtr())["data"] = exchange_info;
                }

                (*event.GetResponceDataPtr())["error"] = ret;
            }
        }

    public:
        ExchangeInfo() {}
        ~ExchangeInfo() {}

        returnType AddSymbol(const symbol_data::SymbolData& symbol_data)
        {
            this->symbols_data[symbol_data.symbol] = symbol_data;

            return RET_OK;
        }

        returnType GetExchangeInfo(json& data)const
        {
            data = this->symbols_data;

            return RET_OK;
        }

        void init(EventBus& event_bus, receiverId_t receiver_id)
        {
            EventReceiver event_receiver = EventReceiver(
                receiver_id, 
                std::bind(&ExchangeInfo::EventHandler, this, std::placeholders::_1),
                nullptr
            );
        
            event_bus.AddReceiver(event_receiver);

            event_bus.Subscribe(receiver_id, EVENT_ID_ADD_SYMBOL);
            event_bus.Subscribe(receiver_id, EVENT_ID_GET_EXCHANGE_INFO);
        }

        void EventHandler(Event event)
        {
            switch(event.GetEventId())
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
};

#endif