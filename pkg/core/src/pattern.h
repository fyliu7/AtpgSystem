/*
 * =====================================================================================
 *
 *       Filename:  pattern.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  08/21/2015 11:45:46 PM
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  xenia-cjen (xc), jonah0604@gmail.com
 *        Company:  LaDS(I), GIEE, NTU
 *
 * =====================================================================================
 */

#ifndef _CORE_PATTERN_H_ 
#define _CORE_PATTERN_H_ 

#include "fault.h"
#include "circuit.h" 

namespace CoreNs { 

typedef std::vector<Value> ValueVec; 

class Pattern { 
public: 

        Pattern(); 
        ~Pattern(); 

    ValueVec pi; 
    ValueVec ppi; 
    ValueVec po; 
    ValueVec ppo; 

}; //Pattern

}; //CoreNs

#endif //_CORE_PATTERN_H_
