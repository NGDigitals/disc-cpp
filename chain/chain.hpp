#ifndef CHAIN_H
#define CHAIN_H

#include <iostream>
#include <vector>

using std::vector;

template <class T>
class Chain { 
private: 
    vector<T> _vElems;

public: 
    T Top() const;
    void Push(T const&);
    void Pop();                
    bool Empty() const {return _vElems.empty();} 
    vector<T> LoadData() const;
    bool SaveData(const T &data) const;
}; 

#endif //CHAIN_H