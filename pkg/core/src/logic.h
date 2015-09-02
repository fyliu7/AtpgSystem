#ifndef _CORE_LOGIC_H_
#define _CORE_LOGIC_H_

#include <iostream>
#include <vector>
#include <cstdio>
#include <cstdarg>

namespace CoreNs { 

typedef unsigned char  Value;
typedef unsigned long  ParaValue;
//typedef std::pair<ParaValue, ParaValue> ParaValPair;

const Value     L       = 0;
const Value     H       = 1;
const Value     X       = 2;
const Value     D       = 3;
const Value     B       = 4;

const ParaValue PARA_L  = 0;
const ParaValue PARA_H  = ~PARA_L;
const size_t BYTE_SIZE  = 8;
const size_t WORD_SIZE  = sizeof(ParaValue) * BYTE_SIZE;

const Value NotMap[5] = { H, L, X, B, D };

const Value AndMap[5][5] = { 
        { L, L, L, L, L }, 
        { L, H, X, D, B }, 
        { L, X, X, X, X }, 
        { L, D, X, D, L }, 
        { L, B, X, L, B }
     };

const Value OrMap[5][5] = { 
        { L, H, X, D, B }, 
        { H, H, H, H, H }, 
        { X, H, X, X, X }, 
        { D, H, X, D, H }, 
        { B, H, X, H, B }
     };

//const Value XorMap[5][5] = {  
//        { L, H, X, D, B }, 
//        { H, L, X, B, D }, 
//        { X, X, X, X, X }, 
//        { D, B, X, L, H }, 
//        { B, D, X, H, L }
//     };
	
const Value NandMap[5][5] = { 
        { H, H, H, H, H }, 
        { H, L, X, B, D }, 
        { H, X, X, X, X }, 
        { H, B, X, B, H }, 
        { H, D, X, H, D }
     };

const Value NorMap[5][5] = { 
        { H, L, X, B, D }, 
        { L, L, L, L, L }, 
        { X, L, X, X, X }, 
        { B, L, X, B, L }, 
        { D, L, X, L, D }
     };

//const Value XnorMap[5][5] = { 
//        { H, L, X, B, D }, 
//        { L, H, X, D, B }, 
//        { X, X, X, X, X }, 
//        { B, D, X, H, L }, 
//        { D, B, X, L, H }
//     };

inline void Print3Value(Value v) { 
    switch(v) { 
    case L: 
    case B: 
        std::cout << "0"; 
        break; 
    case H: 
    case D: 
        std::cout << "1"; 
        break; 
    case X: 
        std::cout << "X"; 
        break; 
    }
}

inline void Print5Value(Value v) { 
    switch(v) { 
    case L: 
    case H: 
        std::cout << (unsigned) v; 
        break; 
    case X: 
        std::cout << "X"; 
        break; 
    case D: 
        std::cout << "D"; 
        break; 
    case B:
        std::cout << "B"; 
        break; 
    }
}

inline bool GetBitVal(ParaValue pv,  int pos) {
	ParaValue mask = 0x01;
	mask <<= pos;
	return pv & mask;
 }

inline void SetBitVal(ParaValue &pv, bool value ,int pos) {
	ParaValue mask = 0x01;
	mask <<= pos;
	if (value) pv |= mask;
	else pv &= ~mask;
}

inline Value EvalNot(Value v){
	return NotMap[v];
}

inline Value EvalAndN(size_t n...) {
    if (n<2) return X; 

    Value ret = X; 
    va_list vals; 
    va_start(vals, n); 

    Value val1 = (unsigned char) va_arg(vals, int);  
    Value val2 = (unsigned char) va_arg(vals, int);  
    ret = AndMap[val1][val2]; 
    for (size_t i=2; i<n; i++) { 
        Value val1 = (unsigned char) va_arg(vals, int);  
        ret = AndMap[ret][val1]; 
    }
    va_end(vals); 
    return ret; 
} 

inline Value EvalOrN(size_t n...) {
    if (n<2) return X; 

    Value ret = X; 
    va_list vals; 
    va_start(vals, n); 

    Value val1 = (unsigned char) va_arg(vals, int);  
    Value val2 = (unsigned char) va_arg(vals, int);  
    ret = OrMap[val1][val2]; 
    for (size_t i=2; i<n; i++) { 
        Value val1 = (unsigned char) va_arg(vals, int);  
        ret = OrMap[ret][val1]; 
    }
    return ret; 
} 

inline Value EvalNandN(size_t n...) {
    if (n<2) return X; 

    Value ret = X; 
    va_list vals; 
    va_start(vals, n); 

    Value val1 = (unsigned char) va_arg(vals, int);  
    Value val2 = (unsigned char) va_arg(vals, int);  
    ret = NandMap[val1][val2]; 
    for (size_t i=2; i<n; i++) { 
        Value val1 = (unsigned char) va_arg(vals, int);  
        ret = NandMap[ret][val1]; 
    }
    return ret; 
} 

inline Value EvalNorN(size_t n...) {
    if (n<2) return X; 

    Value ret = X; 
    va_list vals; 
    va_start(vals, n); 

    Value val1 = (unsigned char) va_arg(vals, int);  
    Value val2 = (unsigned char) va_arg(vals, int);  
    ret = NorMap[val1][val2]; 
    for (size_t i=2; i<n; i++) { 
        Value val1 = (unsigned char) va_arg(vals, int);  
        ret = NorMap[ret][val1]; 
    }
    return ret; 
} 

inline Value EvalAndN(std::vector<Value>& vals) {
    if(vals.size()<2) return X; 

    Value ret = AndMap[vals[0]][vals[1]]; 
    for(size_t n=2; n<vals.size(); n++) 
        ret = AndMap[ret][vals[n]]; 

    return ret; 
}

inline Value EvalOrN(std::vector<Value>& vals) {
    if(vals.size()<2) return X; 

    Value ret = OrMap[vals[0]][vals[1]]; 
    for(size_t n=2; n<vals.size(); n++) 
        ret = OrMap[ret][vals[n]]; 

    return ret; 
}

inline Value EvalNandN(std::vector<Value>& vals) {
    if(vals.size()<2) return X; 

    Value ret = NandMap[vals[0]][vals[1]]; 
    for(size_t n=2; n<vals.size(); n++) 
        ret = NandMap[ret][vals[n]]; 

    return ret; 
}

inline Value EvalNorN(std::vector<Value>& vals) {
    if(vals.size()<2) return X; 

    Value ret = NorMap[vals[0]][vals[1]]; 
    for(size_t n=2; n<vals.size(); n++) 
        ret = NorMap[ret][vals[n]]; 

    return ret; 
}

}; //CoreNs

#endif //_CORE_LOGIC_H_

