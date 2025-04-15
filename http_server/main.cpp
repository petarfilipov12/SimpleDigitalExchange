#include <iostream>

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"

using namespace std;
using namespace httplib;

#define HANDLER_FUNC(func_name)    void func_name(const Request &req, Response &res)

HANDLER_FUNC(HandlerFunc){
    cout << req.remote_addr << ": " << req.remote_port << endl;
    res.set_content("HIII", "text/plain");
}

int main(void){

    // HTTP
    SSLServer svr("../../server_certs/cert.pem", "../../server_certs/key.pem");

    svr.Get("/hi", HandlerFunc);

    svr.listen("0.0.0.0", 8080);

    return 0;
}
