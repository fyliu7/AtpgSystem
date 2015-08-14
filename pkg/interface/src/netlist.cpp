#include <cstdio> 

#include "netlist.h"

using namespace std; 

using namespace IntfNs; 

bool Netlist::parse(const string& filename, const string& top) {
    extern int netlistparse(void *); 
    extern FILE *netlistin; 

    netlistin = fopen(filename.c_str(), "r"); 
    if (!netlistin) { 
        //TODO 
        return false; 
    }

    success_ = true; 
    int res = netlistparse(this); 
    if (res != 0) { 
        fprintf(stderr, "**ERROR Netlist::parse(): wrong input format\n");
    } 
    fclose(netlistin); 

    return success_; 
}

void Netlist::addModule(const char * const name) {
    if(!success_) return; 
    if(getModule(string(name))) { 
        fprintf(stderr, "**ERROR Netlist::addModule(): ");
        fprintf(stderr, "module `%s' already exists\n", name);

        success_ = false; 
        return; 
    }
    
    current_ = new Module(string(name)); 
    module_map_[string(name)] = current_; 
    top_ = current_; //TODO
}

void Netlist::addPorts(NameList * const ports) {
    if(!success_) return; 
    if(!ports) return; 

    NameList *port = ports->head; 
    while (port) { 
        if(current_->getPort(string(port->name))) { 
            string modulename; 
            current_->getName(modulename); 
            fprintf(stderr, "**ERROR Netlist::addPorts(): port ");
            fprintf(stderr, "`%s/%s' ", modulename.c_str(), port->name);
            fprintf(stderr, "already exists\n");

            success_ = false; 
            return; 
        } 
        Port *p = new Port(string(port->name)); 
        current_->addPort(p); 
        port = port->next; 
    }
}

void Netlist::addInputs(NameList * const ins) {
    if(!success_) return; 
    if(!ins) return; 

    NameList *input = ins->head; 
    while (input) { 
        if(current_->getNet(string(input->name))) { 
            //TODO
            success_ = false; 
            return; 
        }
        Net *n = new Net(string(input->name)); 
        current_->addNet(n); 
        addCell(NULL, input->name, NULL, CELL_PI); 
        input = input->next; 
    }
} 

void Netlist::addOutputs(NameList * const outs) {
    if(!success_) return; 
    if(!outs) return; 

    NameList *output = outs->head; 
    while (output) { 
        if(current_->getNet(string(output->name))) { 
            //TODO
            success_ = false; 
            return; 
        }
        Net *n = new Net(string(output->name)); 
        current_->addNet(n); 
        addCell(NULL, output->name, NULL, CELL_PO); 
        output = output->next; 
    }
} 
void Netlist::addNets(NameList * const nets) {
    if(!success_) return; 
    if(!nets) return; 

    NameList *net = nets->head; 
    while (net) { 
        if(current_->getNet(string(net->name))) { 
            success_ = false; 
            return; 
        }
        Net *n = new Net(string(net->name)); 
        current_->addNet(n); 
        net = net->next; 
    }
} 

void Netlist::addCell(const char * const typenm, 
                      const char * const name, 
                      NameList * const nets, 
                      CellType type) {

    if(current_->getCell(string(name))) { 
        //TODO
        success_ = false; 
        return; 
    }

    Net *n; 
    Cell *c; 
    switch(type) { 
        case CELL_PI: {  
            c = new Cell(string(name), type); 
            n = current_->getNet(string(name)); 
            n->setInCell(c); 
            c->addOutNet(n); }
        break; 
        case CELL_PO: {  
            c = new Cell(string(name), type); 
            n = current_->getNet(string(name)); 
            n->addOutCell(c); 
            c->addInNet(n); }
        break; 
        case CELL_DFF: {
            if(!nets) { 
                //TODO 
                success_ = false; 
                return; 
            }
            NameList *net = nets->head; //PPI net 
            n = current_->getNet(string(net->name)); 
            if(!n) { 
                //TODO
                success_ = false; 
                return; 
            }
            c = new Cell(string(name), CELL_PPI); 
            n->setInCell(c); 
            c->addOutNet(n); 
            current_->addCell(c); 
            net = net->next; //PPO net
            n = current_->getNet(string(net->name)); 
            if(!n) { 
                //TODO
                success_ = false; 
                return; 
            }
            c = new Cell(string(name)+string("_O"), CELL_PPO); 
            n->addOutCell(c); 
            c->addInNet(n); }
        break; 
        default: {
            if(!nets) { 
                //TODO
                success_ = false; 
                return; 
            }
            c = new Cell(string(name), type); 
            NameList *net = nets->head; //output net 
            n = current_->getNet(string(net->name)); 
            if(!n) { 
                //TODO
                success_ = false; 
                return; 
            }
            n->setInCell(c); 
            c->addOutNet(n); 
            net = net->next; //input nets
            while(net) { 
                n = current_->getNet(string(net->name)); 
                if(!n) { 
                    //TODO
                    success_ = false; 
                    return; 
                }
                n->addOutCell(c); 
                c->addInNet(n); 
                net = net->next; 
            } }
        break; 
    }    
    current_->addCell(c); 
} 
