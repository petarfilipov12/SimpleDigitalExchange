#ifndef REST_SERVER_H
#define REST_SERVER_H

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"

#include <string>
#include <functional>

#include "event_bus.h"

class RestServer
{
private:
    httplib::SSLServer *svr;
    std::string host;
    unsigned int port;

    EventBus& event_bus;

public:
    RestServer(const std::string& cert_path, const std::string& key_path, EventBus& event_bus);

    RestServer(const std::string& cert_path, const std::string& key_path, const std::string& host, const unsigned int port, EventBus& event_bus);

    ~RestServer();

    void Post(const std::string& url_path, const std::function<void(const httplib::Request&, httplib::Response&)> handler_func);

    void run();

    void init();

    void Handler_AddOrder(const httplib::Request &req, httplib::Response &res);

    void Handler_CancelOrder(const httplib::Request &req, httplib::Response &res);

    void Handler_GetOrder(const httplib::Request &req, httplib::Response &res);

    void Handler_GetOrderBook(const httplib::Request &req, httplib::Response &res);

    void Handler_GetCandles(const httplib::Request &req, httplib::Response &res);

    void Handler_GetTrades(const httplib::Request &req, httplib::Response &res);
};

#endif