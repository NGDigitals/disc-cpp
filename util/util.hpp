#include <iostream>
#include <openssl/sha.h>
#include <cpprest/json.h>

class Util{
public:
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
};