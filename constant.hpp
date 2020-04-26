#ifndef CONSTANT_H
#define CONSTANT_H
 
#include <map>
#include <iostream>

namespace constant{
    enum CLIENT_OPTIONS{
        AUTH_NODE,
        CREATE_WALLET,
        BROADCAST_WALLET,
    };
    const std::map<int, std::string> CLIENT_OPTIONS_MAP = {
        {0, "Authenticate Node"},
        {1, "Create Wallet"},
        {2, "Broadcast Wallet"},
        {3, "Broadcast Transaction"},
    };
    inline constexpr int RSA_KEY_LENGTH = 2048;
    inline constexpr std::string_view MASTER_NAME { "MASTER" };
    // inline constexpr std::string_view STORAGE_DIR { "storage" };
    // inline constexpr std::string_view STORAGE_NODES { "storage/nodes" };
    // inline constexpr std::string_view STORAGE_WALLETS { "storage/wallets" };
    // inline constexpr std::string_view STORAGE_EXT { "txt" };
}

#endif