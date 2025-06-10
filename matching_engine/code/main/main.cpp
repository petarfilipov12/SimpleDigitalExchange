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

CacheOrders cache_orders;
Cache_OrderBookL2 cache_order_book_l2;
CacheCandles cache_candles;
CacheTrades cache_trades;

void Init_EventBus()
{
    thread thread_event_bus([]{event_bus.run();});
    thread_event_bus.detach();
}

void Init_Engine()
{
    thread thread_engine([]{engine.run();});
    thread_engine.detach();

    event_bus.AddReceiver(RECEIVER_ID_ENGINE, Engine::EventHandler);

    event_bus.Subscribe(RECEIVER_ID_ENGINE, EVENT_ID_ADD_ORDER);
    event_bus.Subscribe(RECEIVER_ID_ENGINE, EVENT_ID_CANCEL_ORDER);
}

void Init_EventLogger()
{
    event_bus.AddReceiver(RECEIVER_ID_EVENT_LOGGER, EventLogger_EventHandler);
    EventLogger_Subscribe();
}

void Init_CacheOrders()
{
    event_bus.AddReceiver(RECEIVER_ID_CACHE_ORDERS, CacheOrders::EventHandler);
    
    event_bus.Subscribe(RECEIVER_ID_CACHE_ORDERS, EVENT_ID_TAKER_ORDER_ADDED);
    event_bus.Subscribe(RECEIVER_ID_CACHE_ORDERS, EVENT_ID_TAKER_ORDER_CANCELED);
    event_bus.Subscribe(RECEIVER_ID_CACHE_ORDERS, EVENT_ID_MAKER_ORDER_CANCELED);
    event_bus.Subscribe(RECEIVER_ID_CACHE_ORDERS, EVENT_ID_ORDER_FILLED);
    event_bus.Subscribe(RECEIVER_ID_CACHE_ORDERS, EVENT_ID_GET_ORDER);
}

void Init_CacheOrderBookL2()
{
    event_bus.AddReceiver(RECEIVER_ID_CACHE_ORDER_BOOK_L2, Cache_OrderBookL2::EventHandler);
    
    event_bus.Subscribe(RECEIVER_ID_CACHE_ORDER_BOOK_L2, EVENT_ID_MAKER_ORDER_ADDED);
    event_bus.Subscribe(RECEIVER_ID_CACHE_ORDER_BOOK_L2, EVENT_ID_MAKER_ORDER_CANCELED);
    event_bus.Subscribe(RECEIVER_ID_CACHE_ORDER_BOOK_L2, EVENT_ID_ORDER_FILLED);
    event_bus.Subscribe(RECEIVER_ID_CACHE_ORDER_BOOK_L2, EVENT_ID_GET_ORDER_BOOK);
}

void Init_CacheCandles()
{
    thread thread_cache_candles([]{cache_candles.run();});
    thread_cache_candles.detach();

    event_bus.AddReceiver(RECEIVER_ID_CACHE_CANDLES, CacheCandles::EventHandler);
    
    event_bus.Subscribe(RECEIVER_ID_CACHE_CANDLES, EVENT_ID_ORDER_FILLED);
    event_bus.Subscribe(RECEIVER_ID_CACHE_CANDLES, EVENT_ID_GET_CANDLES);
}

void Init_CacheTrades()
{
    event_bus.AddReceiver(RECEIVER_ID_CACHE_TRADES, CacheTrades::EventHandler);
    
    event_bus.Subscribe(RECEIVER_ID_CACHE_TRADES, EVENT_ID_ORDER_FILLED);
    event_bus.Subscribe(RECEIVER_ID_CACHE_TRADES, EVENT_ID_GET_TRADES);
}

void Init_RestServer()
{
    rest_server.Post("/add_order", RestServer::Handler_AddOrder);
    rest_server.Post("/cancel_order", RestServer::Handler_CancelOrder);
    rest_server.Post("/get_order", RestServer::Handler_GetOrder);
    rest_server.Post("/get_order_book", RestServer::Handler_GetOrderBook);
    rest_server.Post("/get_candles", RestServer::Handler_GetCandles);
    rest_server.Post("/get_trades", RestServer::Handler_GetTrades);

    thread thread_rest_server([]{rest_server.run();});
    thread_rest_server.detach();
}

int main(void){
    srand(time(0));

    // cout << "Init sleep for 1 min\n";
    // sleep(60);
    // cout << "Start\n";
    
    Init_EventBus();
    Init_EventLogger();
    Init_CacheOrders();
    Init_CacheOrderBookL2();
    Init_CacheCandles();
    Init_CacheTrades();
    Init_Engine();
    Init_RestServer();

    while(true)
    {
        sleep(1);
    }

    return 0;
}
