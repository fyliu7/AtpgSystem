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

class Pattern; 

typedef std::vector<Value> ValueVec; 
typedef std::vector<Pattern *> PatVec; 

class Pattern { 
public: 

        //Pattern(); 
        //~Pattern(); 

    ValueVec pi; 
    ValueVec ppi; 
    ValueVec po; 
    ValueVec ppo; 

}; //Pattern

class PatternMgr { 
public: 
        PatternMgr(Circuit *cir); 
        ~PatternMgr(); 

    size_t npi; 
    size_t nppi; 
    size_t npo; 

    IdVec pi_order; 
    IdVec ppi_order; 
    IdVec po_order; 
    IdVec ppo_order; 

    PatVec pats; 
}; //PatternMgr

inline PatternMgr::PatternMgr(Circuit *cir) { 
    npi = cir->npi; 
    nppi = cir->nppi; 
    npo = cir->npo; 

    pi_order = cir->pis; 
    ppi_order = cir->ppis; 
    po_order = cir->pos;  
    ppo_order = cir->ppos; 
} 

inline PatternMgr::~PatternMgr() { 
    for (size_t i=0; i<pats.size(); i++) 
        delete pats[i]; 
}

}; //CoreNs

#endif //_CORE_PATTERN_H_
