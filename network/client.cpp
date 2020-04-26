#include "client.hpp"
#include "../constant.hpp"

void Client::HandleCLI(Node &nNode, int nOption){
   switch (nOption){
      case constant::CLIENT_OPTIONS::AUTH_NODE:{
         std::cout << "Authorize Peer Node" << std::endl;
         std::string sName, sAddress, sToken;
         std::cout << "Enter Node Name: ";
         std::cin >> sName;
         std::cout << "Enter Node Address: ";
         std::cin >> sAddress;
         std::cout << "Enter Node Token: ";
         std::cin >> sToken;
         PeerNode pNode(sName, sAddress, sToken);
         nNode.AuthorizeNode(pNode);
      }
      break;
      case constant::CLIENT_OPTIONS::CREATE_WALLET:{
         nNode.CreateWallet();
      }
      break;
      default:
         if(nOption == -1)
            exit(0);
         else
            std::cout << "Invalid selection" << std::endl;
         break;
   }
}

void Client::HandleCLS(Node &nNode, int nOption, std::vector<std::string> vInput){
   switch (nOption){
   case constant::CLIENT_OPTIONS::AUTH_NODE:
      // PeerNode pNode()
      // nNode.AuthorizeNode();
      break;
   default:
         for ( const auto &aPair : constant::CLIENT_OPTIONS_MAP ) {
            std::cout << "Option : " << aPair.first << " - " << aPair.second << std::endl;
         }
         break;
   }
}

void Client::HandleRequest(web::http::client::http_client & hClient, 
               web::http::method mMethod, web::json::value const & jObject, std::function<void(web::json::value)> Callback){
   auto aRequestTask = pplx::create_task([&hClient, mMethod, jObject]() {
		return (mMethod == web::http::methods::GET || mMethod == web::http::methods::HEAD) ?
         hClient.request(mMethod, "") : hClient.request(mMethod, "", jObject);
	}).then([](web::http::http_response response){
      if (response.status_code() == web::http::status_codes::OK){
         return response.extract_json();
      }
      return pplx::task_from_result(web::json::value());
   }).then([Callback](pplx::task<web::json::value> jPrevTask){
      try{
         Client::DisplayJson(jPrevTask.get(), "R: ");
         Callback(jPrevTask.get());
      }catch (web::http::http_exception const & e){
         std::cout << e.what() << std::endl;
      }
   }).wait();
}

void Client::DisplayJson(web::json::value const & jObject, utility::string_t const & sPrefix){
   std::cout << sPrefix << jObject.serialize() << std::endl;
}