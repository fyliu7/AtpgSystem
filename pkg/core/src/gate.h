#ifndef _CORE_GATE_H_ 
#define _CORE_GATE_H_ 

#include "logic.h"

namespace CoreNs { 

class Gate; 

typedef std::vector<Gate *> GateVec;  

enum GateType { NA = -1,  
                GATE_PI, GATE_PO, 
                GATE_PPI, GATE_PPO, 
                GATE_NOT, 
                GATE_AND, GATE_NAND, GATE_OR, GATE_NOR }; 

class Gate { 
public: 

                  Gate(GateType typ); 
    virtual       ~Gate(); 

    int           id; 
    int           lvl; 
    GateType      type; 
    
    size_t        nfi; 
    size_t        nfo; 
    GateVec       fis; 
    GateVec       fos; 

    ParaValue     gl; 
    ParaValue     gh; 
    ParaValue     fl; 
    ParaValue     fh; 
    Value         val; 

    Value         output_ctr_value; 

    virtual Value GoodEval() const = 0; 
    
}; //Gate

class AndGate : public Gate { 
public: 
          AndGate(): Gate(GATE_AND) {
              output_ctr_value = L; 
          } 
          ~AndGate() {}
    Value GoodEval() const; 
};  

class OrGate : public Gate { 
public: 
          OrGate(): Gate(GATE_OR) { 
              output_ctr_value = H; 
          }  
          ~OrGate() {} 
    Value GoodEval() const; 
}; 

class NandGate : public Gate { 
public: 
          NandGate(): Gate(GATE_NAND) {
              output_ctr_value = H; 
          } 
          ~NandGate() {}
    Value GoodEval() const; 
}; 

class NorGate : public Gate { 
public: 
          NorGate(): Gate(GATE_NOR) {
              output_ctr_value = L; 
          } 
          ~NorGate() {} 
    Value GoodEval() const; 
}; 

class NotGate : public Gate { 
public: 
          NotGate(): Gate(GATE_NOT) {}  
          ~NotGate() {}
    Value GoodEval() const; 
}; 

class PiGate : public Gate { 
public: 
          PiGate(): Gate(GATE_PI) {}  
          ~PiGate() {}
    Value GoodEval() const; 
}; 

class PoGate : public Gate { 
public: 
          PoGate(): Gate(GATE_PO) {}  
          ~PoGate() {} 
    Value GoodEval() const; 
}; 

class PpiGate : public Gate { 
public: 
          PpiGate(): Gate(GATE_PPI) {}  
          ~PpiGate() {}
    Value GoodEval() const; 
}; 

class PpoGate : public Gate { 
public: 
          PpoGate(): Gate(GATE_PPO) {}  
          ~PpoGate() {} 
    Value GoodEval() const; 
}; 

inline Gate::Gate(GateType typ) { 
    type = typ; 
    val  = X; 

    output_ctr_value = X; 
}

inline Gate::~Gate() { 
}

}; //CoreNs

#endif //_CORE_GATE_H_ 
