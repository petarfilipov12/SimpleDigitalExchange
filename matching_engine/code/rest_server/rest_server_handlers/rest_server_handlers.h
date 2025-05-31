#ifndef REST_SERVER_HANDLER_H
#define REST_SERVER_HANDLER_H

#include "types.h"
#include "globals.h"
#include <iostream>

using namespace std;

#define HANDLER_FUNC(func_name)    void func_name(const Request &req, Response &res)

HANDLER_FUNC(RestServerHandler_AddOrder)
{
    json responce_data = {
        {"error", RET_INVALID},
        {"data", {}}
    };
    json j_data = json::parse(req.body);
    j_data["order_id"] = rand();
    j_data["status"] = true;

    Event event(EVENT_ID_ADD_ORDER, j_data, &responce_data);

    event_bus.Send(event);

    while(RET_INVALID == responce_data["error"])
    {
        usleep(10);
    }

    res.set_content(responce_data.dump(), "application/json");
}

HANDLER_FUNC(RestServerHandler_CancelOrder)
{
    json responce_data = {
        {"error", RET_INVALID},
        {"data", {}}
    };
    json j_data = json::parse(req.body);

    Event event(EVENT_ID_CANCEL_ORDER, j_data, &responce_data);

    event_bus.Send(event);

    while(RET_INVALID == responce_data["error"])
    {
        usleep(10);
    }

    res.set_content(responce_data.dump(), "application/json");
}

HANDLER_FUNC(RestServerHandler_GetOrder)
{
    json responce_data = {
        {"error", RET_INVALID},
        {"data", {}}
    };
    json j_data = json::parse(req.body);

    Event event(EVENT_ID_GET_ORDER, j_data, &responce_data);

    event_bus.Send(event);

    while(RET_INVALID == responce_data["error"])
    {
        usleep(10);
    }

    res.set_content(responce_data.dump(), "application/json");
}

HANDLER_FUNC(RestServerHandler_GetOrderBook)
{
    json responce_data = {
        {"error", RET_INVALID},
        {"data", {}}
    };

    Event event(EVENT_ID_GET_ORDER_BOOK, {}, &responce_data);

    event_bus.Send(event);

    while(RET_INVALID == responce_data["error"])
    {
        usleep(10);
    }

    res.set_content(responce_data.dump(), "application/json");
}

HANDLER_FUNC(RestServerHandler_GetCandles)
{
    json responce_data = {
        {"error", RET_INVALID},
        {"data", {}}
    };
    json j_data = json::parse(req.body);

    Event event(EVENT_ID_GET_CANDLES, j_data, &responce_data);

    event_bus.Send(event);

    while(RET_INVALID == responce_data["error"])
    {
        usleep(10);
    }

    res.set_content(responce_data.dump(), "application/json");
}

#endif