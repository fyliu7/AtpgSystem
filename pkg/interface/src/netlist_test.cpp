#include <cstdio> 
#include <cstdlib> 

#include "netlist.h" 

using namespace std; 

using namespace IntfNs; 

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "**ERROR main(): please provide input verilog\n"); 
        exit(-1); 
    }

    Netlist *nl_ = new Netlist(); 
    if (!nl_->parse(string(argv[1]), string())) {
        fprintf(stderr, "**ERROR main(): verilog parser failed\n"); 
        exit(-1); 
    }

    cout << "  netlist info. \n"; 
    cout << "    #ports:\t\t"; 
    cout << nl_->getTop()->getPortNum() << endl; 
    cout << "    #cells:\t\t"; 
    cout << nl_->getTop()->getCellNum() << endl; 
    cout << "    #nets:\t\t"; 
    cout << nl_->getTop()->getNetNum() << endl; 
    
    //nl_->build(); 

    delete nl_; 
    return 0; 
} 
