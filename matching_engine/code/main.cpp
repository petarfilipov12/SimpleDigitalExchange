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

void InitSymbol(const std::string& symbol, EventBus& event_bus, int& last_event_receiver_id)
{
    event_bus.Send(Event(EVENT_ID_ADD_SYMBOL, {{"symbol", symbol}}, nullptr));

    Engine* engine = new Engine(symbol, event_bus, ++last_event_receiver_id);

    CacheOrders* cache_orders = new CacheOrders(symbol, event_bus, ++last_event_receiver_id);
    CacheOrderBookL2* cache_order_book_l2 = new CacheOrderBookL2(symbol, event_bus, ++last_event_receiver_id);
    CacheCandles* cache_candles = new CacheCandles(symbol, event_bus, ++last_event_receiver_id);
    CacheTrades* cache_trades = new CacheTrades(symbol, event_bus, ++last_event_receiver_id);
}

int main(void){
    srand(time(0));

    // std::cout << "Init sleep for 1 min\n";
    // sleep(60);
    // std::cout << "Start\n";

    int last_event_receiver_id = RECEIVER_ID_EVENT_LOGGER;
    
    EventBus event_bus;
    EventLogger event_logger(event_bus);
    ExchangeInfo exchange_info(event_bus, ++last_event_receiver_id);

    InitSymbol("SYMBOL_1", event_bus, last_event_receiver_id);
    InitSymbol("SYMBOL_2", event_bus, last_event_receiver_id);

    RestServer rest_server("../../server_certs/cert2.pem", "../../server_certs/key2.pem", event_bus);

    while(true)
    {
        sleep(1);
    }

    return 0;
}
