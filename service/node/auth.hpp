#ifndef NODE_AUTH_H
#define NODE_AUTH_H

namespace service::node{

    #include <iostream>
    #include <cpprest/json.h>
    #include <cpprest/http_listener.h>
    #pragma comment(lib, "cpprest_2_10")

    #include "../../network/node.hpp"

    struct Auth {
    public:
        static const std::string PATH;
        static void Authorize(const web::http::http_request hRequest, Node & nNode, web::json::value const & jObject){
            int32_t nStatusCode;
            std::cout << "Authorizing..."  << std::endl;
            auto aResponse = web::json::value::object();
            const std::string sName = jObject.at("name").as_string();
            const std::string sToken = jObject.at("token").as_string();
            if(sName.compare(nNode.GetName()) == 0 && sToken.compare(nNode.GetToken()) == 0){
                Wallet wWallet = nNode.GetWallet();
                if(!wWallet._sSymbol.empty() && !wWallet._cPublicKey.empty() && !wWallet._cPrivateKey.empty()){
                    web::json::value jWallet;
                    jWallet["symbol"] = web::json::value::string(wWallet._sSymbol);
                    jWallet["public_key"] = web::json::value::string(wWallet._cPublicKey);
                    jWallet["private_key"] = web::json::value::string(wWallet._cPrivateKey);
                    std::cout << "Authorized"  << std::endl;
                    nStatusCode = web::http::status_codes::OK;
                    aResponse["success"] = web::json::value::boolean(true);
                    //aResponse["wallet"] = web::json::value::array({jWallet});
                    aResponse["message"] = web::json::value::string("Node Authorized");
                }else{
                    nStatusCode = web::http::status_codes::OK;
                    aResponse["success"] = web::json::value::boolean(false);
                    aResponse["message"] = web::json::value::string("No wallet found for this node");
                }
            }else{
                std::cout << "Not Authorized"  << std::endl;
                nStatusCode = web::http::status_codes::OK;
                aResponse["success"] = web::json::value::boolean(false);
                aResponse["message"] = web::json::value::string("Invalid Credentials");
            }
            hRequest.reply(nStatusCode, aResponse);
        }
    };
    const std::string Auth::PATH = "/network/auth";
}

#endif //NODE_AUTH_H