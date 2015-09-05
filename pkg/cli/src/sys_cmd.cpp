/*
 * =====================================================================================
 *
 *       Filename:  sys_cmd.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  09/05/2015 04:47:11 PM
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  xenia-cjen (xc), jonah0604@gmail.com
 *        Company:  LaDS(I), GIEE, NTU
 *
 * =====================================================================================
 */

#include "sys_cmd.h"

using namespace std; 

using namespace Cli; 
using namespace CmdNs; 

SysExitCmd::SysExitCmd(const string &name, 
                       const string &msg, 
                       CmdParser *cli) : Cmd(name, msg) { 

    cli_ = cli; 
}

SysExitCmd::~SysExitCmd() {
}

bool SysExitCmd::run() { 
   cli_->getCmdMgr()->setExit(); 

   return true;   
}

SysHistoryCmd::SysHistoryCmd(const string &name, 
                       const string &msg, 
                       CmdParser *cli) : Cmd(name, msg) { 

    cli_ = cli; 
}

SysHistoryCmd::~SysHistoryCmd() {
}

bool SysHistoryCmd::run() { 
    //TODO
}

SysHelpCmd::SysHelpCmd(const string &name, 
                       const string &msg, 
                       CmdParser *cli) : Cmd(name, msg) { 

    cli_ = cli; 
}

SysHelpCmd::~SysHelpCmd() {
}

bool SysHelpCmd::run() { 
    //TODO
}
