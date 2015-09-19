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

//void CmdMgr::getMatchCmds(const std::string &cmdStr, CmdVec& ret) const {
void CmdMgr::getMatchCmds(const std::string &cmdStr, vector<string>& ret) const {	
    //TODO
	for (CmdMap::const_iterator it = cmd_map_.begin(); it!=cmd_map_.end(); ++it){
		if(!cmdStr.compare(0,cmdStr.size()-1,it->first,0,cmdStr.size()-1)){
			ret.push_back(it->first);
		}	
	}
	
//test	
/*	CmdMap::const_iterator itbegin = cmd_map_.begin();
	CmdMap::const_iterator itend = cmd_map_.end();
	if(itbegin==itend){
	ret.push_back("match1");
	ret.push_back("match2");
	ret.push_back("match3");
	ret.push_back("match4");
	ret.push_back("match5");
	ret.push_back("match6");
	ret.push_back("match7");
	ret.push_back("match8");
	ret.push_back("match9");
	ret.push_back("match10");
	}*/
	
/*	int time=0;
	for(CmdMap::const_iterator it= cmd_map_.begin();time < 5; it++){
	std::cout << time;
	time++;
	}*/
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
