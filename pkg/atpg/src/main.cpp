#include "cli/src/sys_cmd.h"

#include "setup_cmd.h" 
#include "atpg_cmd.h"

using namespace std; 
using namespace TCLAP; 

using namespace AtpgNs; 
using namespace Cli; 
using namespace CmdNs; 
using namespace IntfNs; 
using namespace CoreNs; 

OptMgr      *myOptMgr; 
CmdMgr      *myCmdMgr; 
AtpgMgr     *myAtpgMgr; 
Netlist     *myNl; 

CmdParser   *cli; 

void myInit(const string &name, 
            const string &msg); 
void myInitCmd(); 
void myExec(int argc, char **argv); 
void myExit(int status); 

//void myTest(); 

int main(int argc, char **argv) { 
    myInit("atpg", "atpg tool main process"); 
    myExec(argc, argv); 
    return 0; 
}

void myInit(const string &name, 
            const string &msg) {

    myOptMgr    = new OptMgr(name, msg); 

    try { 
        myOptMgr->regArg("file", 
                         "source file name", 
                         "source_file", 
                         "f", 
                         false); 
    }
    catch (ArgException &e) { 
        //TODO
        myExit(1); 
    }

    myCmdMgr    = new CmdMgr; 
    myNl        = new Netlist; 
    myAtpgMgr   = new AtpgMgr; 

    cli         = new CmdParser(myCmdMgr); 

    myInitCmd(); 
} 

void myInitCmd() {
    // System Commands 
    Cmd *sysExitCmd = new SysExitCmd("exit", "exit the program", cli); 

    myCmdMgr->regCmd("SYSTEM", sysExitCmd); 

    // Setup Commands 
    Cmd *readNetlistCmd = new ReadNetlistCmd("read_netlist", "read gate-level netlist file", myNl); 
    Cmd *reportCellCmd = new ReportCellCmd("report_cell", "report cell(s) information", myNl); 
    
    myCmdMgr->regCmd("SETUP", readNetlistCmd); 
    myCmdMgr->regCmd("SETUP", reportCellCmd); 

    // Atpg Commands 
    Cmd *buildCircuitCmd = new BuildCircuitCmd("build_cir", "build circuit", myNl, myAtpgMgr); 
    Cmd *reportGateCmd = new ReportGateCmd("report_gate", "report gate(s) information", myNl, myAtpgMgr); 
    Cmd *addFaultCmd = new AddFaultCmd("add_fault", "add target fault(s)", myNl, myAtpgMgr); 
    Cmd *runAtpgCmd = new RunAtpgCmd("run_atpg", "run atpg process", myNl, myAtpgMgr); 
    Cmd *reportPatCmd = new ReportPatCmd("report_pattern", "report pattern(s)", myNl, myAtpgMgr); 

    myCmdMgr->regCmd("ATPG", buildCircuitCmd); 
    myCmdMgr->regCmd("ATPG", reportGateCmd); 
    myCmdMgr->regCmd("ATPG", addFaultCmd); 
    myCmdMgr->regCmd("ATPG", runAtpgCmd); 
    myCmdMgr->regCmd("ATPG", reportPatCmd); 
}

void myExec(int argc, char **argv) { 
    try { 
        myOptMgr->parse(argc, argv); 
        string file = myOptMgr->getVal("file"); 

        if(!file.empty()) { 
            if(!cli->openDofile(file.c_str())) { 
                cerr << "Error: connot open file \"" << file << "\"!!\n"; 
                myExit(1); 
            }
        }
        
        cli->readCmd();  
        cout << endl; 
        //myTest(); 

        myExit(0); 
    }
    catch (ArgException &e) { 
        //TODO
        myExit(1); 
    }
} 

void myExit(int status) { 
    delete myOptMgr; 
    delete myCmdMgr; 
    delete myNl; 
    delete myAtpgMgr; 

    exit(status); 
}

