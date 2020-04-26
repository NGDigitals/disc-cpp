#ifndef ROUTER_H
#define ROUTER_H

#include <iostream>

#include <cpprest/json.h>
#include <cpprest/http_listener.h>
#pragma comment(lib, "cpprest_2_10")

#include "node.hpp"

struct Router{
public:
    Router(){}
    void RouteGet(const web::http::http_request hRequest);
    void PostRoute(Node &nNode, const web::http::http_request hRequest, web::json::value const & jObject);

private:
    const std::string API_PATH = "/api/v1";
};

#endif //ROUTER_H