#ifndef REST_SERVER_H
#define REST_SERVER_H

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"

#include <string>
#include <functional>

using namespace httplib;
using namespace std;

class RestServer
{
private:
    SSLServer *svr;
    string host;
    unsigned int port;

public:
    RestServer(string cert_path, string key_path);

    RestServer(string cert_path, string key_path, string host, unsigned int port);

    ~RestServer();

    void Post(string url_path, function<void(const Request&, Response&)> handler_func);

    void run();

    static void Handler_AddOrder(const Request &req, Response &res);

    static void Handler_CancelOrder(const Request &req, Response &res);

    static void Handler_GetOrder(const Request &req, Response &res);

    static void Handler_GetOrderBook(const Request &req, Response &res);

    static void Handler_GetCandles(const Request &req, Response &res);

    static void Handler_GetTrades(const Request &req, Response &res);
};

#endif