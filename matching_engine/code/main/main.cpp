#include <iostream>
#include <nlohmann/json.hpp>
#include <thread>

#include "globals.h"

#include "engine.h"
#include "order.h"

#include "event.h"
#include "event_bus.h"

#include "engine_event_handler.h"
#include "event_receiver_id.h"
#include "event_logger.h"

#include "rest_server.h"
#include "rest_server_handlers.h"

//#include "cache_orders.h"
#include "cache_orders_event_handler.h"
#include "cache_order_book_l2_event_handler.h"

#include "cache_candles_event_handler.h"


using json = nlohmann::json;
using namespace std;

EventBus event_bus;
Engine engine(&event_bus);
RestServer rest_server("../../server_certs/cert2.pem", "../../server_certs/key2.pem");
CacheOrders cache_orders;
Cache_OrderBookL2 cache_order_book_l2;
CacheCandles cache_candles;

void Init_EventBus()
{
    thread thread_event_bus([]{event_bus.run();});
    thread_event_bus.detach();
}

void Init_Engine()
{
    thread thread_engine([]{engine.run();});
    thread_engine.detach();

    event_bus.AddReceiver(RECEIVER_ID_ENGINE, Engine_EventHandler);

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
    event_bus.AddReceiver(RECEIVER_ID_CACHE_ORDERS, CacheOrders_EventHandler);
    
    event_bus.Subscribe(RECEIVER_ID_CACHE_ORDERS, EVENT_ID_ORDER_ADDED);
    event_bus.Subscribe(RECEIVER_ID_CACHE_ORDERS, EVENT_ID_ORDER_CANCELED);
    event_bus.Subscribe(RECEIVER_ID_CACHE_ORDERS, EVENT_ID_ORDER_FILLED);
    event_bus.Subscribe(RECEIVER_ID_CACHE_ORDERS, EVENT_ID_GET_ORDER);
}

void Init_CacheOrderBookL2()
{
    event_bus.AddReceiver(RECEIVER_ID_CACHE_ORDER_BOOK_L2, CacheOrderBookL2_EventHandler);
    
    event_bus.Subscribe(RECEIVER_ID_CACHE_ORDER_BOOK_L2, EVENT_ID_ORDER_ADDED);
    event_bus.Subscribe(RECEIVER_ID_CACHE_ORDER_BOOK_L2, EVENT_ID_ORDER_CANCELED);
    event_bus.Subscribe(RECEIVER_ID_CACHE_ORDER_BOOK_L2, EVENT_ID_ORDER_FILLED);
    event_bus.Subscribe(RECEIVER_ID_CACHE_ORDER_BOOK_L2, EVENT_ID_GET_ORDER_BOOK);
}

void Init_CacheCandles()
{
    thread thread_cache_candles([]{cache_candles.run();});
    thread_cache_candles.detach();

    event_bus.AddReceiver(RECEIVER_ID_CACHE_CANDLES, CacheCandles_EventHandler);
    
    event_bus.Subscribe(RECEIVER_ID_CACHE_CANDLES, EVENT_ID_ORDER_FILLED);
    event_bus.Subscribe(RECEIVER_ID_CACHE_CANDLES, EVENT_ID_GET_CANDLES);
}

void Init_RestServer()
{
    rest_server.Post("/add_order", RestServerHandler_AddOrder);
    rest_server.Post("/cancel_order", RestServerHandler_CancelOrder);
    rest_server.Post("/get_order", RestServerHandler_GetOrder);
    rest_server.Post("/get_order_book", RestServerHandler_GetOrderBook);
    rest_server.Post("/get_candles", RestServerHandler_GetCandles);

    thread thread_rest_server([]{rest_server.run();});
    thread_rest_server.detach();
}

int main(void){
    srand(time(0));
    
    Init_EventBus();
    Init_EventLogger();
    Init_CacheOrders();
    Init_CacheOrderBookL2();
    Init_CacheCandles();
    Init_Engine();
    Init_RestServer();

    while(true)
    {
        sleep(1);
    }

    return 0;
}
