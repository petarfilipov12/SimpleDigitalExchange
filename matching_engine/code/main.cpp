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

#define RECEIVER_ID_ENGINE_SYMBOL_1 1u
#define RECEIVER_ID_CACHE_ORDERS_SYMBOL_1 2u
#define RECEIVER_ID_CACHE_ORDER_BOOK_L2_SYMBOL_1 3u
#define RECEIVER_ID_CACHE_CANDLES_SYMBOL_1 4u
#define RECEIVER_ID_CACHE_TRADES_SYMBOL_1 5u


int main(void){
    srand(time(0));

    // std::cout << "Init sleep for 1 min\n";
    // sleep(60);
    // std::cout << "Start\n";
    
    EventBus event_bus;
    Engine engine(event_bus, "SYMBOL_1");
    RestServer rest_server("../../server_certs/cert2.pem", "../../server_certs/key2.pem", event_bus);
    EventLogger event_logger;

    CacheOrders cache_orders("SYMBOL_1");
    CacheOrderBookL2 cache_order_book_l2("SYMBOL_1");
    CacheCandles cache_candles("SYMBOL_1");
    CacheTrades cache_trades("SYMBOL_1");

    event_bus.init();
    event_logger.init(event_bus);
    cache_orders.init(event_bus, RECEIVER_ID_CACHE_ORDERS_SYMBOL_1);
    cache_order_book_l2.init(event_bus, RECEIVER_ID_CACHE_ORDER_BOOK_L2_SYMBOL_1);
    cache_candles.init(event_bus, RECEIVER_ID_CACHE_CANDLES_SYMBOL_1);
    cache_trades.init(event_bus, RECEIVER_ID_CACHE_TRADES_SYMBOL_1);
    engine.init(RECEIVER_ID_ENGINE_SYMBOL_1);
    rest_server.init();

    while(true)
    {
        sleep(1);
    }

    return 0;
}
