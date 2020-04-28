#ifndef RSA_CRYPT_H
#define RSA_CRYPT_H

#include <string>
#include <vector>
#include <openssl/rsa.h>

class RSACrypt {
public:
    static std::vector<char*> CreateKeys();
    static RSA* ReadRSA(unsigned char *cKey, int nPublic);
    static int Encrypt(unsigned char * cData, int nDataLen, unsigned char *cKey, unsigned char *cOutput);
    static int Decrypt(unsigned char *cEncData, size_t nDataLen, unsigned char *cKey, unsigned char *cOutput);
    static bool SignData(RSA* rRsa, const unsigned char* cData, size_t nDataLen, unsigned char** cOutput, size_t* nOutputLen);
    static bool VerifySignature( RSA* rRsa, unsigned char* cHashData, size_t nHashLen, const char* cPlainData, 
                size_t nPlainLen, bool* bAuthentic);
    static void Base64Encode(const unsigned char* cData, size_t nDataLen, char** cOutput);
    static void Base64Decode(const char* b64Data, unsigned char** cOutput, size_t* nOutputLen);
    // static void WriteEncryptedFile(char* encrypted, RSA *key_pair);
    // static void WriteDecryptedFile(char* encrypted, RSA *key_pair);
    static void ShowError(std::string sMsg);

private:
};

#endif //CRYPT_RSA_H