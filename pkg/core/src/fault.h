#ifndef _CORE_FAULT_H_ 
#define _CORE_FAULT_H_ 

#include "circuit.h" 

namespace CoreNs { 

class Fault; 

typedef std::vector<Fault *> FaultVec; 


class Fault { 
public: 

    enum    Status { UD = 0, DT, AU, RE, AB }; 

            Fault(); 
            Fault(int fgid, Value fv, size_t fp); 
            ~Fault(); 

    int     fgate_id; 
    Value   fval; 
    size_t  fpid; 

    Status  status; 
}; //Fault

class FaultMgr { 
public: 
             ~FaultMgr(); 

    size_t   getFaultNum() const; 
    Fault*   getFault(size_t n) const; 

    void     AddFault(Gate *g, size_t p, Value v); 
    void     AddFault(Gate *g, Value v); 
    void     AddAllFaults(Circuit *cir); 
private: 
    FaultVec faults_; 
}; //FaultMgr

inline Fault::Fault() { 
    status = UD; 
}

inline Fault::~Fault() { 
}

inline Fault::Fault(int fgid, Value fv, size_t fp) { 
    fgate_id = fgid;     
    fval = fv; 
    fpid = fp; 

    status = UD; 
} 

inline FaultMgr::~FaultMgr() { 
    for(size_t i=0; i<faults_.size(); i++) 
        delete faults_[i]; 
}

inline size_t FaultMgr::getFaultNum() const { 
    return faults_.size(); 
}

inline Fault *FaultMgr::getFault(size_t n) const { 
    return (n<getFaultNum())?faults_[n]:NULL; 
}

};  //CoreNs

#endif //_CORE_FAULT_H_
