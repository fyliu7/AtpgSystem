/*
 * =====================================================================================
 *
 *       Filename:  atpg.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  08/21/2015 12:16:02 AM
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  xenia-cjen (xc), jonah0604@gmail.com
 *        Company:  LaDS(I), GIEE, NTU
 *
 * =====================================================================================
 */
#include <climits>

#include "atpg.h"

using namespace std; 

using namespace CoreNs; 

bool Atpg::Tpg(Pattern& p) { 
    init(); 

    AtpgStatus status = DECISION; 
    while(status!=EXIT) { 
        switch(status) { 
        case IMPLY_AND_CHECK: { 
            status = DECISION; 
            if(!Imply()) { 
                status = BACKTRACK; 
            }
                
            if(sim_->IsFaultAtPo()) return true; 

            if(sim_->IsGutOutputAtX()) break; 

            Value fv = sim_->GetGutVal(); 
            if(fv!=D && fv!=B) { 
                status = BACKTRACK; 
            }
            
            break; 
        }
        case DECISION:  
            if(sim_->IsGutOutputAtX()) {  
                FaultActivate(); 
                status = BACKTRACE; 
            }
            else {
                if(!DDrive()) status = BACKTRACK; 
                else status = BACKTRACE; 
            }
            break; 
        case BACKTRACE: 
            status = IMPLY_AND_CHECK; 
            if(!Backtrace()) 
                status = BACKTRACK; 
            break; 
        case BACKTRACK: 
            if(!BackTrack()) return false; 
            status = IMPLY_AND_CHECK; 
            break; 
        }
    }

    return true; 
} 

void Atpg::init() { 
    back_track_count = 0; 

    sim_->Init(); 
}

bool Atpg::Imply() { 
   return sim_->EventDrivenSim(); 
}

bool Atpg::FaultActivate() { 
    Gate *fg = cir_->gates[target_fault_->fgate_id]; 
    size_t fp = target_fault_->fpid; 
    Value fv = target_fault_->fval; 

    if(fp) { //input stuck fault on g.u.t. 
        Value v = fg->fis[fp-1]->val; 
        if(v==X) { 
            Value objv = (fv==D)?H:L; 
            current_obj_.first = fg->fis[fp-1]->id; 
            current_obj_.second = objv; 
            return true; 
        } 
        //else if((fv==D && v==H) || (fv==B && v==L)) { 
        else { 
            Value objv = EvalNot(fg->output_ctr_value); 
            //if(fg->type==GATE_AND || fg->type==GATE_NOR) objv = H; 
            //else if (fg->type==GATE_NAND || fg->type==GATE_OR) objv = L; 
            if(objv==X) return true; //NOT, PO, PPO, TODO: XOR, XNOR
            current_obj_.first = fg->id; 
            current_obj_.second = objv; 
            return true; 
        } 
        //else return false; 
    }
    else { //output stuck fault on g.u.t. 
        Value objv = (fv==D)?H:L; 
        current_obj_.first = fg->id; 
        current_obj_.second = objv; 
        return true; 
    }
} 

bool Atpg::DDrive() {
    GateVec dfront; 
    sim_->GetDFrontier(dfront); 

    if(dfront.size()==0) return false; 
                
    Gate *gtoprop = NULL; 
    int observ = INT_MIN; 
    for(size_t i=0; i<dfront.size(); i++)  
        if(dfront[i]->lvl>observ) { 
            gtoprop = dfront[i]; 
            observ = dfront[i]->lvl; 
        }

    current_obj_.first = gtoprop->id; 
    current_obj_.second = EvalNot(gtoprop->output_ctr_value); 

    return true; 
}

bool Atpg::Backtrace() { 
    Gate *g = cir_->gates[current_obj_.first]; 
    Value objv = current_obj_.second; 
    while (!(g->type==GATE_PI || g->type==GATE_PPI)) {  
        if(g->output_ctr_value==X) { //NOT, TODO: XOR, XNOR  
            if(g->type==GATE_NOT) 
                objv = EvalNot(objv); 
            g = g->fis[0]; 
            continue; 
        }
        
        Gate *gnext = NULL;  
        if (objv==g->output_ctr_value) { //if objv easy to set 
            //choose input of "g" which 
            //  1) is at X 
            //  2) is easiest to control  
            int ctr_ablility = INT_MAX; 
            for(size_t n=0; n<g->nfi; n++) {  
                if(g->fis[n]->val==X && g->fis[n]->lvl<ctr_ablility) { 
                    gnext = g->fis[n]; 
                    ctr_ablility = g->fis[n]->lvl; 
                }
            }
        } 
        else if (objv==EvalNot(g->output_ctr_value)) { //if objv hard to set
            //choose input of "g" which 
            //  1) is at X 
            //  2) is hardest to control  
            int ctr_inablility = INT_MIN; 
            for(size_t n=0; n<g->nfi; n++) {  
                if(g->fis[n]->val==X && g->fis[n]->lvl>ctr_inablility) { 
                    gnext = g->fis[n]; 
                    ctr_inablility = g->fis[n]->lvl; 
                }
            }
        } 

        if(g->type==GATE_NAND || g->type==GATE_NOR) 
            objv = EvalNot(objv); 

        g = gnext; 
    }

    //TODO: do check conflict. 
    g->val = objv; 
    if(target_fault_->fgate_id== g->id) //fault on P/PI; 
        sim_->PushEvent(g->id); 
    else 
        sim_->PushFanoutEvent(g->id); 

    return true; 
}

bool Atpg::BackTrack() { 
    //TODO    

    return false; 
}
