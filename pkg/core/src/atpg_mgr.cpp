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
    if(!pat_mgr) pat_mgr = new PatternMgr(cir); 

    for(size_t n=0; n<f_mgr->getFaultNum(); n++) { 
        Atpg *atpg = new Atpg(cir, f_mgr->getFault(n)); 
        if(atpg->Tpg()) { 
            Pattern *p = new Pattern; 
            atpg->GetPattern(*p); 

            pat_mgr->pats.push_back(p); 
            f_mgr->getFault(n)->status = Fault::DT; 
        }
        else { 
            //TODO 
            cout << "*** BACKTRACK NEEDED!! \n";  
        }
        atpgs_.push_back(atpg); 
    }
} 
