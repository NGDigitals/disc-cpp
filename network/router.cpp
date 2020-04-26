#include <boost/algorithm/string/replace.hpp>

#include "router.hpp"
#include "../service/node/auth.hpp"
#include "../service/node/broadcast.hpp"

void Router::PostRoute(Node & nNode, const web::http::http_request hRequest, web::json::value const & jObject) {
    std::string sRequestPath = hRequest.relative_uri().path();
    boost::algorithm::replace_first(sRequestPath, API_PATH, "");
    std::cout << "Posting request" << std::endl;
    if(sRequestPath.compare(service::node::Auth::PATH) == 0){
        service::node::Auth::Authorize(hRequest, nNode, jObject);
    }
    if(sRequestPath.compare(service::node::Broadcast::WALLET_PATH) == 0){
        service::node::Broadcast::SendWallet(hRequest, nNode, jObject);
    }
}
