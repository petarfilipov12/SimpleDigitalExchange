#ifndef GLOBALS_H
#define GLOBALS_H

#include "event_bus.h"
#include "engine.h"
#include "rest_server.h"
#include "cache_orders.h"
#include "cache_order_book_l2.h"
#include "cache_candles.h"

extern EventBus event_bus;
extern Engine engine;
extern RestServer rest_server;
extern CacheOrders cache_orders;
extern Cache_OrderBookL2 cache_order_book_l2;
extern CacheCandles cache_candles;

#endif