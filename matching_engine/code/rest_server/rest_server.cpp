#include "rest_server.h"

#include "return_type.h"

#include "event.h"

#include "json.h"



RestServer::RestServer(const std::string& cert_path, const std::string& key_path, EventBus& event_bus): event_bus(event_bus)
{
    this->svr = new httplib::SSLServer(cert_path.c_str(), key_path.c_str());

    this->host = "0.0.0.0";
    this->port = 8080;
}

RestServer::RestServer(const std::string& cert_path, const std::string& key_path, const std::string& host, const unsigned int port, EventBus& event_bus): event_bus(event_bus)
{
    this->svr = new httplib::SSLServer(cert_path.c_str(), key_path.c_str());

    this->host = host;
    this->port = port;
}

RestServer::~RestServer()
{
    delete this->svr;
}

void RestServer::Post(const std::string& url_path, const std::function<void(const httplib::Request &, httplib::Response &)> handler_func)
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
void RestServer::init()
{
    this->Post("/add_order", std::bind(&RestServer::Handler_AddOrder, this, std::placeholders::_1, std::placeholders::_2));
    this->Post("/cancel_order", std::bind(&RestServer::Handler_CancelOrder, this, std::placeholders::_1, std::placeholders::_2));
    this->Post("/get_order", std::bind(&RestServer::Handler_GetOrder, this, std::placeholders::_1, std::placeholders::_2));
    this->Post("/get_order_book", std::bind(&RestServer::Handler_GetOrderBook, this, std::placeholders::_1, std::placeholders::_2));
    this->Post("/get_candles", std::bind(&RestServer::Handler_GetCandles, this, std::placeholders::_1, std::placeholders::_2));
    this->Post("/get_trades", std::bind(&RestServer::Handler_GetTrades, this, std::placeholders::_1, std::placeholders::_2));
    this->Post("/get_exchange_info", std::bind(&RestServer::Handler_GetExchangeInfo, this, std::placeholders::_1, std::placeholders::_2));

    std::thread thread_rest_server([this]{this->run();});
    thread_rest_server.detach();
}

/************************/
/*Handler Implementation*/
/************************/
inline void RestServer::HandleRequest(eventId_t event_id, const json& j_data, httplib::Response &res)
{
    std::shared_ptr<json> responce_data(new json);
    responce_data->at("error") = RET_INVALID;
    responce_data->at("data") = {};

    this->event_bus.Send(Event(event_id, j_data, responce_data));

    while(RET_INVALID == responce_data->at("error"))
    {
        usleep(10);
    }

    res.set_content(responce_data->dump(), "application/json");
}

void RestServer::Handler_AddOrder(const httplib::Request &req, httplib::Response &res)
{
    json j_data = json::parse(req.body);
    j_data["order_id"] = rand();
    j_data["filled"] = 0.0;
    j_data["status"] = true;
    j_data["order_added_timestamp"] = 0;

    this->HandleRequest(EVENT_ID_ADD_ORDER, j_data, res);
}

void RestServer::Handler_CancelOrder(const httplib::Request &req, httplib::Response &res)
{
    this->HandleRequest(EVENT_ID_CANCEL_ORDER, json::parse(req.body), res);
}

void RestServer::Handler_GetOrder(const httplib::Request &req, httplib::Response &res)
{
    this->HandleRequest(EVENT_ID_GET_ORDER, json::parse(req.body), res);
}

void RestServer::Handler_GetOrderBook(const httplib::Request &req, httplib::Response &res)
{
    this->HandleRequest(EVENT_ID_GET_ORDER_BOOK, json::parse(req.body), res);
}

void RestServer::Handler_GetCandles(const httplib::Request &req, httplib::Response &res)
{
    this->HandleRequest(EVENT_ID_GET_CANDLES, json::parse(req.body), res);
}

void RestServer::Handler_GetTrades(const httplib::Request &req, httplib::Response &res)
{
    this->HandleRequest(EVENT_ID_GET_TRADES, json::parse(req.body), res);
}

void RestServer::Handler_GetExchangeInfo(const httplib::Request &req, httplib::Response &res)
{
    std::shared_ptr<json> responce_data(new json);
    // responce_data->at("error") = RET_INVALID;
    // responce_data->at("data") = {};

    this->event_bus.Send(Event(EVENT_ID_GET_EXCHANGE_INFO, json::parse(req.body), responce_data));

    while(RET_INVALID == responce_data->at("error"))
    {
        usleep(10);
    }

    res.set_content(responce_data->dump(), "application/json");
}
