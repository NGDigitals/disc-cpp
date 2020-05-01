#include "cache.hpp"
#include "../util/rsa-crypt.hpp"

Cache::Cache(std::string sName) : _sName(sName){}

void Cache::GenerateKeys(){
	std::vector<char*> vKeys = RSACrypt::CreateKeys();
	_cPublicKey = (unsigned char*)vKeys[0];
	_cPrivateKey = (unsigned char*)vKeys[1];
}

std::string Cache::GetName() const{
	return _sName;
}

void Cache::SetName(std::string sName){
	_sName = sName;
}

unsigned char* Cache::GetPublicKey() const{
	return _cPublicKey;
}

void Cache::SetPublicKey(unsigned char* sPublicKey){
	_cPublicKey = sPublicKey;
}

unsigned char* Cache::GetPrivateKey() const{
	return _cPrivateKey;
}

void Cache::SetPrivateKey(unsigned char* sPrivateKey){
	_cPrivateKey = sPrivateKey;
}
