// #include <std_micro_service.hpp>
// #include "test-service.hpp"

// using namespace web;
// using namespace http;

// void TestService::initRestOpHandlers() {
//     _listener.support(methods::GET, std::bind(&TestService::handleGet, this, std::placeholders::_1));
//     _listener.support(methods::PUT, std::bind(&TestService::handlePut, this, std::placeholders::_1));
//     _listener.support(methods::POST, std::bind(&TestService::handlePost, this, std::placeholders::_1));
//     _listener.support(methods::DEL, std::bind(&TestService::handleDelete, this, std::placeholders::_1));
//     _listener.support(methods::PATCH, std::bind(&TestService::handlePatch, this, std::placeholders::_1));
// }

// void TestService::handleGet(http_request message) {
//     auto path = requestPath(message);
//     if (!path.empty()) {
//         if (path[0] == "service" && path[1] == "test") {
//             auto response = json::value::object();
//             response["version"] = json::value::string("0.1.1");
//             response["status"] = json::value::string("ready!");
//             message.reply(status_codes::OK, response);
//         }
//     }
//     else {
//         message.reply(status_codes::NotFound);
//     }
// }

// void TestService::handlePatch(http_request message) {
//     message.reply(status_codes::NotImplemented, responseNotImpl(methods::PATCH));
// }

// void TestService::handlePut(http_request message) {
//     message.reply(status_codes::NotImplemented, responseNotImpl(methods::PUT));
// }

// void TestService::handlePost(http_request message) {
//     message.reply(status_codes::NotImplemented, responseNotImpl(methods::POST));
// }

// void TestService::handleDelete(http_request message) {    
//     message.reply(status_codes::NotImplemented, responseNotImpl(methods::DEL));
// }

// void TestService::handleHead(http_request message) {
//     message.reply(status_codes::NotImplemented, responseNotImpl(methods::HEAD));
// }

// void TestService::handleOptions(http_request message) {
//     message.reply(status_codes::NotImplemented, responseNotImpl(methods::OPTIONS));
// }

// void TestService::handleTrace(http_request message) {
//     message.reply(status_codes::NotImplemented, responseNotImpl(methods::TRCE));
// }

// void TestService::handleConnect(http_request message) {
//     message.reply(status_codes::NotImplemented, responseNotImpl(methods::CONNECT));
// }

// void TestService::handleMerge(http_request message) {
//     message.reply(status_codes::NotImplemented, responseNotImpl(methods::MERGE));
// }

// json::value TestService::responseNotImpl(const http::method & method) {
//     auto response = json::value::object();
//     response["serviceName"] = json::value::string("C++ Mircroservice Sample");
//     response["http_method"] = json::value::string(method);
//     return response ;
// }