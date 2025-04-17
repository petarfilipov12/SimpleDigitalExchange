#include "rest_server.h"

using namespace httplib;
using namespace std;

RestServer::RestServer(string cert_path, string key_path)
{
    this->svr = new SSLServer(cert_path.c_str(), key_path.c_str());

    this->host = "0.0.0.0";
    this->port = 8080;
}

RestServer::RestServer(string cert_path, string key_path, string host, unsigned int port)
{
    this->svr = new SSLServer(cert_path.c_str(), key_path.c_str());

    this->host = host;
    this->port = port;
}

RestServer::~RestServer()
{
    delete this->svr;
}

void RestServer::Post(string url_path, function<void(const Request &, Response &)> handler_func)
{
    this->svr->Post(url_path, handler_func);
}

void RestServer::run()
{
    this->svr->listen(this->host, this->port);
}
