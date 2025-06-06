#include "rest_server_handlers.h"

#include "event.h"
#include "globals.h"

#include<nlohmann/json.hpp>
using json = nlohmann::json;

void RestServerHandler_AddOrder(const Request &req, Response &res)
{
    json responce_data = {
        {"error", RET_INVALID},
        {"data", {}}
    };
    json j_data = json::parse(req.body);
    j_data["order_id"] = rand();
    j_data["filled"] = 0.0;
    j_data["status"] = true;

    Event event(EVENT_ID_ADD_ORDER, j_data, &responce_data);

    event_bus.Send(event);

    while(RET_INVALID == responce_data["error"])
    {
        usleep(10);
    }

    res.set_content(responce_data.dump(), "application/json");
}

void RestServerHandler_CancelOrder(const Request &req, Response &res)
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

void RestServerHandler_GetOrder(const Request &req, Response &res)
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

void RestServerHandler_GetOrderBook(const Request &req, Response &res)
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

void RestServerHandler_GetCandles(const Request &req, Response &res)
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

void RestServerHandler_GetTrades(const Request &req, Response &res)
{
    json responce_data = {
        {"error", RET_INVALID},
        {"data", {}}
    };
    json j_data = json::parse(req.body);

    Event event(EVENT_ID_GET_TRADES, j_data, &responce_data);

    event_bus.Send(event);

    while(RET_INVALID == responce_data["error"])
    {
        usleep(10);
    }

    res.set_content(responce_data.dump(), "application/json");
}