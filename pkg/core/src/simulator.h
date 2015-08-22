#ifndef _CORE_SIMULATOR_H_ 
#define _CORE_SIMULATOR_H_ 

#include <queue> 

#include "pattern.h"

namespace CoreNs { 

class Simulator { 
public: 
         Simulator(Circuit *cir, Fault *ftarget); 
         ~Simulator(); 

    void Init(); 

    bool EventDrivenSim(); 
    void PushFanoutEvent(int gid); 
    void PushEvent(int gid); 

    Value GetGutVal() const; 
    bool IsGutOutputAtX() const; 

    void GetDFrontier(GateVec& df) const; 

    bool IsFaultAtPo() const; 

    void GetPiPattern(Pattern& p) const; 
    void SetPatternToPi(Pattern& p); 

private: 
    Value           FaultEval(Gate* g) const; 

    Circuit         *cir_; 
    Fault           *target_fault_; 

    std::queue<int> *events_; 
}; //Simulator

inline Simulator::Simulator(Circuit *cir, Fault *ftarget) { 
    cir_ = cir; 
    target_fault_ = ftarget; 

    events_ = new std::queue<int> [cir_->ntotlvl]; 
}

inline Simulator::~Simulator() { 
    delete [] events_;
}

inline void Simulator::Init() { 
    for(size_t n=0; n<cir_->ntotgate; n++) 
        cir_->gates[n]->val = X; 
}

inline bool Simulator::IsGutOutputAtX() const { 
    Value fv = cir_->gates[target_fault_->fgate_id]->val; 
    return fv==X; 
}

inline Value Simulator::GetGutVal() const { 
    return cir_->gates[target_fault_->fgate_id]->val; 
}

}; //CoreNs

#endif //_CORE_SIMULATOR_H_
