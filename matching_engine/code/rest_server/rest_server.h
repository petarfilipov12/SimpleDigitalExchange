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


};

#endif