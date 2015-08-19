#include "simulator.h" 

using namespace std; 

using namespace CoreNs; 

bool Simulator::EventDrivenSim() {
    for (size_t l=0; l<cir_->ntotlvl; l++) { 
        while (!events_[l].empty()) { 
            Gate *g = cir_->gates[events_[l].front()]; 
            events_[l].pop();  

            Value v; 
            v = g->GoodEval();

            if(g->val!=v) { 
                g->val = v; 
                for(size_t n=0; n<g->nfo; n++) 
                    PushEvent(g->fos[n]->id); 
            }
        }
    }
    return true; 
} 

void Simulator::PushEvent(int gid) { 
    events_[cir_->gates[gid]->lvl].push(gid); 
} 

Value Simulator::FaultEval(Gate* g) const { 
    if (target_fault->fpid==0) { //fault on output 
        Value v = g->GoodEval(); 
        if(v==L&&target_fault->fval==B) v = B; 
        if(v==H&&target_fault->fval==D) v = D; 
        return v;  
    } 
    else { //fault on input 
        size_t fp = target_fault->fpid; 
        Value v = g->fis[fp]->val; 
        if(v==L&&target_fault->fval==B) v = B; 
        if(v==H&&target_fault->fval==D) v = D; 

        vector<Value> vs; vs.push_back(v); 
        for(size_t n=0; n<g->nfi; n++) 
            if(n!=fp-1) 
                vs.push_back(g->val); 

        switch(g->type) { 
        case GATE_NOT: 
                return EvalNot(v);  
            break; 
        case GATE_AND: 
                return EvalAndN(vs); 
            break; 
        case GATE_OR: 
                return EvalOrN(vs); 
            break; 
        case GATE_NAND: 
                return EvalNandN(vs); 
            break; 
        case GATE_NOR: 
                return EvalNorN(vs); 
            break; 
        case GATE_PO: 
        case GATE_PPO: 
            return v; 
            break; 
        }
    }
}
