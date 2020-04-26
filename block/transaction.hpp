#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <iostream>

using std::string;

struct Transaction
{
    string _sSource;
    string _sRecipient;
    double _dAmount;
    string _sSignature;
    time_t _tTimestamp;
};

#endif //TRANSACTION_H