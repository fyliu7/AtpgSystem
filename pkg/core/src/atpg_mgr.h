#ifndef _CORE_ATPG_MGR_H_ 
#define _CORE_ATPG_MGR_H_ 

#include "atpg.h"

namespace CoreNs { 

typedef std::vector<Atpg*>  AtpgVec; 

class AtpgMgr {
public: 
                AtpgMgr(); 
                ~AtpgMgr(); 

    bool        Run(); 

    Circuit     *cir; 

    FaultMgr    *f_mgr; 
    PatternMgr  *pat_mgr; 

private: 
    AtpgVec     atpgs_; 
}; //AtpgMgr
 
inline AtpgMgr::AtpgMgr() {
    cir        = NULL; 

    f_mgr      = NULL; 
    pat_mgr    = NULL; 
}

inline AtpgMgr::~AtpgMgr() {
    //TODO
    if(cir) delete cir; 

    if(f_mgr) delete f_mgr; 
    if(pat_mgr) delete pat_mgr; 

    for(size_t i=0; i<atpgs_.size(); i++) 
        delete atpgs_[i]; 
}

};  //CoreNs

#endif //_CORE_CMD_MGR_H_
