#include "gate.h" 

using namespace std; 

using namespace CoreNs; 

Value AndGate::GoodEval() const { 
    vector<Value> vals; 
    for (size_t i=0; i<fis.size(); i++) 
        vals.push_back(fis[i]->val); 
    return EvalAndN(vals); 
}

Value OrGate::GoodEval() const { 
    vector<Value> vals; 
    for (size_t i=0; i<fis.size(); i++) 
        vals.push_back(fis[i]->val); 
    return EvalOrN(vals); 
}

Value NandGate::GoodEval() const { 
    vector<Value> vals; 
    for (size_t i=0; i<fis.size(); i++) 
        vals.push_back(fis[i]->val); 
    return EvalNandN(vals); 
}

Value NorGate::GoodEval() const { 
    vector<Value> vals; 
    for (size_t i=0; i<fis.size(); i++) 
        vals.push_back(fis[i]->val); 
    return EvalNorN(vals); 
}

Value NotGate::GoodEval() const { 
    return EvalNot(fis[0]->val); 
}

Value PiGate::GoodEval() const { 
    return val; 
}

Value PpiGate::GoodEval() const { 
    return val; 
}

Value PoGate::GoodEval() const { 
    return fis[0]->val; 
}

Value PpoGate::GoodEval() const { 
    return fis[0]->val; 
}
