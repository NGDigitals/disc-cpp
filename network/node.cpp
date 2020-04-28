#include <arpa/inet.h>
#include <boost/algorithm/string.hpp>

#include "node.hpp"
#include "client.hpp"
#include "../block/wallet.hpp"

const std::string Node::MASTER_NAME = "MASTER";

const std::string Node::PATH = "/network/auth";
const std::string Node::PATH_API = "/api/v1";
const std::string Node::PATH_NETWORK = "/network";
const std::string Node::PATH_AUTH = "/network/auth";
const std::string Node::PATH_BROADCAST = "/network/broadcast";

const char * Node::_cPeersPath = "storage/peers-%s.dat";
const char * Node::_cWalletsPath = "storage/wallets-%s.dat";

std::ostream& operator<< ( std::ostream& oStream, const Node& nNode ){  
    return oStream << nNode.GetName() << std::endl << nNode.GetAddress()  << std::endl << nNode.GetToken();
}

std::ostream& operator<< ( std::ostream& oStream, const Wallet& wWallet ){
    return oStream << wWallet.GetPublicKey() << std::endl << wWallet.GetPrivateKey() << std::endl << std::endl;
}

// std::ostream& operator<< ( std::ostream& oStream, const PeerNode& pNode ){
//     return oStream << pNode.nNode.GetName() << std::endl << pNode.nNode.GetAddress()  << std::endl << pNode.nNode.GetToken() << std::endl << std::endl;
// }

std::istream& operator>> ( std::istream& iStream, Node& nNode ){
    std::string sName, sAddress, sToken;
    while(std::getline( iStream, sName) && std::getline(iStream, sAddress) && std::getline(iStream, sToken) && 
            (sName.empty() || sAddress.empty() || sToken.empty()))
        ;
    if(std::getline( iStream, sName ) && std::getline( iStream, sAddress) && std::getline( iStream, sToken)){
        nNode.SetName(sName);
        nNode.SetAddress(sAddress);
        nNode.SetToken(sToken);
    }else
        nNode = {};
    return iStream;
}

inline std::istream& operator>> ( std::istream& iStream, Wallet& wWallet ){
    std::string sPublicKey, sPrivateKey;
    while( std::getline( iStream, sPublicKey ) && std::getline( iStream, sPublicKey ) && 
            (sPublicKey.empty() || sPrivateKey.empty()))
        ;
    if( std::getline( iStream, sPublicKey) && std::getline(iStream, sPrivateKey)){
        wWallet.SetPublicKey((unsigned char*)strdup(sPublicKey.c_str()));
        wWallet.SetPrivateKey((unsigned char*)strdup(sPrivateKey.c_str()));
    }else 
        wWallet = {};
    return iStream;
}

// std::istream& operator>> ( std::istream& iStream, PeerNode& pNode){
//     std::string sName, sAddress, sToken;
//     while(std::getline( iStream, sName) && std::getline(iStream, sAddress) && std::getline(iStream, sToken) && 
//             (sName.empty() || sAddress.empty() || sToken.empty()))
//         ;
//     if(std::getline( iStream, sName ) && std::getline( iStream, sAddress) && std::getline( iStream, sToken)){
//         pNode.nNode.SetName(sName);
//         pNode.nNode.SetAddress(sAddress);
//         pNode.nNode.SetToken(sToken);
//     }else
//         pNode = {};
//     return iStream;
// }

Node Node::GetInstance(const std::string sName){
    char cFileName[25];
    sprintf(cFileName, Node::_cPeersPath, sName.c_str());
    std::ifstream iFile(cFileName, std::ios::in);
    Node nNode;
    if(iFile.is_open()){
        while(!iFile.eof()){
            iFile >> nNode;
            if(sName.compare(nNode.GetName())==0)
                break;
        }
    }
    iFile.close();
    return nNode;
}

Node::Node(const Node & nNode){
    _sName = nNode.GetName();
    _sToken = nNode.GetToken();
    _sAddress = nNode.GetAddress();
    sprintf(_cPeersFile, Node::_cPeersPath, _sName.c_str());
    sprintf(_cWalletsFile, Node::_cWalletsPath, _sName.c_str());
}

Node::Node(const std::string sName, const std::string sAddress, const std::string sToken){
    Node nNode = Node::GetInstance(sName);
    sprintf(_cPeersFile, Node::_cPeersPath, sName.c_str());
    sprintf(_cWalletsFile, Node::_cWalletsPath, sName.c_str());
    if(!nNode.GetName().empty()){
        *this = nNode;
        std::cout << "Existing Node Token..." << _sToken << std::endl;
    }else{
        _sName = sName;
        _sToken = sToken;
        _sAddress = sAddress;
        std::cout << "New Node Token..." << _sToken << std::endl;
        AddPeer(*this);
    }
}

std::map<std::string, Node> Node::GetPeers() const{
    std::ifstream iFile(_cPeersFile, std::ios::in);
    std::map<std::string, Node> mPeers;
    if(iFile.is_open()){
        Node nNode;
        while(!iFile.eof()){
            iFile >> nNode;
            if(!nNode.GetName().empty() && !nNode.GetAddress().empty() && !nNode.GetToken().empty())
                mPeers[nNode.GetName()] = nNode; 
        }
    }
    iFile.close();
    return mPeers;
}

bool Node::AddPeer(const Node& nNode){
    bool updated = false;
    std::ofstream oFile(_cPeersFile, std::ios::out);
    if(oFile.is_open()){
        std::map<std::string, Node> mPeers = GetPeers();
        mPeers[nNode.GetName()] = nNode;
        for ( const auto &[aKey, aValue] : mPeers){
            oFile << aValue;
        }
        updated = true;
    }
    oFile.close();
    return updated;
}


Wallet Node::CreateWallet(){
    Wallet wWallet;
    wWallet.GenerateKeys();
    AddWallet(wWallet);
    return wWallet;
}

bool Node::AddWallet(const Wallet wWallet){
    std::ofstream oFile(_cWalletsFile, std::ios::out);
    bool updated = false;
    if(oFile.is_open()){
        std::map<unsigned char*, Wallet> mWallets = GetWallets();
        mWallets[wWallet.GetPublicKey()] = wWallet;
        for ( const auto &[pnKey, pnValue] : mWallets ) oFile << pnValue;
        updated = true;
    }
    oFile.close();
    return updated;
}

std::map<unsigned char*, Wallet> Node::GetWallets() const{
    std::ifstream iFile(_cWalletsFile, std::ios::in);
    std::map<unsigned char*, Wallet> mWallets;
    if(iFile.is_open()){
        Wallet wWallet;
        while(!iFile.eof()){
            iFile >> wWallet;
            if(strlen((char*)wWallet.GetPublicKey()) == 0 && strlen((char*)wWallet.GetPrivateKey()) == 0)
                mWallets[wWallet.GetPublicKey()] = wWallet; 
        }
    }
    iFile.close();
    return mWallets;
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

void Node::_BroadcastNode(const Node& nNode) const{
    std::map<std::string, Node> mPeers = GetPeers();
    for(const auto [pKey, pValue] : mPeers){
        if(_sName.compare(pKey) != 0 && _sAddress.compare(pValue.GetAddress())){
            std::cout << "Broadcasting..." <<  pValue.GetAddress() << std::endl;
            web::http::client::http_client client(pValue.GetAddress() + 
                        Node::PATH_BROADCAST + "/peers");
            web::json::value jNode;
            jNode["name"] = web::json::value::string(nNode.GetName());
            jNode["address"] = web::json::value::string(nNode.GetAddress());
            jNode["token"] = web::json::value::string(nNode.GetToken());
            web::json::value jObject;
            jObject["_token"] = web::json::value::string(_sToken);
            jObject["node"] = web::json::value::array({jNode});
            // _cClient.MakeRequest(client, web::http::methods::POST, jObject, 
            //     [this](web::json::value jResponse){
            //         std::cout << jResponse.at("message")  << std::endl;
            //     }
            // );
        }
    }
}

bool Node::AuthorizeNode(const Node& nNode){
    web::http::client::http_client client(nNode.GetAddress() + 
                        Node::PATH_AUTH);
    web::json::value jObject;
    jObject["name"] = web::json::value::string(nNode.GetName());
    jObject["token"] = web::json::value::string(nNode.GetToken());
    Client::HandleRequest(client, web::http::methods::POST, jObject, 
        [this, nNode](web::json::value jResponse){  
            bool bSuccess = jResponse.at("success").as_bool();
            if(bSuccess){
                std::cout << jResponse.at("message")  << std::endl;
                web::json::value jWallet = jResponse.at("wallet");
                Wallet wWallet;
                wWallet.SetPublicKey((unsigned char*)jWallet[0].at("public_key").as_string().c_str());
                wWallet.SetPrivateKey((unsigned char*)jWallet[0].at("public_key").as_string().c_str());
                std::map<std::string, Node> mPeers = this->GetPeers();
                std::map<unsigned char*, Wallet> mWallets = this->GetWallets();
                mPeers[nNode.GetName()] = nNode;
                mWallets[wWallet.GetPublicKey()] = wWallet;
                if(this->AddWallet(wWallet) && this->AddPeer(nNode)){
                    std::cout << "Broadcasting..."  << std::endl;
                    // this->_BroadcastWallet(jWallet[0]);
                    // this->_BroadcastPeerNode(nNode);
                }else{
                    std::cout << "An error occurred while updating node data" << std::endl;
                }
            }else{
                std::cout << jResponse.at("message")  << std::endl;
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