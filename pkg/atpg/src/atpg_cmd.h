#ifndef _ATPG_ATPG_CMD_H_ 
#define _ATPG_ATPG_CMD_H_ 

#include "cmd/src/cmd.h"
#include "interface/src/netlist.h"
#include "core/src/atpg_mgr.h"

namespace AtpgNs { 

class BuildCircuitCmd : public CmdNs::Cmd { 
public: 
            BuildCircuitCmd(const std::string &name, 
                            const std::string &msg, 
                            IntfNs::Netlist *nl, 
                            CoreNs::AtpgMgr *atpg_mgr); 
            ~BuildCircuitCmd(); 
private: 
    bool    run(); 

    IntfNs::Netlist *nl_; 
    CoreNs::AtpgMgr *atpg_mgr_; 
}; 

class ReportCircuitCmd : public CmdNs::Cmd { 
public: 
            ReportCircuitCmd(const std::string &name, 
                             const std::string &msg, 
                             IntfNs::Netlist *nl, 
                             CoreNs::AtpgMgr *atpg_mgr); 
            ~ReportCircuitCmd(); 
private: 
    bool    run();  

    IntfNs::Netlist *nl_; 
    CoreNs::AtpgMgr *atpg_mgr_; 
}; //ReportCircuitCmd

class ReportGateCmd : public CmdNs::Cmd { 
public: 
            ReportGateCmd(const std::string &name, 
                          const std::string &msg, 
                          IntfNs::Netlist *nl, 
                          CoreNs::AtpgMgr *atpg_mgr); 
            ~ReportGateCmd(); 
private: 
    bool    run();  

    void    reportGate() const; 

    IntfNs::Netlist *nl_; 
    CoreNs::AtpgMgr *atpg_mgr_; 
}; //ReportCircuitCmd

class AddFaultCmd : public CmdNs::Cmd { 
public: 
            AddFaultCmd(const std::string &name, 
                        const std::string &msg, 
                        IntfNs::Netlist *nl, 
                        CoreNs::AtpgMgr *atpg_mgr); 
            ~AddFaultCmd(); 
private: 
    bool    run();  

    void    addFault(); 

    IntfNs::Netlist *nl_; 
    CoreNs::AtpgMgr *atpg_mgr_; 
}; //AddFaultCmd

class ReportFaultCmd : public CmdNs::Cmd { 
public: 
            ReportFaultCmd(const std::string &name, 
                        const std::string &msg, 
                        IntfNs::Netlist *nl, 
                        CoreNs::AtpgMgr *atpg_mgr); 
            ~ReportFaultCmd(); 
private: 
    bool    run();  

    void    repFault(); 

    IntfNs::Netlist *nl_; 
    CoreNs::AtpgMgr *atpg_mgr_; 
}; //ReportFaultCmd

class RunAtpgCmd : public CmdNs::Cmd { 
public: 
            RunAtpgCmd(const std::string &name, 
                       const std::string &msg, 
                       IntfNs::Netlist *nl, 
                       CoreNs::AtpgMgr *atpg_mgr); 
            ~RunAtpgCmd(); 
private: 
    bool    run(); 
    
    IntfNs::Netlist *nl_; 
    CoreNs::AtpgMgr *atpg_mgr_; 
}; //RunAtpgCmd

class ReportPatCmd : public CmdNs::Cmd { 
public: 
            ReportPatCmd(const std::string &name, 
                          const std::string &msg, 
                          IntfNs::Netlist *nl, 
                          CoreNs::AtpgMgr *atpg_mgr); 
            ~ReportPatCmd(); 
private: 
    bool    run(); 

    IntfNs::Netlist *nl_; 
    CoreNs::AtpgMgr *atpg_mgr_; 
}; //ReportPatCmd

}; //AtpgNs

#endif //_ATPG_ATPG_CMD_H_
