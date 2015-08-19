#ifndef _CORE_SIMULATOR_H_ 
#define _CORE_SIMULATOR_H_ 

#include <queue> 

#include "fault.h"
#include "circuit.h" 

namespace CoreNs { 

class Simulator { 
public: 
         Simulator(Circuit *cir, Fault *ftarget); 
         ~Simulator(); 

    bool EventDrivenSim(); 
    void PushEvent(int gid); 

private: 
    Value           FaultEval(Gate* g) const; 

    Circuit         *cir_; 
    Fault           *target_fault; 

    std::queue<int> *events_; 
}; //Simulator

inline Simulator::Simulator(Circuit *cir, Fault *ftarget) { 
    cir_ = cir; 
    target_fault = ftarget; 

    events_ = new std::queue<int> [cir_->ntotlvl]; 
}

inline Simulator::~Simulator() { 
    delete [] events_;
}

}; //CoreNs

#endif //_CORE_SIMULATOR_H_
