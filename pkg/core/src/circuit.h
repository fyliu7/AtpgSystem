#ifndef _CORE_CIRCUIT_H_ 
#define _CORE_CIRCUIT_H_ 

#include "gate.h" 

namespace CoreNs { 

typedef std::vector<int> IdVec;  

class Circuit { 
public: 
        ~Circuit(); 

    size_t  npi; 
    size_t  nppi; 
    size_t  npo; 
    size_t  nlogic; 
    
    size_t  ntotgate; 
    size_t  ntotlvl; 

    IdVec   pis; 
    IdVec   ppis; 
    IdVec   pos; 
    IdVec   ppos;  

    GateVec gates; 

}; //Circuit

inline Circuit::~Circuit() { 
//  for (size_t i=0; i<gates.size(); i++) 
//      delete gates[i]; 
}

}; //CoreNs

#endif //_CORE_CIRCUIT_H_
