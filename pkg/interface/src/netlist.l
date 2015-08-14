%{ 
#include "netlist.h" 
#include "netlist.y.hpp"

using namespace std; 

int netlistline = 1; 
%}

NAME    [^ #(),;=\t\n]+ 
COMMENT "//"[^\n]*\n 

%option noyywrap 

%%
"//".*      ; 

"module"    { return MODULE;    }
"endmodule" { return ENDMODULE; }
"assign"    { return ASSIGN;    } 
"input"     { return INPUT;     }
"output"    { return OUTPUT;    } 
"wire"      { return WIRE;      } 

"not"       { return NOT;       }
"and"       { return AND;       }
"or"        { return OR;        } 
"nand"      { return NAND;      } 
"nor"       { return NOR;       }
"dff"       { return DFF;       } 

{NAME}      { 
    strcpy(netlistlval.ychar, netlisttext); 
    return NAME; 
}

"("         { return netlisttext[0];    } 
")"         { return netlisttext[0];    } 
","         { return netlisttext[0];    } 
";"         { return netlisttext[0];    } 
"="         { return netlisttext[0];    } 

\n          { netlistline++;    }
[ \t\v\f]   ; 
.           ;

%%
