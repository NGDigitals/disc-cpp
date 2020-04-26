#ifndef NODE_CONST
#define NODE_CONST

namespace node::constant {
    constexpr std::string_view MASTER_NAME { "MASTER" };
    constexpr std::string_view NETWORK_PATH { "/network" };
    constexpr std::string_view NETWORK_AUTH_PATH { "/network/auth" };
    constexpr std::string_view NETWORK_BROADCAST_PATH { "/network/broadcast" };
    constexpr std::string_view API_PATH { "/api/v1" };
}

#endif //NODE_CONST

#ifndef NODE_H
#define NODE_H

#include <vector>
#include <fstream>
#include <iostream>

#include <cpprest/json.h>
#include <cpprest/http_client.h>
#pragma comment(lib, "cpprest_2_10")

#include "../block/wallet.hpp"

struct PeerNode {
    std::string sToken;
    std::string sName;
    std::string sAddress;
    PeerNode(){}
    PeerNode(std::string sName, std::string sAddress, std::string sToken) : 
                sName(sName), sAddress(sAddress), sToken(sToken){}
};

class Node {
public:
    Node() {};
    Node(const Node & nNode);
    Node(const std::string sName, const std::string sAddress, const std::string sToken);
    std::string GetName() const;
    std::string GetToken() const;
    std::string GetAddress() const;
    void SetName(std::string sName);
    void SetToken(std::string sName);
    void SetAddress(std::string sName);
    Wallet GetWallet() const;
    void CreateWallet();
    bool UpdateWallets(const Wallet wWallet);
    bool UpdatePeerNodes(const PeerNode pNodes);
    bool AuthorizeNode(const PeerNode pNode);

    static Node GetInstance(std::string sName);
    static bool isIPv4(const std::string& sAddress);
    static bool isIPv6(const std::string& sAddress);

private:
    // char _cInstanceFile[25];
    char _cWalletsFile[25];
    char _cPeerNodesFile[25];

    // const static char *_cInstancePath;
    static const char *_cWalletsPath;
    static const char *_cPeerNodesPath;

    Wallet _wWallet;
    //Client _cClient;
    std::string _sName;
    std::string _sToken;
    std::string _sAddress;
    std::string _sNodeFilePath;
    std::string _sPeerFilePath;
    std::string _sWalletsFilePath;
    std::vector<Wallet> _vWallets;
    std::map<std::string, PeerNode> _mPeerNodes;

    void _InitializeSetup();
    // void _SaveInstance();
    void _CreatePeerNodes();
    void _GenerateToken();
    void _BroadcastWallet(const web::json::value jWallet) const;
    void _BroadcastPeerNode(PeerNode pNode) const;
};

#endif //NODE_H
