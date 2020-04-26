#ifndef WALLET_H
#define WALLET_H

#include <iostream>

class Wallet {
    public:
    std::string _sSymbol;
    char * _cPublicKey;
    char * _cPrivateKey;

    Wallet(){}
    Wallet(std::string _sSymbol);
    //~Wallet(void);
private:
    void _GenerateKeys();
};

#endif //WALLET_H