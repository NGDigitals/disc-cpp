#ifndef WALLET_H
#define WALLET_H

#include <vector>
#include <iostream>
#include "block/transaction.hpp"

using std::string;

class Wallet {
private:
    string _sSymbol;
    string _sPublicKey;
    string _sPrivateKey;

    void _SaveKeys() const;
    void _LoadKeys() const;
    void _GenerateKeys() const;
public:
    bool SignTransaction(Transaction tTrans) const;
    bool VerifyTransaction(Transaction tTrans) const;
};

#endif //WALLET_H