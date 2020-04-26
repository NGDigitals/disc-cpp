#ifndef BLOCK_H
#define BLOCK_H

#include <vector>
#include <iostream>
#include "transaction.hpp"

using std::string;
using std::vector;

class Block {
private:
    uint64_t _nIndex;
    uint32_t _nNonce;
    vector<Transaction> _vTrans;
    time_t _tTimestamp;

    string _GenerateHash() const;
public:
    string sHash;
    string sPrevHash;

    Block(uint64_t nIndex, const string &sPrevHash, const vector<Transaction> &vTrans, uint32_t _nNonce, time_t _tTimestamp);
    void CreateBlock(uint32_t nDifficulty);
};

#endif //BLOCK_H