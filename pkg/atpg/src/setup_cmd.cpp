#include "setup_cmd.h"

using namespace std; 
using namespace TCLAP; 

using namespace AtpgNs; 
using namespace CmdNs; 
using namespace IntfNs; 

ReadNetlistCmd::ReadNetlistCmd(const string &name, 
                               const string &msg, 
                               Netlist *nl) : Cmd(name, msg) { 

    nl_ = nl; 
    try { 
        opt_mgr_->regArg("nl_file", 
                         "verilog gate-level netlist file", 
                         "netlsit_file", 
                         "", 
                         true); 
    }
    catch (ArgException &e) { 
        //TODO
        exit(1); 
    }
} 

ReadNetlistCmd::~ReadNetlistCmd() {}
bool ReadNetlistCmd::run() { 
    string nl_fname = opt_mgr_->getVal("nl_file"); 
    
    nl_->parse(nl_fname, string()); //TODO
    //cout << "  netlist info. \n"; 
    //cout << "    #ports:\t\t"; 
    //cout << nl_->getTop()->getPortNum() << endl; 
    //cout << "    #cells:\t\t"; 
    //cout << nl_->getTop()->getCellNum() << endl; 
    //cout << "    #nets:\t\t"; 
    //cout << nl_->getTop()->getNetNum() << endl; 

    return true; 
}

ReportCellCmd::ReportCellCmd(const string &name, 
                             const string &msg, 
                             Netlist *nl) : Cmd(name, msg) { 

    nl_ = nl; 
    try { 
        opt_mgr_->regArg("cell", 
                         "cell(s) to be reported", 
                         "cell(s)", 
                         "", 
                         false, 
                         true); 

    } 
    catch (ArgException &e) { 
        //TODO 
        exit(1); 
    }
}

ReportCellCmd::~ReportCellCmd() {} 
bool ReportCellCmd::run() { 
    if(!nl_->getTop()) { 
        //TODO 
        return false; 
    }

    //TODO: specified cells 

    for(size_t n=0; n<nl_->getTop()->getCellNum(); n++) { 
        string nm; 
        Cell *c = nl_->getTop()->getCell(n);   

        cout << "Cell "; 
        c->getName(nm); 
        cout << nm << " type("; 
        cout << c->getType() << ")\n"; 
        
        CellVec fanins, fanouts; 
        c->getFaninCell(fanins); 
        for (size_t i=0; i<c->getOutNetNum(); i++) { 
            c->getFanoutCell(i, fanouts); 
        }
        
        cout << "fi[" << fanins.size() << "] "; 
        for (size_t i=0; i<fanins.size(); i++) { 
            fanins[i]->getName(nm); 
            cout << nm << " "; 
        }
        cout << endl; 

        cout << "fo[" << fanouts.size() << "] "; 
        for (size_t i=0; i<fanouts.size(); i++) { 
            fanouts[i]->getName(nm); 
            cout << nm << " "; 
        }
        cout << endl; 
        cout << endl; 
    }

    return true; 
}
