#ifndef NODE_H
#define NODE_H

#include <vector>
#include <fstream>
#include <iostream>

#include <cpprest/json.h>
#include <cpprest/http_client.h>
#pragma comment(lib, "cpprest_2_10")

#include "../block/wallet.hpp"

class Node {
public:
    Node() {};
    Node(const Node & nNode);
    Node(const std::string sName, const std::string sAddress, const std::string sToken);
    
    std::string GetName() const;
    std::string GetToken() const;
    std::string GetAddress() const;
    std::map<std::string, Node> GetPeers() const;
    std::map<unsigned char*, Wallet> GetWallets() const;

    void SetName(std::string sName);
    void SetToken(std::string sName);
    void SetAddress(std::string sName);
    
    Wallet CreateWallet();
    bool AddWallet(const Wallet wWallet);
    bool AddPeer(const Node& pNode);
    bool AuthorizeNode(const Node& nNode);

    static Node GetInstance(const std::string sName);
    static bool isIPv4(const std::string& sAddress);
    static bool isIPv6(const std::string& sAddress);

private:
    std::string _sName;
    std::string _sToken;
    std::string _sAddress;
    // std::string _sPeersPath;
    // std::string _sWalletsPath;

    char _cPeersFile[25];
    char _cWalletsFile[25];

    static const std::string MASTER_NAME;
    static const std::string PATH;
    static const std::string PATH_API;
    static const std::string PATH_NETWORK;
    static const std::string PATH_AUTH;
    static const std::string PATH_BROADCAST;

    static const char *_cPeersPath;
    static const char *_cWalletsPath;
    
    void _InitializeSetup();
    void _GenerateToken();
    void _BroadcastWallet(const web::json::value jWallet) const;
    void _BroadcastNode(const Node& nNode) const;
};

#endif //NODE_H
