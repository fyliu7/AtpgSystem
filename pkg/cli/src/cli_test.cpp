/****************************************************************************
  FileName     [ main.cpp ]
  PackageName  [ main ]
  Synopsis     [ Define main() function ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2007-2011 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/
#include <string.h>
#include <cstdlib>
#include "cmdParser.h"

using namespace std;

using namespace CmdNs; 
using namespace Cli; 

static void
usage()
{
   cout << "Usage: cmdReader [ -file < doFile > ]" << endl;
}

static void
myexit()
{
   usage();
   exit(-1);
}

int
main(int argc, char** argv)
{
   CmdMgr cmdMgr; 
   CmdParser cmd(&cmdMgr);
   ifstream dof;

   if (argc == 3) {  // -file <doFile>
      if (strcmp(argv[1], "-file") == 0) {
         if (!cmd.openDofile(argv[2])) {
            cerr << "Error: cannot open file \"" << argv[2] << "\"!!\n";
            myexit();
         }
      }
      else {
         cerr << "Error: unknown argument \"" << argv[1] << "\"!!\n";
         myexit();
      }
   }
   else if (argc != 1) {
      cerr << "Error: illegal number of argument (" << argc << ")!!\n";
      myexit();
   }

   cmd.readCmd();  // press "Ctrl-d" to break
   cout << endl;  // a blank line between each command

   return 0;
}
