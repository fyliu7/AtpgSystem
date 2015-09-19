#include "atpg_cmd.h" 

using namespace std; 
using namespace TCLAP; 

using namespace AtpgNs; 
using namespace CmdNs; 
using namespace IntfNs; 
using namespace CoreNs; 

BuildCircuitCmd::BuildCircuitCmd(const string &name, 
                                 const string &msg, 
                                 Netlist *nl, 
                                 AtpgMgr *atpg_mgr) : Cmd(name, msg) { 

    nl_ = nl; 
    atpg_mgr_ = atpg_mgr; 
    //TODO
}

BuildCircuitCmd::~BuildCircuitCmd() { 
}

bool BuildCircuitCmd::run() { 
    if(!nl_->getTop()) { 
        //TODO; 
        return false; 
    }

    if(!atpg_mgr_->cir) { 
        atpg_mgr_->cir = new Circuit; 
    }

    return nl_->build(atpg_mgr_->cir); 
}

ReportGateCmd::ReportGateCmd(const string &name, 
                             const string &msg, 
                             Netlist *nl, 
                             AtpgMgr *atpg_mgr) : Cmd(name, msg) {

    nl_ = nl; 
    atpg_mgr_ = atpg_mgr; 
    try { 
        opt_mgr_->regArg("gate", 
                         "gate(s) to be reported", 
                         "gate(s)", 
                         "", 
                         false, 
                         true); 
    } 
    catch (ArgException &e) { 
        //TODO
        exit(1); 
    }
}

ReportGateCmd::~ReportGateCmd() {
} 
bool ReportGateCmd::run() { 
    if(!atpg_mgr_->cir) { 
        //TODO
        return false; 
    } 
    
    //TODO: specified gates 
    reportGate(); 
    return true; 
}

void ReportGateCmd::reportGate() const { 
    for(int n=0; n<atpg_mgr_->cir->ntotgate; n++) { 
        Gate *g = atpg_mgr_->cir->gates[n]; 
        cout << " Gate"; 
        cout << " id (" << n << ")"; 
        cout << " lvl(" << g->lvl << ")"; 
        cout << " typ(" << g->type << ")\n"; 
        cout << "    fi[" << g->nfi << "] "; 
        for(int ni=0; ni<g->nfi; ni++) 
            cout << g->fis[ni]->id << " "; 
        cout << endl; 
        cout << "    fo[" << g->nfo << "] "; 
        for(int no=0; no<g->nfo; no++) 
            cout << g->fos[no]->id << " "; 
        cout << endl << endl; 
    } 
}

AddFaultCmd::AddFaultCmd(const string &name, 
                         const string &msg, 
                         Netlist *nl, 
                         AtpgMgr *atpg_mgr) : Cmd(name, msg) { 
    
    nl_ = nl; 
    atpg_mgr_ = atpg_mgr; 
    try { 
        opt_mgr_->regOpt("all", 
                         "add all faults as targeted", 
                         false, 
                         "a"); 
        //TODO
    } 
    catch (ArgException &e) { 
        //TODO
        exit(1); 
    }
}

AddFaultCmd::~AddFaultCmd() {}  
bool AddFaultCmd::run() {
    if(!atpg_mgr_->cir) { 
        //TODO
        return false; 
    }

    if(!atpg_mgr_->f_mgr)  
        atpg_mgr_->f_mgr = new FaultMgr; 

    cout << "Adding fualt(s)...\n\n"; 
    if(opt_mgr_->getOptVal("all")) { 
        atpg_mgr_->f_mgr->AddAllFaults(atpg_mgr_->cir); 

        cout << "currently has " << atpg_mgr_->f_mgr->getFaultNum(); 
        cout << " fault(s)...\n\n"; 

        return true; 
    }
    
    //TODO
    return false; 
}  

ReportFaultCmd::ReportFaultCmd(const string &name, 
                                 const string &msg, 
                                 Netlist *nl, 
                                 AtpgMgr *atpg_mgr) : Cmd(name, msg){

    nl_ = nl; 
    atpg_mgr_ = atpg_mgr; 

    try { 
        opt_mgr_->regArg("fault", 
                         "fault(s) to be reported", 
                         "fault(s)", 
                         "", 
                         false, 
                         true); 

        opt_mgr_->regArg("status", 
                         "fault(s) under specified status reported", 
                         "status", 
                         "s", 
                         false, 
                         false); 
    } 
    catch (ArgException &e) { 
        //TODO
        exit(1); 
    }
} 

ReportFaultCmd::~ReportFaultCmd() { 
}

bool ReportFaultCmd::run() { 
    if(!atpg_mgr_->f_mgr) { 
        //TODO
        return false; 
    }

    string st = opt_mgr_->getVal("status"); 
    Fault::Status sta = Fault::NA; 
    if(!st.empty()) { 
        if (st=="UD") sta = Fault::UD; 
        else if (st=="DT") sta = Fault::DT; 
        //TODO
    } 

    //TODO: print specific fault 
    repFault(sta); 

    return true; 
}

void ReportFaultCmd::repFault(Fault::Status status) {
    for(size_t n=0; n<atpg_mgr_->f_mgr->getFaultNum(); n++) { 
        Fault *f = atpg_mgr_->f_mgr->getFault(n); 
        if(status!=Fault::NA && status!=f->status) continue; 
        //Gate *g = atpg_mgr_->cir->gates[f->fgate_id]; 
        Cell *c = nl_->cells[atpg_mgr_->cir->gates[f->fgate_id]->id]; 

        string type = (f->fval==D)?"SA0":"SA1"; 
        cout << type << " "; 
        
        //string fnm; nl_->cells[g->id]->getName(fnm); 
        string fnm; 
        if(c->getType()==CELL_PO) { 
            c->getInNet(0)->getName(fnm); 
            cout << fnm << endl; 
            continue; 
        }
        else if(c->getType()==CELL_PPO) { 
            c->getName(fnm); 
            cout << fnm << "/D" << endl;  
            continue; 
        }
        else { 
            c->getOutNet(0)->getName(fnm); 

            if(c->getType()==CELL_PI) { 
                cout << fnm << endl;  
                continue; 
            }
            else if(c->getType()==CELL_PPI) { 
                cout << "U_" << fnm << "/Q" << endl;  
                continue; 
            }
        }
        if(!f->fpid) { 
            cout << "U_" << fnm << "/Y";  
        }
        else {
            string faninnm; c->getInNet(f->fpid-1)->getName(faninnm); 
            cout << "U_" << fnm << "/" << (char)('A'+f->fpid-1);  
        }

        cout << endl; 
    }
}

RunAtpgCmd::RunAtpgCmd(const string &name, 
                       const string &msg, 
                       Netlist *nl, 
                       AtpgMgr *atpg_mgr) : Cmd(name, msg) { 

    nl_ = nl; 
    atpg_mgr_ = atpg_mgr; 
}

RunAtpgCmd::~RunAtpgCmd() { 
}

bool RunAtpgCmd::run() { 
    if(!atpg_mgr_->cir) { 
        //TODO 
        return false; 
    }
    
    if(!atpg_mgr_->f_mgr) { 
        atpg_mgr_->f_mgr = new FaultMgr; 
        atpg_mgr_->f_mgr->AddAllFaults(atpg_mgr_->cir); 
    }
    atpg_mgr_->Run(); 

    return true;  
}

ReportPatCmd::ReportPatCmd(const string &name, 
                             const string &msg, 
                             Netlist *nl, 
                             AtpgMgr *atpg_mgr) : Cmd(name, msg) { 

    nl_ = nl; 
    atpg_mgr_ = atpg_mgr; 
}

ReportPatCmd::~ReportPatCmd() { 
}

bool ReportPatCmd::run() { 
    if(!atpg_mgr_->pat_mgr) { 
        //TODO 
        return false; 
    }

    string nm; 
    for (size_t n=0; n<atpg_mgr_->pat_mgr->npi; n++) {  
        nl_->cells[atpg_mgr_->pat_mgr->pi_order[n]]->getName(nm); 
        cout << nm << " "; 
    }
    cout << "|\n"; 

    for (size_t n=0; n<atpg_mgr_->pat_mgr->nppi; n++) { 
        nl_->cells[atpg_mgr_->pat_mgr->ppi_order[n]]->getOutNet(0)->getName(nm); 
        cout << nm << " "; 
    }
    cout << "|\n"; 
    
    for (size_t n=0; n<atpg_mgr_->pat_mgr->npo; n++) { 
        nl_->cells[atpg_mgr_->pat_mgr->po_order[n]]->getName(nm); 
        cout << nm << " "; 
    }
    cout << "|\n"; 


    cout << "BASIC_SCAN\n"; 
    cout << "_num_of_pattern_" << atpg_mgr_->pat_mgr->pats.size() << endl; 
    for(size_t i=0; i<atpg_mgr_->pat_mgr->pats.size(); i++) { 
        Pattern *p = atpg_mgr_->pat_mgr->pats[i]; 
        cout << "_pattern_" << i+1 << " "; 
        for(size_t j=0; j<p->pi.size(); j++) 
            Print3Value(p->pi[j]); 
        cout << " | "; 
        cout << " | "; 
        for(size_t j=0; j<p->ppi.size(); j++) 
            Print3Value(p->ppi[j]); 
        cout << " | "; 
        cout << " | "; 
        for(size_t j=0; j<atpg_mgr_->pat_mgr->npo; j++) 
            cout << "X"; 
        cout << " | "; 
        cout << " | "; 
        for(size_t j=0; j<atpg_mgr_->pat_mgr->nppi; j++) 
            cout << "X"; 
        cout << endl; 
    }

    return true; 
}
