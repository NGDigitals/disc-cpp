#include <arpa/inet.h>
#include <boost/algorithm/string.hpp>

#include "node.hpp"
#include "client.hpp"
#include "../block/wallet.hpp"

// std::ifstream iFile(_sWalletsFilePath, std::ios::in);
// if(iFile.is_open()){
//     Wallet wWallet;
//     while( iFile >> wWallet){
//         _vWallets.emplace_back(wWallet);
//     }
//     iFile.close();
//     for ( const auto &wallet : _vWallets ) 
//         std::cout << "Wallet - " << wallet._sSymbol << std::endl;
// }

// const char * Node::_cInstancePath = "storage/%s.dat";
const char * Node::_cWalletsPath = "storage/wallets-%s.dat";
const char * Node::_cPeerNodesPath = "storage/nodes-%s.dat";

std::ostream& operator<< ( std::ostream& oStream, const Node& nNode ){  
    return oStream << nNode.GetName() << std::endl << nNode.GetAddress()  << std::endl << nNode.GetToken();
}

std::ostream& operator<< ( std::ostream& oStream, const Wallet& wWallet ){
    return oStream << wWallet._sSymbol << std::endl << wWallet._cPublicKey << std::endl << wWallet._cPrivateKey << std::endl << std::endl;
}

std::ostream& operator<< ( std::ostream& oStream, const PeerNode& pNode ){
    return oStream << pNode.sName << std::endl << pNode.sAddress  << std::endl << pNode.sToken << std::endl << std::endl;
}

std::istream& operator>> ( std::istream& iStream, Node& nNode ){
    std::string sName, sAddress, sToken;
    if( std::getline( iStream, sName ) && std::getline( iStream, sAddress) && std::getline( iStream, sToken)
        && (!sName.empty() && !sAddress.empty() && !sToken.empty())){
        nNode.SetName(sName);
        nNode.SetAddress(sAddress);
        nNode.SetToken(sToken);
    }else{
        nNode = {};
    }
    return iStream;
}

inline std::istream& operator>> ( std::istream& iStream, Wallet& wWallet ){
    // while( std::getline( iStream, wWallet._sSymbol ) && 
    //         (wWallet._sSymbol.empty() || !wWallet._cPublicKey || !wWallet._cPrivateKey))
    //     ;
    std::string sPublicKey, sPrivateKey;
    if( std::getline( iStream, wWallet._sSymbol ) && std::getline( iStream, sPublicKey) && 
            std::getline(iStream, sPrivateKey)){
        wWallet._cPublicKey = strdup(sPublicKey.c_str());
        wWallet._cPrivateKey = strdup(sPrivateKey.c_str());
    }else 
        wWallet = Wallet();
    return iStream;
}

std::istream& operator>> ( std::istream& iStream, PeerNode& pNode){
    while(std::getline( iStream, pNode.sName ) && std::getline( iStream, pNode.sAddress) && std::getline( iStream, pNode.sToken) && 
            (pNode.sName.empty() || !pNode.sAddress.empty() || !pNode.sToken.empty()))
        ;
    if(std::getline( iStream, pNode.sName ) && std::getline( iStream, pNode.sAddress) && std::getline( iStream, pNode.sToken))
        ;
    else
        pNode = {};
    return iStream;
}

// std::istream& operator>> ( std::istream& iStream, PeerNode& pNode ){
//     while( std::getline( iStream, pNode.sName ) && (pNode.sName.empty() || pNode.sAddress.empty() || pNode.sToken.empty()))
//         ;
//     if( std::getline( iStream, pNode.sName ) && std::getline( iStream, pNode.sAddress) && std::getline( iStream, pNode.sToken))
//         ;
//     else 
//         pNode = {};
//     return iStream;
// }

Node::Node(const Node & nNode){
    _sName = nNode._sName;
    _sToken = nNode._sToken;
    _sAddress = nNode._sAddress;
}

Node::Node(const std::string sName, const std::string sAddress, const std::string sToken){
    Node nNode = Node::GetInstance(sName);
    if(!nNode._sName.empty()){
        *this = nNode;
        std::cout << "Existing Node Token..." << _sToken << std::endl;
    }else{
        _sName = sName;
        _sToken = sToken;
        _sAddress = sAddress;
        std::cout << "New Node Token..." << _sToken << std::endl;
        _CreatePeerNodes();
        //_SaveInstance();
        //_InitializeSetup();
    }
}

Node Node::GetInstance(std::string sName){
    char cFileName[25];
    sprintf(cFileName, Node::_cPeerNodesPath, sName.c_str());
    std::ifstream iFile(cFileName, std::ios::in);
    Node nNode;
    if(iFile.is_open()){
        while(!iFile.eof()){
            iFile >> nNode;
            if(sName.compare(nNode._sName)==0)
                break;
        }
    }
    iFile.close();
    return nNode;
}

void Node::_CreatePeerNodes(){
    PeerNode pNode(_sName, _sAddress, _sToken);
    UpdatePeerNodes(pNode);
}

bool Node::UpdatePeerNodes(const PeerNode pNode){
    sprintf(_cPeerNodesFile, Node::_cPeerNodesPath, _sName.c_str());
    std::ofstream oFile(_cPeerNodesFile, std::ios::app);
    bool updated = false;
    if(oFile.is_open()){
        _mPeerNodes[pNode.sName] = pNode;
        for ( const auto &[pnKey, pnValue] : _mPeerNodes ) oFile << pnValue;
        updated = true;
    }
    oFile.close();
    return updated;
}

// void Node::_SaveInstance(){
//     std::ofstream oFile(_cInstanceFile, std::ios::out);
//     if(oFile.is_open()) oFile << Node(*this);
//     oFile.close();
// }

// void Node::_InitializeSetup(){
//     CreateWallet();
//     //_CreatePeerNodes();
//     // if(_sName == "MASTER"){
//     //     PeerNode pNode("BISC-EPL", "http://0.0.0.0:5001", "FwzrFD9A6S");
//     //     _AuthorizeNode(pNode);
//     //     PeerNode pNode1("BISC-UCL", "http://0.0.0.0:5002", "UJEv9XH4B6");
//     //     _AuthorizeNode(pNode1);
//     // }
// }

void Node::CreateWallet(){
    _wWallet = Wallet(_sName);
    UpdateWallets(_wWallet);
}

bool Node::UpdateWallets(const Wallet wWallet){
    sprintf(_cWalletsFile, Node::_cWalletsPath, _sName.c_str());
    std::ofstream oFile(_cWalletsFile, std::ios::app);
    bool updated = false;
    if(oFile.is_open()){
        oFile << wWallet;
        _vWallets.emplace_back(wWallet);
        updated = true;
    }
    oFile.close();
    return updated;
}

// void Node::_BroadcastWallet(const web::json::value jWallet) const{
//     for(const auto [pKey, pValue] : _mPeerNodes){
//         if(_sName.compare(pKey) != 0 && _sAddress.compare(pValue.sAddress)){
//             web::http::client::http_client client(pValue.sAddress + 
//                         node::constant::NETWORK_BROADCAST_PATH.begin() + "/wallets");
//             web::json::value jObject;
//             jObject["_token"] = web::json::value::string(_sToken);
//             jObject["wallet"] = jWallet;
//             // _cClient.MakeRequest(client, web::http::methods::POST, jObject, 
//             //     [this](web::json::value jResponse){
//             //         std::cout << jResponse.at("message")  << std::endl;
//             //     }
//             // );
//         }
//     }
// }

// void Node::_BroadcastPeerNode(PeerNode pNode) const{
//     for(const auto [pKey, pValue] : _mPeerNodes){
//         if(_sName.compare(pKey) != 0 && _sAddress.compare(pValue.sAddress)){
//             web::http::client::http_client client(pValue.sAddress + 
//                         node::constant::NETWORK_BROADCAST_PATH.begin() + "/peers");
//             web::json::value jNode;
//             jNode["name"] = web::json::value::string(pNode.sName);
//             jNode["address"] = web::json::value::string(pNode.sAddress);
//             jNode["token"] = web::json::value::string(pNode.sToken);
//             web::json::value jObject;
//             jObject["_token"] = web::json::value::string(_sToken);
//             jObject["node"] = web::json::value::array({jNode});
//             // _cClient.MakeRequest(client, web::http::methods::POST, jObject, 
//             //     [this](web::json::value jResponse){
//             //         std::cout << jResponse.at("message")  << std::endl;
//             //     }
//             // );
//         }
//     }
// }

bool Node::AuthorizeNode(const PeerNode pNode){
    web::http::client::http_client client(pNode.sAddress + 
                        node::constant::NETWORK_AUTH_PATH.begin());
    web::json::value jObject;
    jObject["name"] = web::json::value::string(pNode.sName);
    jObject["token"] = web::json::value::string(pNode.sToken);
    Client::HandleRequest(client, web::http::methods::POST, jObject, 
        [this, pNode](web::json::value jResponse){  
            std::cout << "Authorization returned..."  << std::endl;
            bool bSuccess = jResponse.at("success").as_bool();
            std::cout << "Authorization returned...1"  << std::endl;
            if(bSuccess){
                std::cout << "Authorization returned...2"  << std::endl;
                std::cout << jResponse.at("message")  << std::endl;
                std::cout << "Authorization returned...3"  << std::endl;
                // web::json::value jWallet = jResponse.at("wallet");
                // Wallet wWallet(jWallet[0].at("symbol").as_string());
                // strcpy(wWallet._cPublicKey, jWallet[0].at("public_key").as_string().c_str());
                // strcpy(wWallet._cPrivateKey, jWallet[0].at("private_key").as_string().c_str());
                //std::vector<Wallet> vWallets(this->_vWallets); //Create copy of wallets
                //std::map<std::string, PeerNode> mPeerNodes(this->_mPeerNodes); //Create copy of peer nodes
                //vWallets.emplace_back(wWallet); //Update wallets copy
                //mPeerNodes[pNode.sName] = PeerNode(pNode.sName, pNode.sAddress, pNode.sToken); //Update peer nodes copy
                //Update wallets and peer nodes with copies
                // if(this->UpdateWallets(wWallet) && this->UpdatePeerNodes(pNode)){
                //     //this->_BroadcastWallet(jWallet[0]);
                //     //this->_BroadcastPeerNode(pNode);
                // }else{
                    // std::cout << "An error occurred while updating node data" << std::endl;
                // }
            }else{
                std::cout << "Authorization returned...4"  << std::endl;
                std::cout << jResponse.at("message")  << std::endl;
                std::cout << "Authorization returned...5"  << std::endl;
            }
        }
    );
    return false;
}

std::string Node::GetName() const{
    return _sName;
}

std::string Node::GetToken() const{
    return _sToken;
}

std::string Node::GetAddress() const{
    return _sAddress;
}

Wallet Node::GetWallet() const{
    return _wWallet;
}

void Node::SetName(std::string sName){
    _sName = sName;
}

void Node::SetToken(std::string sToken){
    _sToken = sToken;
}

void Node::SetAddress(std::string sAddress){
    _sAddress = sAddress;
}

bool Node::isIPv4(const std::string& sAddress){
    struct sockaddr_in sinAddress;
    int nResult = inet_pton(AF_INET, sAddress.c_str(), &(sinAddress. sin_addr)) != 0;
    return nResult != 0;
}

bool Node::isIPv6(const std::string& sAddress){
    struct sockaddr_in6 sinAddress; 
    int nResult =  inet_pton(AF_INET6, sAddress.c_str(), &(sinAddress.sin6_addr));
    return nResult != 0;
}