/*
 * =====================================================================================
 *
 *       Filename:  sys_cmd.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  09/05/2015 04:35:31 PM
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  xenia-cjen (xc), jonah0604@gmail.com
 *        Company:  LaDS(I), GIEE, NTU
 *
 * =====================================================================================
 */

#ifndef _CLI_SYS_CMD_H_ 
#define _CLI_SYS_CMD_H_

#include "cmdParser.h"

namespace Cli { 

class SysExitCmd : public CmdNs::Cmd { 
public: 
                SysExitCmd(const std::string &name, 
                           const std::string &msg, 
                           CmdParser *cli);  
                ~SysExitCmd(); 
private: 
    bool        run(); 

    CmdParser   *cli_; 
}; 

class SysHistoryCmd : public CmdNs::Cmd { 
public: 
                SysHistoryCmd(const std::string &name, 
                           const std::string &msg, 
                           CmdParser *cli);  
                ~SysHistoryCmd(); 
private: 
    bool        run(); 

    CmdParser   *cli_; 
}; 

class SysHelpCmd : public CmdNs::Cmd { 
public: 
                SysHelpCmd(const std::string &name, 
                           const std::string &msg, 
                           CmdParser *cli);  
                ~SysHelpCmd(); 
private: 
    bool        run(); 

    CmdParser   *cli_; 
}; 

}; //Cli

#endif //_CLI_SYS_CMD_H_
