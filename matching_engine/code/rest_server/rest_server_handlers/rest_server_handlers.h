#include "globals.h"
#include <iostream>

using namespace std;

#define HANDLER_FUNC(func_name)    void func_name(const Request &req, Response &res)

HANDLER_FUNC(RestServerHandler_AddOrder)
{
    cout << req.remote_addr << ": " << req.remote_port << ", data: " << req.body << endl;

    // json j_data;
    // string price;
    // int order_id;
    // int order_side;
    // int order_type;

    // j_data["price"] = price;
    // j_data["order_id"] = order_id;
    // j_data["order_side"] = order_side;
    // j_data["order_type"] = order_type;

    // Event event(EVENT_ID_ADD_ORDER, j_data);

    // event_bus.Send(event);

    res.set_content("ADD_ORDER", "text/plain");
}

HANDLER_FUNC(RestServerHandler_CancelOrder)
{
    cout << req.remote_addr << ": " << req.remote_port << ", data: " << req.body << endl;

    // json j_data;
    // string price;
    // int order_id;
    // int order_side;
    // int order_type;

    // j_data["price"] = price;
    // j_data["order_id"] = order_id;
    // j_data["order_side"] = order_side;
    // j_data["order_type"] = order_type;

    // Event event(EVENT_ID_ADD_ORDER, j_data);

    // event_bus.Send(event);

    res.set_content("CANCEL_ORDER", "text/plain");
}