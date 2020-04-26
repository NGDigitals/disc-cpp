#include <fstream>
#include <iostream>
#include <openssl/ssl.h>
#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/pem.h>

#include "wallet.hpp"

const int RSA_KEY_LENGTH {1024};
constexpr std::string_view HEX_CONSTANT {"0123456789abcdef"};

char * Bin2Hex(const unsigned char *cInput, size_t sInputLen){
	char *cOutput;
	if (cInput == NULL || sInputLen == 0) return NULL;
	cOutput = (char *) malloc((sInputLen*2) +1);
    srand((unsigned) time(0));
    int result = 1 + (rand() % 2);
	for (size_t i = 0; i < sInputLen; i++) {
		cOutput[i*2] = HEX_CONSTANT[cInput[i] >> 4];
		cOutput[i*2+1] = HEX_CONSTANT[cInput[i] & 0x0F];
	}
	cOutput[sInputLen*2] = '\0';
	return cOutput;
}

int HexChar2Bin(const char cHex, char * cOutput){
	if (cOutput == NULL) return 0;
	if (cHex >= '0' && cHex <= '9') {
		*cOutput = cHex - '0';
	} else if (cHex >= 'A' && cHex <= 'F') {
		*cOutput = cHex - 'A' + 10;
	} else if (cHex >= 'a' && cHex <= 'f') {
		*cOutput = cHex - 'a' + 10;
	} else
		return 0;
	return 1;
}

size_t Hex2Bin(const char *cHex, unsigned char **cOutput){
	if (cHex == NULL || *cHex == '\0' || cOutput == NULL) return 0;
	size_t sLen = strlen(cHex);
	if (sLen % 2 != 0) return 0;
	sLen /= 2;
	*cOutput = (unsigned char *) malloc(sLen);
	memset(*cOutput, 'A', sLen);
    char cBin1, cBin2;
	for (size_t i = 0; i < sLen; i++) {
		if (!HexChar2Bin(cHex[i * 2], &cBin1) || !HexChar2Bin(cHex[(i * 2) +1], &cBin2)) {
			return 0;
		}
		(*cOutput)[i] = (cBin1 << 4) | cBin2;
	}
	return sLen;
}
//  int padding = RSA_PKCS1_PADDING;
 
// int public_encrypt(unsigned char * data,int data_len,unsigned char * key, unsigned char *encrypted)
// {
//     RSA * rsa = createRSA(key,1);
//     int result = RSA_public_encrypt(data_len,data,encrypted,rsa,padding);
//     return result;
// }
// int private_decrypt(unsigned char * enc_data,int data_len,unsigned char * key, unsigned char *decrypted)
// {
//     RSA * rsa = createRSA(key,0);
//     int  result = RSA_private_decrypt(data_len,enc_data,decrypted,rsa,padding);
//     return result;
// }
 
 
// int private_encrypt(unsigned char * data,int data_len,unsigned char * key, unsigned char *encrypted)
// {
//     RSA * rsa = createRSA(key,0);
//     int result = RSA_private_encrypt(data_len,data,encrypted,rsa,padding);
//     return result;
// }
// int public_decrypt(unsigned char * enc_data,int data_len,unsigned char * key, unsigned char *decrypted)
// {
//     RSA * rsa = createRSA(key,1);
//     int  result = RSA_public_decrypt(data_len,enc_data,decrypted,rsa,padding);
//     return result;
// }
 
// void printLastError(char *msg)
// {
//     char * err = malloc(130);;
//     ERR_load_crypto_strings();
//     ERR_error_string(ERR_get_error(), err);
//     printf("%s ERROR: %s\n",msg, err);
//     free(err);
// }
Wallet::Wallet(std::string sSymbol) : _sSymbol(sSymbol){
    _GenerateKeys();
}

void Wallet::_GenerateKeys() {
    size_t binlen = 0;
    size_t sPbkLen = 0, sPvkLen = 0;
    RSA *rRSA = NULL;
    BIGNUM *bBigNum = NULL;
    BIO *bPublic = NULL, *bPrivate = NULL;
    char *cPublicKey = NULL, *cPrivateKey = NULL;

    unsigned char *bin;

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
    _cPublicKey = Bin2Hex((unsigned char *)cPublicKey, strlen(cPublicKey));
    _cPrivateKey = Bin2Hex((unsigned char *)cPrivateKey, strlen(cPrivateKey));
	//binlen = Hex2Bin(_cPublicKey, &bin); 

    FreeAll:
        BIO_free_all(bPublic);
        BIO_free_all(bPrivate);
        RSA_free(rRSA);
        BN_free(bBigNum);
}

// Wallet::~Wallet(void) {
//     // free(_cPublicKey);
//     // free(_cPrivateKey);
// }
