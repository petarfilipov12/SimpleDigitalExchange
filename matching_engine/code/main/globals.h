#ifndef GLOBALS_H
#define GLOBALS_H

#include "event_bus.h"
#include "engine.h"
#include "rest_server.h"
#include "cache_orders.h"

extern EventBus event_bus;
extern Engine engine;
extern RestServer rest_server;
extern CacheOrders cache_orders;

#endif