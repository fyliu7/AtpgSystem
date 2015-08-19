/*
 * =====================================================================================
 *
 *       Filename:  atpg.h
 *
 *    Description:  header file for main ATPG process 
 *
 *        Version:  1.0
 *        Created:  08/19/2015 08:45:58 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  xenia-cjen (xc), jonah0604@gmail.com
 *        Company:  LaDS(I), GIEE, NTU
 *
 * =====================================================================================
 */

#ifndef _CORE_ATPG_H_ 
#define _CORE_ATPG_H_ 

#include "simulator.h" 

namespace CoreNs { 

class Atpg { 
public: 
        Atpg(Circuit *cir, Fault* f); 
        ~Atpg(); 

    void init(); 
    bool FaultActivate(); 
    bool Imply; 

protected: 
    Circuit *cir_; 
    Simulator *sim_; 

    Fault *target_fault_; 
}; //Atpg

inline Atpg::Atpg(Circuit *cir, Fault *f) { 
    cir_ = cir; 
    target_fault_ = f; 

    sim_ = new Simulator(cir_, target_fault_); 
}

inline Atpg::~Atpg() { 
    delete sim_; 
}

}; //CoreNs 

#endif //_CORE_ATPG_H_
