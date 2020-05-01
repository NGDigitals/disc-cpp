#ifndef WALLET_H
#define WALLET_H

#include <iostream>

class Wallet {
public:
    Wallet(){};
    void GenerateKeys();
    char* CreateAddress() const;
    int VerifyAddress(char* base64Address) const;
    unsigned char* GetPublicKey() const;
    void SetPublicKey(unsigned char* sPublicKey);
    unsigned char* GetPrivateKey() const;
    void SetPrivateKey(unsigned char* sPrivateKey);

private:
    unsigned char *_cPublicKey;
    unsigned char *_cPrivateKey;
    unsigned char *_cAddress;
};

#endif //WALLET_H