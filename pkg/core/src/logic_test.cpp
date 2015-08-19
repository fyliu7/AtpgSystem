#include "logic.h" 

using namespace std; 
using namespace CoreNs; 

int main() {

cout<<"#AND(H, L) = "<<(size_t)EvalAndN(2, H, L)<<"\t\tAns: 0/L\n"; 
cout<<"#AND(H, H, X) = "<<(size_t)EvalAndN(3, H, H, X)<<"\t\tAns: 2/X\n"; 
cout<<"#OR(H, H, X) = "<<(size_t)EvalOrN(3, H, H, X)<<"\t\tAns: 1/H\n"; 

    return 0; 
}
