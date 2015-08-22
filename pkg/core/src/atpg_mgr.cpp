/*
 * =====================================================================================
 *
 *       Filename:  atpg_mgr.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  08/21/2015 02:39:07 PM
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  xenia-cjen (xc), jonah0604@gmail.com
 *        Company:  LaDS(I), GIEE, NTU
 *
 * =====================================================================================
 */

#include "atpg_mgr.h" 

using namespace std; 

using namespace CoreNs; 

bool AtpgMgr::Run() { 
    for(size_t n=0; n<f_mgr->getFaultNum(); n++) { 
        Atpg *atpg = new Atpg(cir, f_mgr->getFault(n)); 
        Pattern p; 
        if(!atpg->Tpg(p)) { 
            //TODO 
            cout << "*** BACKTRACK NEEDED!! \n";  
        }
        atpgs_.push_back(atpg); 
    }
} 
