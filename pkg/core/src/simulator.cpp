#include "simulator.h" 

using namespace std; 

using namespace CoreNs; 

bool Simulator::EventDrivenSim() {
    for (size_t l=0; l<cir_->ntotlvl; l++) { 
        while (!events_[l].empty()) { 
            Gate *g = cir_->gates[events_[l].front()]; 
            events_[l].pop();  

            Value v; 
            v = (g->id==target_fault_->fgate_id)?FaultEval(g):g->GoodEval();

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

void Simulator::GetDFrontier(GateVec& df) const { 
    for (size_t n=0; n<cir_->ntotgate; n++) { 
        Gate *g = cir_->gates[n]; 
        if(g->val!=X) continue; 

        for(size_t i=0; i<g->nfi; i++) { 
            if(g->fis[i]->val==D || 
               g->fis[i]->val==B) { 

                df.push_back(g); 
                break; 
            }
        }
    }
}

bool Simulator::IsFaultAtPo() const { 
    for (size_t n=0; n<cir_->npo; n++) { 
        Value v = cir_->gates[cir_->pos[n]]->val;  
        if(v==D || v==B) return true; 
    }

    for (size_t n=0; n<cir_->nppi; n++) { 
        Value v = cir_->gates[cir_->ppos[n]]->val;  
        if(v==D || v==B) return true; 
    }
    return false; 
}

void Simulator::GetPiPattern(Pattern& p) const { 
    for (size_t n=0; n<cir_->npi; n++) 
       p.pi.push_back(cir_->gates[cir_->pis[n]]->val); 
    for (size_t n=0; n<cir_->nppi; n++) 
       p.ppi.push_back(cir_->gates[cir_->ppis[n]]->val); 
}

Value Simulator::FaultEval(Gate* g) const { 
    if (target_fault_->fpid==0) { //fault on output 
        Value v = g->GoodEval(); 
        if(v==L&&target_fault_->fval==B) v = B; 
        if(v==H&&target_fault_->fval==D) v = D; 
        return v;  
    } 
    else { //fault on input 
        size_t fp = target_fault_->fpid; 
        Value v = g->fis[fp-1]->val; 
        if(v==L&&target_fault_->fval==B) v = B; 
        if(v==H&&target_fault_->fval==D) v = D; 

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

