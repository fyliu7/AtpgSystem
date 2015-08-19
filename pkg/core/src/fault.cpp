#include "fault.h" 

using namespace std; 

using namespace CoreNs; 

void FaultMgr::AddFault(Gate *g, size_t p, Value v) { 
    //TODO
    faults_.push_back(new Fault(g->id, p, v)); 
} 

void FaultMgr::AddFault(Gate *g, Value v) { 
    //TODO
    faults_.push_back(new Fault(g->id, 0, v)); 
}

void FaultMgr::AddAllFaults(Circuit *cir) { 
    for (size_t i=0; i<cir->gates.size(); i++) { 
        Gate *g = cir->gates[i]; 
        for(int j=0; j<=g->fis.size(); j++) { 
            AddFault(g, j, D); 
            AddFault(g, j, B); 
        }
    }
}
