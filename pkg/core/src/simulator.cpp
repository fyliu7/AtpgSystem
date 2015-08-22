#include "simulator.h" 

using namespace std; 

using namespace CoreNs; 

bool Simulator::EventDrivenSim() {
    for (size_t l=0; l<cir_->ntotlvl; l++) { 
        while (!events_[l].empty()) { 
            Gate *g = cir_->gates[events_[l].front()]; 
            events_[l].pop();  

            Value v; 
            v = (g->id==target_fault->fgate_id)?FaultEval(g):g->GoodEval();

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

void Simulator::PushFanoutEvent(int gid) {
    Gate *g = cir_->gates[gid]; 
    for(size_t n=0; n<g->nfo; n++) 
        PushEvent(g->fos[n]->id);  
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
        Value v = g->fis[fp-1]->val; 
        if(v==L&&target_fault->fval==B) v = B; 
        if(v==H&&target_fault->fval==D) v = D; 

        vector<Value> vs; vs.push_back(v); 
        for(size_t n=0; n<g->nfi; n++) 
            if(n!=fp-1) 
                vs.push_back(g->fis[n]->val); 

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

void Simulator::GetPiPattern(Pattern& p) { 
    for (size_t n=0; n<cir_->npi; n++) 
       p.pi.push_back(cir_->gates[cir_->pis[n]]->val); 
    for (size_t n=0; n<cir_->nppi; n++) 
       p.ppi.push_back(cir_->gates[cir_->ppis[n]]->val); 
}
