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

CmdMgr::Result CmdMgr::exec(const string &cmdStr) { 
    //TODO
    return NOP; 
}

CatMapIter CmdMgr::getCat(const string &cat) { 
    pair<CatMapIter, bool> ret; 

    ret = cat_map_.insert(pair<string, Catagory>(cat, Catagory())); 
    
    return ret.first; 
}

Cmd *CmdMgr::parseCmd(const string &cmdStr) { 
    //TODO 
    return NULL; 
} 
