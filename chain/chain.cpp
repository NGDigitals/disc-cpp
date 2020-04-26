#include "chain.hpp"

using std::cout;
using std::endl;
using std::out_of_range;

template <class T>
void Chain<T>::Push (T const& tElem) { 
   _vElems.push_back(tElem);    
} 

template <class T>
void Chain<T>::Pop () { 
   if (_vElems.empty()) { 
      throw out_of_range("Chain<>::Pop(): empty chain"); 
   }
   _vElems.pop_back();         
} 

template <class T>
T Chain<T>::Top () const { 
   if (_vElems.empty()) { 
      throw out_of_range("Chain<>::top(): empty chain"); 
   }
   return _vElems.back();      
}