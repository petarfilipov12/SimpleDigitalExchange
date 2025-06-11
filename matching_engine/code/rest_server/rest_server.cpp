#include "rest_server.h"

#include "types.h"

#include "event.h"
#include "globals.h"

#include<nlohmann/json.hpp>
using json = nlohmann::json;

using namespace httplib;
using namespace std;

RestServer::RestServer(const string& cert_path, const string& key_path)
{
    this->svr = new SSLServer(cert_path.c_str(), key_path.c_str());

    this->host = "0.0.0.0";
    this->port = 8080;
}

RestServer::RestServer(const string& cert_path, const string& key_path, const string& host, const unsigned int port)
{
    this->svr = new SSLServer(cert_path.c_str(), key_path.c_str());

    this->host = host;
    this->port = port;
}

RestServer::~RestServer()
{
    delete this->svr;
}

void RestServer::Post(const string& url_path, const function<void(const Request &, Response &)> handler_func)
{
    this->svr->Post(url_path, handler_func);
}

void RestServer::run()
{
    this->svr->listen(this->host, this->port);
}
/**************************/
/*Init Func implementation*/
/**************************/
void RestServer::init(RestServer& rest_server)
{
    rest_server.Post("/add_order", RestServer::Handler_AddOrder);
    rest_server.Post("/cancel_order", RestServer::Handler_CancelOrder);
    rest_server.Post("/get_order", RestServer::Handler_GetOrder);
    rest_server.Post("/get_order_book", RestServer::Handler_GetOrderBook);
    rest_server.Post("/get_candles", RestServer::Handler_GetCandles);
    rest_server.Post("/get_trades", RestServer::Handler_GetTrades);

    thread thread_rest_server([&rest_server]{rest_server.run();});
    thread_rest_server.detach();
}

/************************/
/*Handler Implementation*/
/************************/
void RestServer::Handler_AddOrder(const Request &req, Response &res)
{
    json responce_data = {
        {"error", RET_INVALID},
        {"data", {}}
    };
    json j_data = json::parse(req.body);
    j_data["order_id"] = rand();
    j_data["filled"] = 0.0;
    j_data["status"] = true;
    j_data["order_added_timestamp"] = 0;

    event_bus.Send(Event(EVENT_ID_ADD_ORDER, j_data, &responce_data));

    while(RET_INVALID == responce_data["error"])
    {
        usleep(10);
    }

    res.set_content(responce_data.dump(), "application/json");
}

void RestServer::Handler_CancelOrder(const Request &req, Response &res)
{
    json responce_data = {
        {"error", RET_INVALID},
        {"data", {}}
    };
    json j_data = json::parse(req.body);

    event_bus.Send(Event(EVENT_ID_CANCEL_ORDER, j_data, &responce_data));

    while(RET_INVALID == responce_data["error"])
    {
        usleep(10);
    }

    res.set_content(responce_data.dump(), "application/json");
}

void RestServer::Handler_GetOrder(const Request &req, Response &res)
{
    json responce_data = {
        {"error", RET_INVALID},
        {"data", {}}
    };
    json j_data = json::parse(req.body);

    event_bus.Send(Event(EVENT_ID_GET_ORDER, j_data, &responce_data));

    while(RET_INVALID == responce_data["error"])
    {
        usleep(10);
    }

    res.set_content(responce_data.dump(), "application/json");
}

void RestServer::Handler_GetOrderBook(const Request &req, Response &res)
{
    json responce_data = {
        {"error", RET_INVALID},
        {"data", {}}
    };

    event_bus.Send(Event(EVENT_ID_GET_ORDER_BOOK, {}, &responce_data));

    while(RET_INVALID == responce_data["error"])
    {
        usleep(10);
    }

    res.set_content(responce_data.dump(), "application/json");
}

void RestServer::Handler_GetCandles(const Request &req, Response &res)
{
    json responce_data = {
        {"error", RET_INVALID},
        {"data", {}}
    };
    json j_data = json::parse(req.body);

    event_bus.Send(Event(EVENT_ID_GET_CANDLES, j_data, &responce_data));

    while(RET_INVALID == responce_data["error"])
    {
        usleep(10);
    }

    res.set_content(responce_data.dump(), "application/json");
}

void RestServer::Handler_GetTrades(const Request &req, Response &res)
{
    json responce_data = {
        {"error", RET_INVALID},
        {"data", {}}
    };
    json j_data = json::parse(req.body);

    event_bus.Send(Event(EVENT_ID_GET_TRADES, j_data, &responce_data));

    while(RET_INVALID == responce_data["error"])
    {
        usleep(10);
    }

    res.set_content(responce_data.dump(), "application/json");
}
