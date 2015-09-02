#include "setup_cmd.h" 
#include "atpg_cmd.h"

using namespace std; 

using namespace AtpgNs; 
using namespace CmdNs; 
using namespace IntfNs; 
using namespace CoreNs; 

int main(int argc, char **argv) { 
    
    AtpgMgr mgr; 
    Netlist nl; 
    ReadNetlistCmd cmdRdNl("atpg", "read gate-level netlist file", &nl); 
    BuildCircuitCmd cmdBC("build_cir", "build circuit", &nl, &mgr); 
    //ReportCellCmd  cmdRptC("report_cell", "report cell(s) information", &nl); 
    //ReportGateCmd  cmdRptG("report_gate", "report gate(s) information", &nl, &mgr); 
    AddFaultCmd    cmdAddF("add_fault", "add target fault(s)", &nl, &mgr); 
    RunAtpgCmd     cmdRunA("run_atpg", "run atpg process", &nl, &mgr); 
    ReportPatCmd   cmdRptP("report_pattern", "report pattern(s)", &nl, &mgr); 

    vector<string> argsRdNl; 
    //vector<string> argsRptC; 
    vector<string> argsBC; 
    //vector<string> argsRptG; 
    vector<string> argsAddF; 
    vector<string> argsRunA; 
    vector<string> argsRptP; 

    for(int i=0; i<argc; i++) 
        argsRdNl.push_back(string(argv[i])); 
    argsBC.push_back(string("build_cir")); 
    //argsRptC.push_back(string("report_cell")); 
    //argsRptG.push_back(string("report_gate")); 
    argsAddF.push_back(string("add_fault")); 
    argsAddF.push_back(string("--all")); 
    argsRunA.push_back(string("run_atpg")); 
    argsRptP.push_back(string("report_pattern")); 

    cmdRdNl.exec(argsRdNl); 
    //cmdRptC.exec(argsRptC);  
    cmdBC.exec(argsBC); 
    //cmdRptG.exec(argsRptG);  
    cmdAddF.exec(argsAddF); 
    cmdRunA.exec(argsRunA); 
    cmdRptP.exec(argsRptP); 

    return 0; 
}
