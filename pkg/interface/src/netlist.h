#ifndef _INTF_NETLIST_H_ 
#define _INTF_NETLIST_H_

#include "module.h" 

namespace IntfNs { 

const int NAME_LEN_MAX = 256; 

//typedef std::vector<Module *> ModuleVec; 
typedef std::map<std::string, Module *> ModuleMap; 

struct NameList { 
    char name[NAME_LEN_MAX]; 
    NameList *next; 
    NameList *head; 
}; //NameList

class Netlist { 
public: 
        Netlist(); 
        ~Netlist(); 

    bool parse(const std::string& filename, const std::string& top); 
    bool build(CoreNs::Circuit* cir); 

    Module* getModule(const std::string& name) const; 
    Module* getTop() const; 

    void addModule(const char * const name); 
    void addPorts(NameList * const ports); 
    void addInputs(NameList * const ins); 
    void addOutputs(NameList * const outs); 
    void addNets(NameList * const nets); 
    void addCell(const char * const typenm, 
                 const char * const name, 
                 NameList * const nets, 
                 CellType type = CELL_MODINST);  
    
protected:  
    void levelize(CellVec& cells);  

    std::string name_; 

    Module *top_; 
    Module *current_; 

    bool success_; 

    ModuleMap module_map_; 
}; //Netlist 

inline Netlist::Netlist() {
}

inline Netlist::~Netlist() {
    ModuleMap::iterator it = module_map_.begin(); 
    for (; it!=module_map_.end(); ++it)  
        delete it->second; 
} 

inline Module *Netlist::getModule(const std::string& name) const { 
    ModuleMap::const_iterator it = module_map_.find(name); 
    return (it == module_map_.end())?NULL:it->second; 
}

inline Module *Netlist::getTop() const { 
    return top_; 
}

}; //IntfNs
#endif //_INTF_NETLIST_H_
