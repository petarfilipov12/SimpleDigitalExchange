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



int main(void){
    srand(time(0));

    // std::cout << "Init sleep for 1 min\n";
    // sleep(60);
    // std::cout << "Start\n";
    
    EventBus event_bus;
    Engine engine(event_bus);
    RestServer rest_server("../../server_certs/cert2.pem", "../../server_certs/key2.pem", event_bus);
    EventLogger event_logger;

    CacheOrders cache_orders;
    CacheOrderBookL2 cache_order_book_l2;
    CacheCandles cache_candles;
    CacheTrades cache_trades;

    event_bus.init();
    event_logger.init(event_bus);
    cache_orders.init(event_bus);
    cache_order_book_l2.init(event_bus);
    cache_candles.init(event_bus);
    cache_trades.init(event_bus);
    engine.init();
    rest_server.init();

    while(true)
    {
        sleep(1);
    }

    return 0;
}
