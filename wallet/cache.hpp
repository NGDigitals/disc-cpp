#ifndef CACHE_H
#define CACHE_H

#include <iostream>

class Cache {
public:
    Cache(){};
    Cache(std::string sName);

    void GenerateKeys();
    std::string GetName() const;
    void SetName(const std::string sName);
    unsigned char* GetPublicKey() const;
    void SetPublicKey(unsigned char* sPublicKey);
    unsigned char* GetPrivateKey() const;
    void SetPrivateKey(unsigned char* sPrivateKey);

private:
    std::string _sName;
    unsigned char *_cPublicKey;
    unsigned char *_cPrivateKey;
};

#endif //CACHE_H