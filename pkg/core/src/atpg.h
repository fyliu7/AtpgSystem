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

#define _MAX_BACK_TRACK_LIMIT_   256

#include "simulator.h" 

namespace CoreNs { 

typedef std::pair<int, Value> Objective; 

class Atpg { 
public: 
    enum AtpgStatus { IMPLY_AND_CHECK = 0, 
                      DECISION, 
                      BACKTRACE, 
                      BACKTRACK,  
                      EXIT }; 

        Atpg(Circuit *cir, Fault* f); 
        ~Atpg(); 

        bool Tpg(Pattern& p); 

protected: 
    void init(); 
    bool Imply(); 
    bool FaultActivate(); 
    bool DDrive(); 
    bool Backtrace(); 
    bool BackTrack(); 

    Circuit   *cir_; 
    Simulator *sim_; 

    size_t    back_track_limit;
    size_t    back_track_count;

    Fault     *target_fault_; 

    Objective current_obj_; 
}; //Atpg

inline Atpg::Atpg(Circuit *cir, Fault *f) { 
    cir_ = cir; 
    target_fault_ = f; 
    back_track_limit = _MAX_BACK_TRACK_LIMIT_; 

    sim_ = new Simulator(cir_, target_fault_); 
}

inline Atpg::~Atpg() { 
    delete sim_; 
}

}; //CoreNs 

#endif //_CORE_ATPG_H_
