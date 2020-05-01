#include "wallet.hpp"
#include "../util/rsa-crypt.hpp"

void Wallet::GenerateKeys(){
	std::vector<char*> vKeys = RSACrypt::CreateKeys();
	_cPublicKey = (unsigned char*)vKeys[0];
	_cPrivateKey = (unsigned char*)vKeys[1];
}

char* Wallet::CreateAddress() const{
	RSA* rRsa = RSACrypt::ReadRSA(_cPrivateKey, 0);
	unsigned char* cEncodedAdd;
	size_t nEncodedLen;
	RSACrypt::SignData(rRsa, _cPublicKey, strlen((char*)_cPublicKey), &cEncodedAdd, &nEncodedLen);
	char* base64Address;
	RSACrypt::Base64Encode(cEncodedAdd, nEncodedLen, &base64Address);
	free(cEncodedAdd);
	return base64Address;
}

int Wallet::VerifyAddress(char* base64Address) const{
	RSA* rRsa = RSACrypt::ReadRSA(_cPublicKey, 1);
	unsigned char* cEncodedAdd;
	size_t nEncodedLen;
	bool authentic;
	RSACrypt::Base64Decode(base64Address, &cEncodedAdd, &nEncodedLen);
	bool result = RSACrypt::VerifySignature(rRsa, cEncodedAdd, nEncodedLen, (char*)_cPublicKey, strlen((char*)_cPublicKey), &authentic);
	return (result & authentic);
}

unsigned char* Wallet::GetPublicKey() const{
	return _cPublicKey;
}

void Wallet::SetPublicKey(unsigned char* sPublicKey){
	_cPublicKey = sPublicKey;
}

unsigned char* Wallet::GetPrivateKey() const{
	return _cPrivateKey;
}

void Wallet::SetPrivateKey(unsigned char* sPrivateKey){
	_cPrivateKey = sPrivateKey;
}
