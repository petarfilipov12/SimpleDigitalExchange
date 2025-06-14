#ifndef REST_SERVER_H
#define REST_SERVER_H

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"

#include <string>

#include "event_bus.h"

using namespace httplib;
using namespace std;

class RestServer
{
private:
    SSLServer *svr;
    string host;
    unsigned int port;
    EventBus& event_bus;

public:
    RestServer(const string& cert_path, const string& key_path, EventBus& event_busP);

    RestServer(const string& cert_path, const string& key_path, const string& host, const unsigned int port, EventBus& event_busP);

    ~RestServer();

    void run();

    void init();

    void Handler_AddOrder(const Request &req, Response &res);

    void Handler_CancelOrder(const Request &req, Response &res);

    void Handler_GetOrder(const Request &req, Response &res);

    void Handler_GetOrderBook(const Request &req, Response &res);

    void Handler_GetCandles(const Request &req, Response &res);

    void Handler_GetTrades(const Request &req, Response &res);
};

#endif