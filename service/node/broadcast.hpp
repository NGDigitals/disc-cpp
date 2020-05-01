#ifndef NODE_BROADCAST_H
#define NODE_BROADCAST_H

namespace service::node{

    #include <iostream>
    #include <cpprest/json.h>
    #include <cpprest/http_listener.h>
    #pragma comment(lib, "cpprest_2_10")

    #include "../../network/node.hpp"

    struct Broadcast {
    public:
        static const std::string PATH_NODE;
        static const std::string PATH_WALLET;
        static const std::string PATH_TRANSACTION;

        static void AddNode(const web::http::http_request hRequest, Node & nNode, web::json::value const & jObject){
            int32_t nStatusCode;
            auto aResponse = web::json::value::object();
            std::string sToken = jObject.at("_token").as_string();
            web::json::value jNode = jObject.at("node");
            nNode.AddPeer(Node(jNode[0].at("name").as_string(), jNode[0].at("address").as_string(), jNode[0].at("token").as_string()));
            nStatusCode = web::http::status_codes::OK;
            aResponse["message"] = web::json::value::string("Wallets Updated");
            hRequest.reply(nStatusCode, aResponse);
        }

        static void AddWallet(const web::http::http_request hRequest, Node & nNode, web::json::value const & jObject){
            int32_t nStatusCode;
            auto aResponse = web::json::value::object();
            std::string sToken = jObject.at("_token").as_string();
            web::json::value jWallet = jObject.at("wallet");
            // Wallet wWallet(jWallet.at("symbol").as_string());
            // strcpy(wWallet._cPublicKey, jWallet.at("public_key").as_string().c_str());
            // strcpy(wWallet._cPrivateKey, jWallet.at("private_key").as_string().c_str());
            // nNode.UpdateWallets(wWallet);
            nStatusCode = web::http::status_codes::OK;
            aResponse["message"] = web::json::value::string("Wallets Updated");
            hRequest.reply(nStatusCode, aResponse);
        }
    };
    const std::string Broadcast::PATH_NODE = "/network/broadcast/node";
    const std::string Broadcast::PATH_WALLET = "/network/broadcast/wallet";
    const std::string Broadcast::PATH_TRANSACTION = "/network/broadcast/transaction";
}

#endif //NODE_BROADCAST_H