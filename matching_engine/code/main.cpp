#include <iostream>
#include "json.h"

#include "engine.h"
#include "event_bus.h"

#include "engine.h"
#include "event_logger.h"

#include "rest_server.h"

#include "cache_orders.h"
#include "cache_order_book_l2.h"
#include "cache_candles.h"
#include "cache_trades.h"

#include "exchange_info.h"

#include <string>
#include <map>
#include <memory>
#include <chrono>

struct SymbolSpecificModules
{
    std::unique_ptr<Engine> engine;

    std::unique_ptr<CacheOrders> cache_orders;
    std::unique_ptr<CacheOrderBookL2> cache_order_book_l2;
    std::unique_ptr<CacheCandles> cache_candles;
    std::unique_ptr<CacheTrades> cache_trades;
};

void InitSymbol(std::map<std::string, SymbolSpecificModules>& symbol_modules_map, const std::string& symbol, EventBus& event_bus, int& last_event_receiver_id)
{
    event_bus.Send(Event(EVENT_ID_ADD_SYMBOL, {{"symbol", symbol}}, nullptr));

    SymbolSpecificModules temp;

    temp.engine = std::make_unique<Engine>(symbol, event_bus, ++last_event_receiver_id);
    temp.cache_orders = std::make_unique<CacheOrders>(symbol, event_bus, ++last_event_receiver_id);
    temp.cache_order_book_l2 = std::make_unique<CacheOrderBookL2>(symbol, event_bus, ++last_event_receiver_id);
    temp.cache_candles = std::make_unique<CacheCandles>(symbol, event_bus, ++last_event_receiver_id);
    temp.cache_trades = std::make_unique<CacheTrades>(symbol, event_bus, ++last_event_receiver_id);

    symbol_modules_map[symbol] = std::move(temp);
}

int main(void){
    std::map<std::string, SymbolSpecificModules> symbol_modules_map;
    srand(time(0));

    // std::cout << "Init sleep for 1 min\n";
    // std::this_thread::sleep_for(std::chrono::seconds(60));
    // std::cout << "Start\n";

    int last_event_receiver_id = RECEIVER_ID_EVENT_LOGGER;
    
    EventBus event_bus;
    EventLogger event_logger(event_bus);
    ExchangeInfo exchange_info(event_bus, ++last_event_receiver_id);

    InitSymbol(symbol_modules_map, "SYMBOL_1", event_bus, last_event_receiver_id);
    InitSymbol(symbol_modules_map, "SYMBOL_2", event_bus, last_event_receiver_id);

    RestServer rest_server("../../server_certs/cert2.pem", "../../server_certs/key2.pem", event_bus);

    while(true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
