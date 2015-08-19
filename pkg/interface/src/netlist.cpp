#include <cstdio> 
#include <queue>
#include <algorithm>

#include "netlist.h"

using namespace std; 

using namespace IntfNs; 
using namespace CoreNs; 

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
        success_ = false; 
    } 
    fclose(netlistin); 

    return success_; 
}

bool Netlist::build(Circuit* cir) {
    CellVec cells; 
    levelize(cells); 
    cir->ntotlvl = cells[cells.size()-1]->getLevel(); 
    
    size_t pi_cnt       = 0, 
           ppi_cnt      = 0, 
           po_cnt       = 0, 
           logic_cnt    = 0;  

    for(size_t n=0; n<cells.size(); n++) { 
        Cell *c = cells[n];   
        
        Gate *g; 
        switch(c->getType()) { 
        case CELL_AND:  
            g = new AndGate; 
            logic_cnt++; 
            break; 
        case CELL_OR:  
            g = new OrGate; 
            logic_cnt++; 
            break; 
        case CELL_NAND:  
            g = new NandGate; 
            logic_cnt++; 
            break; 
        case CELL_NOR:  
            g = new NorGate; 
            logic_cnt++; 
            break; 
        case CELL_PI: 
            g = new PiGate; 
            cir->pis.push_back(n); 
            pi_cnt++; 
            break; 
        case CELL_PPI: 
            g = new PpiGate; 
            cir->ppis.push_back(n); 
            ppi_cnt++; 
            break; 
        case CELL_PO: 
            g = new PoGate; 
            cir->pos.push_back(n); 
            po_cnt++; 
            break; 
        case CELL_PPO: 
            g = new PpoGate; 
            cir->ppos.push_back(n); 
            break; 
        default: 
            break; 
        }

        g->id = n; 
        g->lvl = c->getLevel(); 

        cir->gates.push_back(g); 
        c->setGate(g);
    }

    cir->npi = pi_cnt; 
    cir->nppi = ppi_cnt; 
    cir->npo = po_cnt; 
    cir->nlogic = logic_cnt; 

    cir->ntotgate = cells.size(); 

    for(size_t n=0; n<cells.size(); n++) { 
        Cell *c = cells[n];   
        Gate *g = cells[n]->getGate(); 

        CellVec fanins, fanouts; 
        c->getFaninCell(fanins); 
        for (size_t i=0; i<c->getOutNetNum(); i++) { 
            c->getFanoutCell(i, fanouts); 
        }
        
        for (size_t i=0; i<fanins.size(); i++)  
            g->fis.push_back(fanins[i]->getGate()); 
        g->nfi = g->fis.size(); 

        for (size_t i=0; i<fanouts.size(); i++)  
            g->fos.push_back(fanouts[i]->getGate());  
        g->nfo = g->fos.size(); 
    }

    return true; 
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
            string modulename; 
            current_->getName(modulename); 
            fprintf(stderr, "**ERROR Netlist::addInputs(): input ");
            fprintf(stderr, "`%s/%s' ", modulename.c_str(), input->name);
            fprintf(stderr, "net already exists\n");
            success_ = false; 
            return; 
        }
        Net *n = new Net(string(input->name)); 
        current_->addNet(n); 
        addCell(NULL, input->name, NULL, CELL_PI); 

        Port *p = current_->getPort(string(input->name)); 
        if(!p) { 
            fprintf(stderr, "**ERROR Netlist::addInputs(): ");
            fprintf(stderr, "port `%s' not exists\n", input->name);
            success_ = false; 
            return; 
        }
        p->setType(PORT_INPUT); 

        input = input->next; 
    }
} 

void Netlist::addOutputs(NameList * const outs) {
    if(!success_) return; 
    if(!outs) return; 

    NameList *output = outs->head; 
    while (output) { 
        if(current_->getNet(string(output->name))) { 
            string modulename; 
            current_->getName(modulename); 
            fprintf(stderr, "**ERROR Netlist::addOutputs(): output ");
            fprintf(stderr, "`%s/%s' ", modulename.c_str(), output->name);
            fprintf(stderr, "net already exists\n");
            success_ = false; 
            return; 
        }
        Net *n = new Net(string(output->name)); 
        current_->addNet(n); 
        addCell(NULL, output->name, NULL, CELL_PO); 

        Port *p = current_->getPort(string(output->name)); 
        if(!p) { 
            fprintf(stderr, "**ERROR Netlist::addOutputs(): ");
            fprintf(stderr, "port `%s' not exists\n", output->name);
            success_ = false; 
            return; 
        }
        p->setType(PORT_OUTPUT); 

        output = output->next; 
    }
} 

void Netlist::addNets(NameList * const nets) {
    if(!success_) return; 
    if(!nets) return; 

    NameList *net = nets->head; 
    while (net) { 
        if(current_->getNet(string(net->name))) { 
            fprintf(stderr, "**ERROR Netlist::addNet(): ");
            fprintf(stderr, "net `%s' already exists\n", net->name);
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
        fprintf(stderr, "**ERROR Netlist::addCell(): ");
        fprintf(stderr, "cell `%s' already exists\n", name);
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
                fprintf(stderr, "**ERROR Netlist::addCell(): ");
                fprintf(stderr, "empty net list\n");
                success_ = false; 
                return; 
            }
            NameList *net = nets->head; //PPI net 
            n = current_->getNet(string(net->name)); 
            if(!n) { 
                fprintf(stderr, "**ERROR Netlist::addCell(): ");
                fprintf(stderr, "net `%s' not exists\n", net->name);
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
                fprintf(stderr, "**ERROR Netlist::addCell(): ");
                fprintf(stderr, "net `%s' not exists\n", net->name);
                success_ = false; 
                return; 
            }
            c = new Cell(string(name)+string("_O"), CELL_PPO); 
            n->addOutCell(c); 
            c->addInNet(n); }
        break; 
        default: {
            if(!nets) { 
                fprintf(stderr, "**ERROR Netlist::addCell(): ");
                fprintf(stderr, "empty net list\n");
                success_ = false; 
                return; 
            }
            c = new Cell(string(name), type); 
            NameList *net = nets->head; //output net 
            n = current_->getNet(string(net->name)); 
            if(!n) { 
                fprintf(stderr, "**ERROR Netlist::addCell(): ");
                fprintf(stderr, "net `%s' not exists\n", net->name);
                success_ = false; 
                return; 
            }
            n->setInCell(c); 
            c->addOutNet(n); 
            net = net->next; //input nets
            while(net) { 
                n = current_->getNet(string(net->name)); 
                if(!n) { 
                    fprintf(stderr, "**ERROR Netlist::addCell(): ");
                    fprintf(stderr, "net `%s' not exists\n", net->name);
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

bool myGateComp(Cell*, Cell*); 
void Netlist::levelize(CellVec& cells) {
    queue<Cell*> q_cells; 
    for(size_t i=0; i<top_->getCellNum(); i++) { 
        Cell *c = top_->getCell(i); 
        if(c->getType()==CELL_PI || c->getType()==CELL_PPI) { 
            c->setLevel(0); 
            q_cells.push(c); 
        }
        cells.push_back(c); 
    }
    
    CellVec fanouts; 
    while (!q_cells.empty()) { 
        Cell *c = q_cells.front(); 
        q_cells.pop(); 

        fanouts.clear(); 
        for(size_t i=0; i<c->getOutNetNum(); i++) 
            c->getFanoutCell(i, fanouts); 

        for(size_t i=0; i<fanouts.size(); i++) { 
            Cell *fo = fanouts[i]; 
            //if(fo->getLevel()==-1) 
                q_cells.push(fo); 
            if(fo->getLevel()<c->getLevel()+1) fo->setLevel(c->getLevel()+1); 
        }
    }
    std::sort(cells.begin(), cells.end(), myGateComp); 
}

bool myGateComp(Cell* c1, Cell* c2) {
    return (c1->getLevel() < c2->getLevel()); 
} 
