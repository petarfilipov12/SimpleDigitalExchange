#include "globals.h"
#include <iostream>

using namespace std;

#define HANDLER_FUNC(func_name)    void func_name(const Request &req, Response &res)

HANDLER_FUNC(RestServerHandler_HandlerAddOrder){
    cout << req.remote_addr << ": " << req.remote_port << ", data: " << req.body << endl;
    res.set_content("ADD_ORDER", "text/plain");
}