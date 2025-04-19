#ifndef REST_SERVER_HANDLER_H
#define REST_SERVER_HANDLER_H

#include "types.h"
#include "globals.h"
#include <iostream>

using namespace std;

#define HANDLER_FUNC(func_name)    void func_name(const Request &req, Response &res)

HANDLER_FUNC(RestServerHandler_AddOrder)
{
    cout << req.remote_addr << ": " << req.remote_port << ", data: " << req.body << endl;

    json responce_data;
    json j_data = json::parse(req.body);
    j_data["order_id"] = rand();

    Event event(EVENT_ID_ADD_ORDER, j_data, &responce_data);

    event_bus.Send(event);

    while(responce_data.empty())
    {
        //Timer
    }

    res.set_content(responce_data.dump(), "application/json");
}

HANDLER_FUNC(RestServerHandler_CancelOrder)
{
    cout << req.remote_addr << ": " << req.remote_port << ", data: " << req.body << endl;

    json responce_data;
    json j_data = json::parse(req.body);

    Event event(EVENT_ID_CANCEL_ORDER, j_data, &responce_data);

    event_bus.Send(event);

    while(responce_data.empty())
    {
        //Timer
    }

    res.set_content(responce_data.dump(), "application/json");
}

#endif