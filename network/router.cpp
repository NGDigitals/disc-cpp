#include <boost/algorithm/string/replace.hpp>

#include "router.hpp"
#include "../service/node/auth.hpp"
#include "../service/node/broadcast.hpp"

void Router::PostRoute(Node & nNode, const web::http::http_request hRequest, web::json::value const & jObject) {
    std::string sRequestPath = hRequest.relative_uri().path();
    boost::algorithm::replace_first(sRequestPath, API_PATH, "");
    if(sRequestPath.compare(service::node::Auth::PATH_AUTH) == 0){
        service::node::Auth::Authorize(hRequest, nNode, jObject);
    }
    if(sRequestPath.compare(service::node::Broadcast::PATH_NODE) == 0){
        service::node::Broadcast::AddNode(hRequest, nNode, jObject);
    }
    if(sRequestPath.compare(service::node::Broadcast::PATH_WALLET) == 0){
        service::node::Broadcast::AddWallet(hRequest, nNode, jObject);
    }
}
