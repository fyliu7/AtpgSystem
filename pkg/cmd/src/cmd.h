#ifndef _CMD_CMD_H_ 
#define _CMD_CMD_H_

#include "opt_mgr.h"

namespace CmdNs { 

class Cmd; 

typedef std::set<std::string>                           Catagory; 
typedef std::map<std::string, Catagory>                 CatMap; 
typedef std::map<std::string, Cmd *>                    CmdMap; 
typedef std::vector<Cmd *>                              CmdVec; 
typedef CatMap::iterator                                CatMapIter; 
typedef CmdMap::iterator                                CmdMapIter; 

class Cmd { 
public: 
                        Cmd(const std::string &name, 
                            const std::string &msg); 
    virtual             ~Cmd(); 

            bool        exec(std::vector<std::string> &args); 
            std::string getName() const; 

    OptMgr              *opt_mgr_; 
protected: 
    virtual bool        run() = 0; 
}; //Cmd

class CmdMgr { 
public: 
            CmdMgr();  
            ~CmdMgr(); 
 
    enum    Result { EXIT = -1, SUCCESS, FAIL, NOT_EXIST, NOP };

    bool    regCmd(const std::string &cat, Cmd *cmd); 

    Cmd     *getCmd(const std::string &name) const; 
    void    getMatchCmds(const std::string &cmdStr, CmdVec& ret) const; 

    void    setExit(); 

    Result  exec(const std::string &cmdStr);     

private:  
    CatMapIter  getCat(const std::string &cat); 
    Cmd         *parseCmd(const std::string &cmdStr, 
                          std::vector<std::string> &ret); 

    bool        is_exit_; 

    CatMap      cat_map_; 
    CmdMap      cmd_map_; 
}; //CmdMgr

inline Cmd::Cmd(const std::string &name, const std::string &msg) { 
    opt_mgr_ = new OptMgr(name, msg); 
}

inline Cmd::~Cmd() { 
    delete opt_mgr_; 
}

inline CmdMgr::CmdMgr() {
    is_exit_ = false; 
}

inline CmdMgr::~CmdMgr() {
    CmdMapIter it = cmd_map_.begin(); 

    for(; it!=cmd_map_.end(); it++) 
        delete it->second; 
}
inline void CmdMgr::setExit() { 
    is_exit_ = true; 
}

inline std::string Cmd::getName() const { 
    return opt_mgr_->getName(); 
}
}; //CmdNs 

#endif //_CMD_CDM_H_ 
