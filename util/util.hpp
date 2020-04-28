#include <iostream>
#include <algorithm>
#include <openssl/sha.h>
#include <cpprest/json.h>

constexpr std::string_view HEX_CONSTANT {"0123456789abcdef"};

class Util{
public:
    static bool IsAlphaNum(const std::string &sString){
        return std::find_if_not(sString.begin(), sString.end(), [](char c) { 
            return isalnum(c); 
        }) == sString.end();
    }

    static std::string RandomString(const int nLen) {
        srand(time(NULL));
        static const char cAlphaNum[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
        std::stringstream sStream;
        for (int i = 0; i < nLen; ++i) {
            sStream << cAlphaNum[rand() % (sizeof(cAlphaNum) - 1)];
        }
        return sStream.str();
    }

    static std::string SHA256( const std::string sString ){
        unsigned char cHash[SHA256_DIGEST_LENGTH];
        SHA256_CTX sha256;
        SHA256_Init(&sha256);
        SHA256_Update(&sha256, sString.c_str(), sString.size());
        SHA256_Final(cHash, &sha256);
        std::stringstream sStream;
        for(int i = 0; i < SHA256_DIGEST_LENGTH; i++){
            sStream << std::hex << std::setw(2) << std::setfill('0') << (int)cHash[i];
        }
        return sStream.str();
    }

    static void SHA256String (unsigned char cHash[SHA256_DIGEST_LENGTH], char cOutput[65]){
        int i = 0;
        for(i = 0; i < SHA256_DIGEST_LENGTH; i++){
            sprintf(cOutput + (i * 2), "%02x", cHash[i]);
        }
        cOutput[64] = 0;
    }

    static int SHA256File(char* cPath, char cOutput[65]){
        FILE* file = fopen(cPath, "rb");
        if(!file) return -1;
        unsigned  char cHash[SHA256_DIGEST_LENGTH];
        SHA256_CTX sha256;
        SHA256_Init(&sha256);
        const int nBufSize = 32768;
        char* cBuffer = new char[ nBufSize ];
        int bytesRead = 0;
        if(!cBuffer) return -1;
        while((bytesRead = fread(cBuffer, 1, nBufSize, file))){
            SHA256_Update(&sha256, cBuffer, bytesRead);
        }
        SHA256_Final(cHash, &sha256);
        SHA256String(cHash, cOutput);
        fclose(file);
        delete [] cBuffer;
        return 0;
    }

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
};