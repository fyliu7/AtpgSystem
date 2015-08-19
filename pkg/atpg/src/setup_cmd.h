#ifndef _ATPG_SETUP_CMD_H_ 
#define _ATPG_SETUP_CMD_H_

#include "cmd/src/cmd.h"
#include "interface/src/netlist.h"

namespace AtpgNs { 

class ReadNetlistCmd : public CmdNs::Cmd { 
public: 
            ReadNetlistCmd(const std::string &name, 
                           const std::string &msg, 
                           IntfNs::Netlist *nl); 
            ~ReadNetlistCmd(); 
private: 
    bool    run();  

    IntfNs::Netlist *nl_; 
}; //ReadNetlsitCmd

class ReportCellCmd : public CmdNs::Cmd { 
public: 
            ReportCellCmd(const std::string &name, 
                          const std::string &msg, 
                          IntfNs::Netlist *nl); 
            ~ReportCellCmd(); 

private: 
    bool    run(); 

    IntfNs::Netlist *nl_; 
}; //ReportCellCmd

}; //AtpgNs

#endif //_ATPG_SETUP_CMD_H_
