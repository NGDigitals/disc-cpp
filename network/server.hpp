#ifndef SERVER_H
#define SERVER_H

#include <map>
#include <set>
#include <iomanip>
#include <iostream>
#include <functional>

#include <cpprest/json.h>
#include <cpprest/http_listener.h>
#pragma comment(lib, "cpprest_2_10")

#include <boost/lexical_cast.hpp>  
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string/predicate.hpp>

#include "node.hpp"
#include "router.hpp"

// struct Router{
// public:
//     Node nNode;
//     const std::vector<std::string> vServices = {"node", "block", "wallet"};
//     const std::vector<std::string> vBroadcasts = {"peers", "wallets", "transactions"};

//     Router(){}
//     Router(const Node & cNode) : nNode(cNode){}
//     void GetRoute(web::http::http_request request, web::json::value const & jObject){
//         request.reply(web::http::status_codes::OK, jObject);
//     }

//     void PostRoute(web::http::http_request & hRequest, web::json::value const & jObject){
//         std::string sRequestPath = hRequest.relative_uri().path();
//         int32_t nStatusCode;
//         auto aResponse = web::json::value::object();
//         if(boost::starts_with(sRequestPath, node::constant::NETWORK_AUTH_PATH.begin())){
//             boost::algorithm::replace_first(sRequestPath, node::constant::NETWORK_AUTH_PATH.begin(), "");
//             std::vector<std::string> vPaths;
//             boost::split(vPaths, sRequestPath, [](char c){
//                 return c == '/';
//             });
//             if(vPaths.size() == 1){
//                 std::string aLabel = jObject.at("label").as_string();
//                 const std::string aToken = jObject.at("token").as_string();
//                 if(aLabel.compare(nNode.GetLabel()) == 0 && aToken.compare(nNode.GetToken()) == 0){
//                     Wallet wWallet = nNode.GetWallet();
//                     web::json::value jWallet;
//                     jWallet["symbol"] = web::json::value::string(wWallet._sSymbol);
//                     jWallet["public_key"] = web::json::value::string(wWallet._cPublicKey);
//                     jWallet["private_key"] = web::json::value::string(wWallet._cPrivateKey);
//                     nStatusCode = web::http::status_codes::OK;
//                     aResponse["success"] = web::json::value::boolean(true);
//                     aResponse["wallet"] = web::json::value::array({jWallet});
//                     aResponse["message"] = web::json::value::string("Node Authorized");
//                 }else{
//                     nStatusCode = web::http::status_codes::OK;
//                     aResponse["success"] = web::json::value::boolean(false);
//                     aResponse["message"] = web::json::value::string("Invalid Credentials");
//                 }
//             }else{
//                 nStatusCode = web::http::status_codes::OK;
//                 aResponse["success"] = web::json::value::boolean(false);
//                 aResponse["message"] = web::json::value::string("Not Allowed");
//             }
//         }else if(boost::starts_with(sRequestPath, node::constant::NETWORK_BROADCAST_PATH.begin())){
//             boost::algorithm::replace_first(sRequestPath, node::constant::NETWORK_BROADCAST_PATH.begin(), "");
//             std::vector<std::string> vNodePaths;
//             boost::split(vNodePaths, sRequestPath, [](char c){
//                 return c == '/';
//             });
//             std::cout << "Broadcasting node..." << vNodePaths.size() << std::endl;
//             if(vNodePaths.size() > 1){
//                 std::string sTargetName = vNodePaths[1];
//                 if(std::count(vBroadcasts.begin(), vBroadcasts.end(), sTargetName)){
//                     std::string sToken = jObject.at("_token").as_string();
//                     if(sTargetName == "wallets"){
//                         std::cout << "Updating wallet..." << std::endl;
//                         web::json::value jWallet = jObject.at("wallet");
//                         Wallet wWallet(jWallet.at("symbol").as_string());
//                         strcpy(wWallet._cPublicKey, jWallet.at("public_key").as_string().c_str());
//                         strcpy(wWallet._cPrivateKey, jWallet.at("private_key").as_string().c_str());
//                         nNode.UpdateWallets(wWallet);
//                         nStatusCode = web::http::status_codes::OK;
//                         aResponse["message"] = web::json::value::string("Wallets Updated");
//                     }else if(sTargetName == "peers"){
//                         std::cout << "Updating nodes...server" << std::endl;
//                         web::json::value jNode = jObject.at("node");
//                         PeerNode pNode(jNode[0].at("label").as_string(), jNode[0].at("address").as_string(), jNode[0].at("token").as_string());
//                         nNode.UpdatePeerNodes(pNode);
//                         nStatusCode = web::http::status_codes::OK;
//                         aResponse["message"] = web::json::value::string("Peer Nodes Updated");
//                     }else{
//                         nStatusCode = web::http::status_codes::OK;
//                         aResponse["message"] = web::json::value::string("Target Not Available");
//                     }
//                 }else{
//                     nStatusCode = web::http::status_codes::OK;
//                     aResponse["message"] = web::json::value::string("Target Not Found");
//                 }
//             }else{
//                 nStatusCode = web::http::status_codes::OK;
//                 aResponse["message"] = web::json::value::string("Invalid Request");
//             }
//         }else if(boost::starts_with(sRequestPath, node::constant::API_PATH.begin())){
//             boost::algorithm::replace_first(sRequestPath, node::constant::API_PATH.begin(), "");
//             std::vector<std::string> vServicePath;
//             boost::split(vServicePath, sRequestPath, [](char c){
//                 return c == '/';
//             });
//             if(vServicePath.size() > 1){
//                 std::string sServiceName = vServicePath[1];
//                 if(std::count(vServices.begin(), vServices.end(), sServiceName)){
//                     if(sServiceName == "node"){
//                         aResponse["message"] = web::json::value::string("Node service");
//                         nStatusCode = web::http::status_codes::OK;
//                     }else if(sServiceName == "block"){
//                         aResponse["message"] = web::json::value::string("Block service");
//                         nStatusCode = web::http::status_codes::OK;
//                     }else if(sServiceName == "wallet"){
//                         aResponse["message"] = web::json::value::string("Wallet service");
//                         nStatusCode = web::http::status_codes::OK;
//                     }else{
//                         nStatusCode = web::http::status_codes::OK;
//                         aResponse["message"] = web::json::value::string("Invalid Request");
//                     }
//                 }
//             }
//         }else{
//             nStatusCode = web::http::status_codes::OK;
//             aResponse["success"] = web::json::value::boolean(false);
//             aResponse["message"] = web::json::value::string("This request URL is invalid");
//         }
//         hRequest.reply(nStatusCode, aResponse);
//     }
// };

class Server {
public:
    Server(const Node & nNode);
    void HandleGet(web::http::http_request hRequest) const;
    void HandlePost(web::http::http_request hRequest);
    void HandleRequest(Node & nNode, web::http::http_request hRequest, 
                 std::function<void(Node &, web::http::http_request &, web::json::value const &)> fAction);
    void DisplayJson(web::json::value const & jValue, utility::string_t const & sPrefix) const;

private:
    Node _nNode;
    Router _rRouter;

};

#endif //__SERVER_H__