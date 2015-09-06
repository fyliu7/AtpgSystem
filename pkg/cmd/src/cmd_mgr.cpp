/*
 * =====================================================================================
 *
 *       Filename:  cmd_mgr.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2015/09/03 02:57:34
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  xenia-cjen (xc), jonah0604@gmail.com
 *        Company:  LaDS(I), GIEE, NTU
 *
 * =====================================================================================
 */

#include "cmd.h" 

using namespace std; 

using namespace CmdNs; 

bool CmdMgr::regCmd(const string &cat, Cmd *cmd) { 
    CatMapIter iter = getCat(cat); 
    pair<Catagory::iterator, bool> ret; 

    ret = (iter->second).insert(cmd->getName()); 
    if(!ret.second) return false; 

    cmd_map_.insert(pair<string, Cmd *>(cmd->getName(), cmd)); 
    return true; 
} 

Cmd *CmdMgr::getCmd(const std::string& name) const { 
    CmdMap::const_iterator iter = cmd_map_.find(name); 

    return (iter!=cmd_map_.end())?(iter->second):NULL;
}

void CmdMgr::getMatchCmds(const std::string &cmdStr, CmdVec& ret) const { 
    //TODO
} 

CmdMgr::Result CmdMgr::exec(const string &cmdStr) { 
    Result res = NOP; 

    vector<string> args; 
    Cmd *cmd = parseCmd(cmdStr, args); 

    if(!cmd) return NOT_EXIST; 
    res = (cmd->exec(args))?SUCCESS:FAIL; 
    if(is_exit_) return EXIT; 

    return res; 
}

CatMapIter CmdMgr::getCat(const string &cat) { 
    pair<CatMapIter, bool> ret; 

    ret = cat_map_.insert(pair<string, Catagory>(cat, Catagory())); 
    
    return ret.first; 
}

Cmd *CmdMgr::parseCmd(const string &cmdStr, 
                      vector<string> &ret) { 

    string cmdl = cmdStr; 
    //TODO: tokenize the string "cmdl" into 
    //      command & argument(s), and assign 
    //      the command string to the "cmdl". 

    string delim = " "; 
    size_t curr_pos; 
    size_t next_pos = -1; 
    do { 
        curr_pos = next_pos + 1; 
        next_pos = cmdl.find_first_of(delim, curr_pos); 
        ret.push_back(cmdl.substr(curr_pos, next_pos-curr_pos)); 
    } while (next_pos!=string::npos); 

    //ret.insert(ret.begin(), cmdl); 
    cmdl = ret[0]; 

    return getCmd(cmdl); 
} 
