#include <iostream>
#include <stdlib.h>
#include <openssl/ssl.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/sha.h>

#include "rsa-crypt.hpp"

const int RSA_KEY_LENGTH = 1024;

size_t DecodeLength(const char* b64input) {
    size_t len = strlen(b64input), padding = 0;
    if (b64input[len-1] == '=' && b64input[len-2] == '=') //last two chars are =
        padding = 2;
    else if (b64input[len-1] == '=') //last char is =
        padding = 1;
    return (len*3)/4 - padding;
}

std::vector<char*> RSACrypt::CreateKeys() {
	int nResult;
    size_t sPbkLen = 0, sPvkLen = 0;
    char *cPublicKey, *cPrivateKey;
    RSA *rRSA = NULL;
    BIGNUM *bBigNum = NULL;
    BIO *bPublic = NULL, *bPrivate = NULL;

    bBigNum = BN_new();
	if(BN_set_word(bBigNum, RSA_F4) != 1)
		goto FreeAll;

    rRSA = RSA_new();
	if(RSA_generate_key_ex(rRSA, RSA_KEY_LENGTH, bBigNum, NULL) != 1)
		goto FreeAll;
	
    bPublic = BIO_new(BIO_s_mem());
    bPrivate = BIO_new(BIO_s_mem());
    if(PEM_write_bio_RSAPublicKey(bPublic, rRSA) != 1)
        goto FreeAll;
    if(PEM_write_bio_RSAPrivateKey(bPrivate, rRSA, NULL, NULL, 0, NULL, NULL) != 1)
        goto FreeAll;
    
    sPbkLen = BIO_pending(bPublic);
    sPvkLen = BIO_pending(bPrivate);

    cPublicKey = (char*) malloc(sPbkLen + 1);
    cPrivateKey = (char*) malloc(sPvkLen + 1);
    
    BIO_read(bPublic, cPublicKey, sPbkLen);
    BIO_read(bPrivate, cPrivateKey, sPvkLen);
    
    cPublicKey[sPbkLen] = '\0';
    cPrivateKey[sPvkLen] = '\0';
    
    FreeAll:
        BIO_free_all(bPublic);
        BIO_free_all(bPrivate);
        RSA_free(rRSA);
        BN_free(bBigNum);
    
    return {cPublicKey, cPrivateKey};
}

RSA *RSACrypt::ReadRSA(unsigned char *cKey, int nPubKey){
    RSA *rRsa= NULL;
    BIO *bBio;
    bBio = BIO_new_mem_buf(cKey, -1);
    if (bBio == NULL){
        std::cerr << "Failed to create key BIO" << std::endl;
        return 0;
    }
    if(nPubKey)
        rRsa = PEM_read_bio_RSAPublicKey(bBio, &rRsa,NULL, NULL);
    else
        rRsa = PEM_read_bio_RSAPrivateKey(bBio, &rRsa,NULL, NULL);
    return rRsa;
}

int RSACrypt::Encrypt(unsigned char *cData, int nDataLen, unsigned char *cKey, unsigned char *cOutput){
    RSA *rRsa = ReadRSA(cKey, 1);
    int nResult = RSA_public_encrypt(nDataLen, cData, cOutput, rRsa, RSA_PKCS1_PADDING);
    return nResult;
}

int RSACrypt::Decrypt(unsigned char *cEncData, size_t nDataLen, unsigned char *cKey, unsigned char *cOutput){
    RSA * rRsa = ReadRSA(cKey, 0);
    int  nResult = RSA_private_decrypt(nDataLen, cEncData, cOutput, rRsa, RSA_PKCS1_PADDING);
    return nResult;
}
 
bool RSACrypt::SignData(RSA* rRsa, const unsigned char* cData, size_t nDataLen, unsigned char** cOutput, size_t* nOutputLen) {
    EVP_MD_CTX* evpCtx = EVP_MD_CTX_create();
    EVP_PKEY* evpKey  = EVP_PKEY_new();
    EVP_PKEY_assign_RSA(evpKey, rRsa);
    if (EVP_DigestSignInit(evpCtx, NULL, EVP_sha256(), NULL, evpKey) <= 0) 
        return false;
    if (EVP_DigestSignUpdate(evpCtx, cData, nDataLen) <= 0) 
        return false;
    if (EVP_DigestSignFinal(evpCtx, NULL, nOutputLen) <= 0) 
        return false;
    
    *cOutput = (unsigned char*) malloc(*nOutputLen);
    if (EVP_DigestSignFinal(evpCtx, *cOutput, nOutputLen) <= 0) 
        return false;
    EVP_MD_CTX_cleanup(evpCtx);
    return true;
}

void RSACrypt::Base64Encode(const unsigned char* cData, size_t nDataLen, char** cOutput) { 
    BIO *bBio, *b64;
    BUF_MEM *bufMem;

    b64 = BIO_new(BIO_f_base64());
    bBio = BIO_new(BIO_s_mem());
    bBio = BIO_push(b64, bBio);

    BIO_write(bBio, cData, nDataLen);
    BIO_flush(bBio);
    BIO_get_mem_ptr(bBio, &bufMem);
    BIO_set_close(bBio, BIO_NOCLOSE);
    BIO_free_all(bBio);
    *cOutput = (*bufMem).data;
}

void RSACrypt::Base64Decode(const char* b64Data, unsigned char** cOutput, size_t* nOutputLen) {
    BIO *bBio, *b64;
    int nInputLen = DecodeLength(b64Data);
    *cOutput = (unsigned char*)malloc(nInputLen + 1);
    (*cOutput)[nInputLen] = '\0';

    bBio = BIO_new_mem_buf(b64Data, -1);
    b64 = BIO_new(BIO_f_base64());
    bBio = BIO_push(b64, bBio);

    *nOutputLen = BIO_read(bBio, *cOutput, strlen(b64Data));
    BIO_free_all(bBio);
}

bool RSACrypt::VerifySignature( RSA* rRsa, unsigned char* cHashData, size_t nHashLen, const char* cPlainData, 
                size_t nPlainLen, bool* bAuthentic) {
    *bAuthentic = false;
    EVP_PKEY* evPubKey  = EVP_PKEY_new();
    EVP_PKEY_assign_RSA(evPubKey, rRsa);
    EVP_MD_CTX* evpCtx = EVP_MD_CTX_create();
    if (EVP_DigestVerifyInit(evpCtx,NULL, EVP_sha256(),NULL, evPubKey)<=0)
        return false;
    if (EVP_DigestVerifyUpdate(evpCtx, cPlainData, nPlainLen) <= 0) {
        return false;
    }
    int AuthStatus = EVP_DigestVerifyFinal(evpCtx, cHashData, nHashLen);
    if (AuthStatus == 1) {
        *bAuthentic = true;
        EVP_MD_CTX_cleanup(evpCtx);
        return true;
    }else if(AuthStatus == 0){
        *bAuthentic = false;
        EVP_MD_CTX_cleanup(evpCtx);
        return true;
    } else{
        *bAuthentic = false;
        EVP_MD_CTX_cleanup(evpCtx);
        return false;
    }
}
// void RSACrypt::WriteEncryptedFile(char* cData, RSA *rRSA) {
//     FILE* file = fopen("encrypted_file.bin", "w");
//     fwrite(cData, sizeof(*cData), RSA_size(rRSA), file);
//     fclose(file);
// }

// void RSACrypt::WriteDecryptedFile(char* cData, RSA *rRSA) {
//     FILE *file = fopen("decrypted_file.txt", "w");
//     // fwrite(decrypt, sizeof(*decrypt), decrypt_length - 1, decrypted_file);
//     fclose(file);
// }

void RSACrypt::ShowError(std::string sMsg){
    char *cError = (char *) malloc(130);
    ERR_load_crypto_strings();
    ERR_error_string(ERR_get_error(), cError);
    std::cerr << sMsg << " ERROR: " << std::endl << cError << std::endl;
    free(cError);
}