#ifndef _CORE_ATPG_H_
#define _CORE_ATPG_H_

#define _MAX_BACK_TRACK_LIMIT_   256

#include "simulator.h" 

namespace CoreNs { 

class Atpg {
public:
            Atpg(Circuit *cir, Fault* f);     
            ~Atpg();  

    bool    Tpg(TestPattern& p); 
    
    void    Initialize(); 
    bool    FaultActivate(); 
    bool    Imply(); 
    

protected: 
	Circuit         *cir_;
	Simulator       *sim_;

    const size_t    back_track_limit; 
    size_t          back_track_count; 

    Fault           *current_fault; 
}; //Atpg

inline Atpg::Atpg(Circuit *cir, Fault* f) : 
    back_track_limit(_MAX_BACK_TRACK_LIMIT_) { 
    
    cir_            = cir; 
    sim_            = new Simulator(cir_); 

    current_fault   = f; 
}

inline Atpg::~Atpg() {
    delete sim_; 
}

}; //CoreNs 
#endif // _CORE_ATPG_H_
