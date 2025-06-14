#include "rest_server.h"

#include "types.h"

#include "event.h"


#include<nlohmann/json.hpp>
using json = nlohmann::json;

using namespace httplib;
using namespace std;

RestServer::RestServer(const string& cert_path, const string& key_path, EventBus& event_busP): event_bus(event_busP)
{
    this->svr = new SSLServer(cert_path.c_str(), key_path.c_str());

    this->host = "0.0.0.0";
    this->port = 8080;
}

RestServer::RestServer(const string& cert_path, const string& key_path, const string& host, const unsigned int port, EventBus& event_busP): event_bus(event_busP)
{
    this->svr = new SSLServer(cert_path.c_str(), key_path.c_str());

    this->host = host;
    this->port = port;
}

RestServer::~RestServer()
{
    delete this->svr;
}

void RestServer::run()
{
    this->svr->listen(this->host, this->port);
}
/**************************/
/*Init Func implementation*/
/**************************/
void RestServer::init()
{
    this->svr->Post("/add_order", [](const Request &req, Response &res){res.set_content({"testkey", "testval"}, "application/json");});

    // this->svr->Post("/add_order", [this](const Request &req, Response &res){this->Handler_AddOrder(req, res);});
    // this->svr->Post("/cancel_order", httplib::SSLServer::Handler(bind(&RestServer::Handler_CancelOrder, this, placeholders::_1, placeholders::_2)));
    // this->svr->Post("/get_order", httplib::SSLServer::Handler(bind(&RestServer::Handler_GetOrder, this, placeholders::_1, placeholders::_2)));
    // this->svr->Post("/get_order_book", httplib::SSLServer::Handler(bind(&RestServer::Handler_GetOrderBook, this, placeholders::_1, placeholders::_2)));
    // this->svr->Post("/get_candles", httplib::SSLServer::Handler(bind(&RestServer::Handler_GetCandles, this, placeholders::_1, placeholders::_2)));
    // this->svr->Post("/get_trades", httplib::SSLServer::Handler(bind(&RestServer::Handler_GetTrades, this, placeholders::_1, placeholders::_2)));

    thread thread_rest_server([this]{this->run();});
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

    this->event_bus.Send(Event(EVENT_ID_ADD_ORDER, j_data, &responce_data));

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

    this->event_bus.Send(Event(EVENT_ID_CANCEL_ORDER, j_data, &responce_data));

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

    this->event_bus.Send(Event(EVENT_ID_GET_ORDER, j_data, &responce_data));

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

    this->event_bus.Send(Event(EVENT_ID_GET_ORDER_BOOK, {}, &responce_data));

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

    this->event_bus.Send(Event(EVENT_ID_GET_CANDLES, j_data, &responce_data));

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

    this->event_bus.Send(Event(EVENT_ID_GET_TRADES, j_data, &responce_data));

    while(RET_INVALID == responce_data["error"])
    {
        usleep(10);
    }

    res.set_content(responce_data.dump(), "application/json");
}
