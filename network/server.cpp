#include "server.hpp"

using namespace web::http::experimental::listener;

Server::Server(const Node & nNode) : _nNode(nNode){
    //_rRouter.nNode = nNode;
    http_listener listener (_nNode.GetAddress());
    listener.support(web::http::methods::GET, [this](web::http::http_request request){
        this->HandleGet(request);
    });
    listener.support(web::http::methods::POST, [this](web::http::http_request request){
        this->HandlePost(request);
    });
    try{
        listener
            .open()
            .then([&listener, &nNode]() {
                std::cout << std::endl << "Running on " << nNode.GetAddress() << std::endl; 
            }).wait();
        while (true);
    }catch (std::exception const & e){
        std::cout << e.what() << std::endl;
        exit(0);
    }
}

void Server::HandleGet(web::http::http_request hRequest) const{
    //hRequest.
    // auto answer = web::json::value::object();
    // for (auto const & p : _mDict){
    //     answer[p.first] = web::json::value::string(p.second);
    // }
    // DisplayJson(web::json::value::null(), "R: ");
    // DisplayJson(answer, "S: ");
    
    // hRequest.reply(web::http::status_codes::OK, answer);
}

// void Server::HandlePost(web::http::http_request hRequest){
//     const std::function<void(web::http::http_request &, web::json::value const &)> Route = 
//                 std::bind( &Router::PostRoute, &_rRouter, std::placeholders::_1, std::placeholders::_2 );
//     HandleRequest(hRequest, Route);
// }

void Server::HandlePost(web::http::http_request hRequest){
    const std::function<void(Node &, web::http::http_request &, web::json::value const &)> Handler = 
                std::bind(&Router::PostRoute, &_rRouter, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    HandleRequest(_nNode, hRequest, Handler);
}

void Server::HandleRequest(Node & nNode, web::http::http_request hRequest, 
                std::function<void(Node &, web::http::http_request &, web::json::value const &)> Handler){
    hRequest
        .extract_json()
        .then([&hRequest, &Handler, &nNode](pplx::task<web::json::value> task) {
            try{
                auto const & jObject = task.get();
                if (!jObject.is_null()){
                    Handler(nNode, hRequest, jObject);
                }
            }catch (web::http::http_exception const & e){
                std::cout << e.what() << std::endl;
            }
    }).wait();
    auto aResponse = web::json::value::object();
    hRequest.reply(web::http::status_codes::OK, aResponse);
}


// void Server::HandleRequest(web::http::http_request hRequest, 
//                 std::function<void(web::http::http_request &, web::json::value const &)> vFunc){
//     auto aAnswer = web::json::value::object();
//     hRequest
//         .extract_json()
//         .then([&aAnswer, &vFunc, &hRequest, this](pplx::task<web::json::value> task) {
//             try{
//                 auto const & jObject = task.get();
//                 if (!jObject.is_null()){
//                     vFunc(hRequest, jObject);
//                 }
//             }catch (web::http::http_exception const & e){
//                 std::cout << e.what() << std::endl;
//             }
//     }).wait();
//     hRequest.reply(web::http::status_codes::OK, aAnswer);
// }

void Server::DisplayJson(web::json::value const & jValue, utility::string_t const & sPrefix) const{
    std::cout << sPrefix << jValue.serialize() << std::endl;
}