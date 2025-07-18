#ifndef EXCHANGE_INFO_H
#define EXCHANGE_INFO_H

#include "return_type.h"

#include <string>
#include <map>

#include "symbol_data.h"

#include "event.h"
#include "event_bus.h"
#include "json.h"

class ExchangeInfo
{   
    private:
        std::map<std::string, symbol_data::SymbolData> symbols_data;

        returnType AddSymbol(const symbol_data::SymbolData& symbol_data);

        returnType GetExchangeInfo(json& data)const;

        void EventHandler_AddSymbol(Event &event);

        void EventHandler_GetExchangeInfo(Event &event);

        void EventHandler(Event event);

    public:
        ExchangeInfo(EventBus& event_bus, receiverId_t receiver_id);
        ~ExchangeInfo();
};

#endif