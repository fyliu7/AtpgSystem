#ifndef _INTF_MODULE_H_ 
#define _INTF_MODULE_H_ 

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "core/src/circuit.h"

namespace IntfNs {

class Module; 
class Cell; 
class Net; 
class Port; 

typedef std::vector<Cell *> CellVec; 
typedef std::vector<Net *> NetVec; 
typedef std::vector<Port *> PortVec; 

typedef std::map<std::string, Cell *> CellMap;  
typedef std::map<std::string, Net *> NetMap;   

enum CellType { CELL_MODINST = 0, 
                CELL_PI, CELL_PO, 
                CELL_PPI, CELL_PPO, 
                CELL_DFF, 
                CELL_NOT, 
                CELL_AND, CELL_NAND, CELL_OR, CELL_NOR, 
                CELL_XOR, CELL_XNOR }; 

enum PortType { PORT_NA = -1, 
                PORT_INPUT, PORT_OUTPUT}; 

class Module { 
public: 
        Module(const std::string& name); 
        ~Module(); 

    void getName(std::string& ret) const; 

    size_t getPortNum() const; 
    Port *getPort(const size_t& n) const; 
    Port *getPort(const std::string& name) const; 
    void addPort(Port * const p); 
    void clearPorts(); 
    
    size_t getCellNum() const; 
    Cell *getCell(const size_t& n) const; 
    Cell *getCell(const std::string& name) const; 
    void addCell(Cell * const c); 
    void clearCells(); 

    size_t getNetNum() const; 
    Net *getNet(const size_t& n) const; 
    Net *getNet(const std::string& name) const; 
    void addNet(Net * const n); 
    void clearNets(); 

protected: 
    //int id_; 
    std::string name_; 

    PortVec port_vec_; 

    CellMap cell_map_; 
    NetMap net_map_; 
    
}; //Module

class Cell {
public: 
        Cell(CellType type); 
        Cell(const std::string& name, CellType type); 
        Cell(const std::string& type_name); 
        Cell(const std::string& name, const std::string& type_name); 
        ~Cell(); 

    void getName(std::string& ret) const; 
    int getLevel() const; 
    CellType getType() const; 
    CoreNs::Gate *getGate() const; 

    void setLevel(int lvl); 
    void setGate(CoreNs::Gate* g); 

    size_t getPortNum() const; 
    Port *getPort(const size_t& n) const; 
    Port *getPort(const std::string& name) const; 
    void addPort(Port* p); 
    void clearPorts(); 

    void addInNet(Net *n); 
    void addOutNet(Net *n); 

    size_t getInNetNum() const; 
    Net *getInNet(const size_t& n) const; 
    size_t getOutNetNum() const; 
    Net *getOutNet(const size_t& n) const; 

    void getFaninCell(CellVec& ret) const; 
    bool getFanoutCell(const size_t& n, CellVec& ret) const; 

protected: 
    //int id_; 
    std::string name_; 
    std::string type_name_; 
    CoreNs::Gate *gate_;

    //Cell *top_; 
    int lvl_;

    CellType type_; 

    NetVec in_nets_; 
    NetVec out_nets_; 

    PortVec port_vec_; 
    
}; //Cell

class Net { 
public: 
        Net(const std::string& name);  
        ~Net(); 

    void getName(std::string& ret) const; 
    
    Cell *getInCell() const; 
    void getOutCells(CellVec& ret) const; 

    void setInCell(Cell *c); 
    void addOutCell(Cell *c); 

    //void addInPort(Port *p); 
    //void addOutPort(Port *p); 

protected: 
    //int id_; 
    std::string name_; 

    Port *in_port_; 
    PortVec out_ports_; 

    Cell *in_cell_; 
    CellVec out_cells_; 
}; 

class Port { 
public: 

        //Port(PortType type); 
        Port(const std::string& name); 
        Port(const std::string& name, PortType type); 

    void setType(PortType type); 
    
    void getName(std::string& ret) const; 

    bool SetInNet(Net *n); 
    bool SetOutNet(Net *n); 

protected: 
    std::string name_; 

    PortType type_; 

    Net *in_net_; 
    Net *out_net_; 
}; //Port 

inline Module::Module(const std::string& name) {
    name_ = name; 
}

inline Module::~Module() {
    clearPorts(); 
    clearCells(); 
    clearNets(); 
}

inline void Module::getName(std::string& ret) const {
    ret = name_; 
} 

inline size_t Module::getPortNum() const {
    return port_vec_.size(); 
}

inline Port *Module::getPort(const size_t& n) const {
    return port_vec_[n]; 
}

inline Port *Module::getPort(const std::string& name) const {
    Port *ret = NULL; 
    for(size_t n=0; n<port_vec_.size(); n++) { 
        std::string portname; 
        port_vec_[n]->getName(portname); 
        if(portname==name)
            ret = port_vec_[n]; 
    }
    return ret; 
}

inline void Module::addPort(Port * const p) {
    port_vec_.push_back(p); 
}

inline void Module::clearPorts() {
    for(size_t n=0; n<port_vec_.size(); n++) 
        delete port_vec_[n]; 
    port_vec_.clear(); 
}

inline size_t Module::getCellNum() const {
    return cell_map_.size(); 
}

inline Cell *Module::getCell(const size_t& n) const {
    if (n>=getCellNum()) return NULL; 

    CellMap::const_iterator it = cell_map_.begin(); 
    std::advance(it, n); 
    return it->second; 
}

inline Cell *Module::getCell(const std::string& name) const { 
    CellMap::const_iterator it = cell_map_.find(name); 
    return (it==cell_map_.end())?NULL:it->second; 
}

inline void Module::addCell(Cell * const c) {
    std::string cname; 
    c->getName(cname); 
    cell_map_[cname] = c; 
}

inline void Module::clearCells() {
    for(size_t n=0; n<getCellNum(); n++) { 
        Cell *c = getCell(n);
        delete c; 
    }    
    cell_map_.clear(); 
}

inline size_t Module::getNetNum() const { 
    return net_map_.size(); 
}

inline Net *Module::getNet(const size_t& n) const {
    if(n>=getNetNum()) return NULL; 

    NetMap::const_iterator it = net_map_.begin(); 
    std::advance(it, n); 
    return it->second; 
}

inline Net *Module::getNet(const std::string& name) const {
    NetMap::const_iterator it = net_map_.find(name); 
    return (it==net_map_.end())?NULL:it->second; 
}

inline void Module::addNet(Net * const n) {
    std::string nname; 
    n->getName(nname);  
    net_map_[nname] = n; 
}

inline void Module::clearNets() {
    for(size_t i=0; i<getNetNum(); i++) { 
        Net* n = getNet(i); 
        delete n; 
    }
    net_map_.clear(); 
}

inline Cell::Cell(const std::string& name, CellType type) { 
    name_ = name; 
    type_ = type; 
    lvl_ = -1; 
}

inline Cell::~Cell(){
}

inline void Cell::getName(std::string& ret) const {
    ret = name_; 
}

inline int Cell::getLevel() const {
    return lvl_; 
}

inline CoreNs::Gate *Cell::getGate() const { 
    return gate_;  
}

inline CellType Cell::getType() const { 
    return type_; 
}

inline void Cell::setLevel(int lvl) {
    lvl_ = lvl; 
}

inline void Cell::setGate(CoreNs::Gate* g) { 
    gate_ = g;
}

inline void Cell::addInNet(Net *n) {
    in_nets_.push_back(n); 
}

inline void Cell::addOutNet(Net *n) { 
    out_nets_.push_back(n); 
}

inline size_t Cell::getInNetNum() const {
    return in_nets_.size(); 
} 
 
inline Net *Cell::getInNet(const size_t& n) const { 
    return (n<getInNetNum())?in_nets_[n]:NULL; 
}
 
inline size_t Cell::getOutNetNum() const { 
    return out_nets_.size(); 
} 

inline Net *Cell::getOutNet(const size_t& n) const { 
    return (n<getOutNetNum())?out_nets_[n]:NULL;
} 

inline void Cell::getFaninCell(CellVec& ret) const { 
    for(size_t i=0; i<getInNetNum(); i++) { 
        Net *n = getInNet(i); 
        ret.push_back(n->getInCell()); 
    }
}
 
inline bool Cell::getFanoutCell(const size_t& n, CellVec& ret) const { 
    Net *net = getOutNet(n); 
    if(!net) return false; 

    net->getOutCells(ret); 
    return true; 
} 

inline Net::Net(const std::string& name) {
    name_ = name; 
}

inline Net::~Net() { 
}

inline void Net::getName(std::string& ret) const { 
    ret = name_; 
}

inline Cell *Net::getInCell() const {
    return in_cell_; 
}
 
inline void Net::getOutCells(CellVec& ret) const { 
    ret = out_cells_; 
} 

inline void Net::setInCell(Cell *c) {
    in_cell_ = c; 
}

inline void Net::addOutCell(Cell *c) { 
    out_cells_.push_back(c); 
}

inline Port::Port(const std::string& name) {
    name_ = name;  
}

inline void Port::setType(PortType type) {
    type_ = type; 
}

inline void Port::getName(std::string& ret) const { 
    ret = name_;
}

}; //IntfNs 

#endif //_INTF_MODULE_H_ 
