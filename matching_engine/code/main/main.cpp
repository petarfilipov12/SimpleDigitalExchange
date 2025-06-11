#include <iostream>
#include <nlohmann/json.hpp>
#include <thread>

#include "globals.h"

#include "engine.h"
#include "order.h"

#include "event.h"
#include "event_bus.h"

#include "engine.h"
#include "event_receiver.h"
#include "event_logger.h"

#include "rest_server.h"

#include "cache_orders.h"
#include "cache_order_book_l2.h"
#include "cache_candles.h"
#include "cache_trades.h"

using json = nlohmann::json;
using namespace std;

EventBus event_bus;
Engine engine(event_bus);
RestServer rest_server("../../server_certs/cert2.pem", "../../server_certs/key2.pem");
EventLogger event_logger;

CacheOrders cache_orders;
Cache_OrderBookL2 cache_order_book_l2;
CacheCandles cache_candles;
CacheTrades cache_trades;

int main(void){
    srand(time(0));

    // cout << "Init sleep for 1 min\n";
    // sleep(60);
    // cout << "Start\n";
    
    EventBus::init(event_bus);
    EventLogger::init(event_bus);
    CacheOrders::init(event_bus);
    Cache_OrderBookL2::init(event_bus);
    CacheCandles::init(cache_candles, event_bus);
    CacheTrades::init(event_bus);
    Engine::init(engine, event_bus);
    RestServer::init(rest_server);

    while(true)
    {
        sleep(1);
    }

    return 0;
}
