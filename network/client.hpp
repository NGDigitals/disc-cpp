#ifndef CLIENT_H
#define CLIENT_H

#include <vector>
#include <iostream>

#include <cpprest/json.h>
#include <cpprest/http_client.h>
#pragma comment(lib, "cpprest_2_10")

#include "node.hpp"

class Client{
public:
    Client() {}

    static void HandleCLI(Node &nNode, int nOption);
    static void HandleCLS(Node &nNode, int nOption, std::vector<std::string> vInput);
    static void HandleRequest(web::http::client::http_client & hClient, 
            web::http::method mMethod, web::json::value const & jValue, std::function<void(web::json::value)> Callback);
    static void DisplayJson(web::json::value const & jObject, utility::string_t const & sPrefix);
};

#endif //CLIENT_H