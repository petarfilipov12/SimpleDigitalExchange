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
    Engine* engine = new Engine(event_bus, symbol);

    CacheOrders* cache_orders = new CacheOrders(symbol);
    CacheOrderBookL2* cache_order_book_l2 = new CacheOrderBookL2(symbol);
    CacheCandles* cache_candles = new CacheCandles(symbol);
    CacheTrades* cache_trades = new CacheTrades(symbol);

    event_bus.Send(Event(EVENT_ID_ADD_SYMBOL, {{"symbol", symbol}}, nullptr));

    cache_orders->init(event_bus, ++last_event_receiver_id);
    cache_order_book_l2->init(event_bus, ++last_event_receiver_id);
    cache_candles->init(event_bus, ++last_event_receiver_id);
    cache_trades->init(event_bus, ++last_event_receiver_id);
    engine->init(++last_event_receiver_id);
}

int main(void){
    srand(time(0));

    // std::cout << "Init sleep for 1 min\n";
    // sleep(60);
    // std::cout << "Start\n";

    int last_event_receiver_id = RECEIVER_ID_EVENT_LOGGER;
    
    EventBus event_bus;
    RestServer rest_server("../../server_certs/cert2.pem", "../../server_certs/key2.pem", event_bus);
    EventLogger event_logger;
    ExchangeInfo exchange_info;

    event_bus.init();
    event_logger.init(event_bus);
    exchange_info.init(event_bus, ++last_event_receiver_id);

    InitSymbol("SYMBOL_1", event_bus, last_event_receiver_id);

    rest_server.init();

    while(true)
    {
        sleep(1);
    }

    return 0;
}
