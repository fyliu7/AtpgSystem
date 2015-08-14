%{
#define YYPARSE_PARAM param 

#include <cstdio> 
#include <cstring>

#include "netlist.h" 

using namespace std; 
using namespace IntfNs; 

extern char *netlisttext; 
extern int netlistline; 
extern int netlistlex(void); 

void netlistfreeNameList(NameList* names); 
void netlisterror(const char *msg); 

IntfNs::Netlist *nl; 

%} 

%union { 
    char                ychar[IntfNs::NAME_LEN_MAX]; 
    IntfNs::CellType    ycelltype; 
    IntfNs::NameList*   ynamelist; 
}

%token MODULE ENDMODULE ASSIGN INPUT OUTPUT WIRE 
%token NOT AND OR NAND NOR DFF
%token <ychar> NAME 

%type <ycelltype> cell_type 
%type <ynamelist> list 

%start start 

%% 

start 
    : init modules
    ; 

init 
    : { nl = (Netlist *)(param); }
    ; 

modules
    : modules module 
    | module 
    ;

module 
    : MODULE add_module ports body ENDMODULE 
    ; 

add_module 
    : NAME { nl->addModule($1); }
    ;

ports 
    : '(' list ')' ';' { nl->addPorts($2); 
                         netlistfreeNameList($2); }
    | ';'
    ; 

body
    : body net 
    | body assign
    | body cell
    |
    ;

net 
    : INPUT     list ';' { nl->addInputs($2); 
                           netlistfreeNameList($2); }
    | OUTPUT    list ';' { nl->addOutputs($2); 
                           netlistfreeNameList($2); }
    | WIRE      list ';' { nl->addNets($2); 
                           netlistfreeNameList($2); }
    ; 

assign 
    : ASSIGN NAME '=' NAME ';' {}
    ;

cell 
    : cell_type NAME '(' list ')' ';'   { nl->addCell(NULL, $2, $4, $1);  
                                          netlistfreeNameList($4); } 
    ;

cell_type 
    : NOT   { $$ = CELL_NOT;       } 
    | AND   { $$ = CELL_AND;       }
    | OR    { $$ = CELL_OR;        } 
    | NAND  { $$ = CELL_NAND;      } 
    | NOR   { $$ = CELL_NOR;       } 
    | DFF   { $$ = CELL_DFF;       }
    | NAME  { $$ = CELL_MODINST;   }
    ; 

list 
    : list ',' NAME { $$ = new NameList; 
                      strcpy($$->name, $3); 
                      $1->next = $$; 
                      $$->head = $1->head; 
                      $$->next = NULL; 
}
    | NAME          { $$ = new NameList; 
                      strcpy($$->name, $1); 
                      $$->head = $$; 
                      $$->next = NULL; 
}
    |               { $$ = NULL;         }
    ;

%%

void netlistfreeNameList(NameList* names) {
    if(!names) return; 
    NameList *name = names->head; 
    while (name) { 
        NameList *curr = name; 
        name = name->next; 
        delete curr; 
    }
} 

void netlisterror(const char *msg) {
    fprintf(stderr, "**ERROR vlog_fileerror(): ");
    fprintf(stderr, "At line %d. Near `%s'. ", netlistline, netlisttext);
    fprintf(stderr, "%s\n", msg);
} 
