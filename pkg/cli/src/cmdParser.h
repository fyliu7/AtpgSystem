/****************************************************************************
  FileName     [ cmdParser.h ]
  PackageName  [ cmd ]
  Synopsis     [ Define class CmdParser ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2007-2011 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/
#ifndef _CLI_CMD_PARSER_H_
#define _CLI_CMD_PARSER_H_

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "cmd/src/cmd.h"

#include "vt100.h"
#include "charDef.h"

using namespace std;

using namespace CmdNs; 

//----------------------------------------------------------------------
//    Forward Declaration
//----------------------------------------------------------------------

//----------------------------------------------------------------------
//    Base class : CmdParser
//----------------------------------------------------------------------

namespace Cli { 

class CmdParser
{
#define READ_BUF_SIZE    65536
#define TAB_POSITION     8
#define PG_OFFSET        10

public:
   CmdParser(CmdMgr* cmdMgr) : _readBufPtr(_readBuf), _readBufEnd(_readBuf),
                 _historyIdx(0), _tempCmdStored(false) { _cmdMgr = cmdMgr; }
   virtual ~CmdParser() {}

   bool openDofile(const char* dof) {
        _dofile.open(dof); return _dofile.is_open(); }

   void readCmd();

   CmdMgr *getCmdMgr() const; 

private:
   // Private member functions
   void resetBufAndPrintPrompt() {
        _readBufPtr = _readBufEnd = _readBuf;
        *_readBufPtr = 0;
        printPrompt();
   }
   void readCmdInt(istream&);
   void printPrompt() const { cout << "atpg> "; }
   bool moveBufPtr(char* const);
   bool deleteChar();
   void insertChar(char, int = 1);
   void deleteLine();
   void moveToHistory(int index);
   bool execLine(); 
   void addHistory();
   void retrieveHistory();
   bool autoComplete(); 
   #ifdef TA_KB_SETTING
   void taTestOnly() {}
   #endif

   // Data members
   CmdMgr*   _cmdMgr;                // command manager 

   ifstream  _dofile;
   char      _readBuf[READ_BUF_SIZE];// save the current line input
                                     // be consistent as shown on the screen
   char*     _readBufPtr;            // point to the cursor position
                                     // also be the insert and delete point
   char*     _readBufEnd;            // end of string position of _readBuf
                                     // make sure *_readBufEnd = 0
   vector<string>   _history;        // oldest:_history[0],latest:_hist.back()
   int              _historyIdx;     // (1) Position to insert history string
                                     //     i.e. _historyIdx = _history.size()
                                     // (2) When up/down/pgUp/pgDn is pressed,
                                     //     position to history to retrieve
   bool      _tempCmdStored;         // When up/pgUp is pressed, current line
                                     // will be stored in _history and
                                     // _tempCmdStored will be true.
                                     // Reset to false when new command added
};

inline CmdMgr *CmdParser::getCmdMgr() const { 
    return _cmdMgr; 
} 

}; //Cli

#endif // _CLI_CMD_PARSER_H_
