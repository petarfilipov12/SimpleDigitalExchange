#ifndef REST_SERVER_HANDLER_H
#define REST_SERVER_HANDLER_H

#include "types.h"
#include "rest_server.h"

void RestServerHandler_AddOrder(const Request &req, Response &res);

void RestServerHandler_CancelOrder(const Request &req, Response &res);

void RestServerHandler_GetOrder(const Request &req, Response &res);

void RestServerHandler_GetOrderBook(const Request &req, Response &res);

void RestServerHandler_GetCandles(const Request &req, Response &res);

#endif