/****************************************************************************
  FileName     [ cmdReader.cpp ]
  PackageName  [ cmd ]
  Synopsis     [ Define command line reader member functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2007-2011 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/
#include <cassert>
#include <cstring>
#include "cmdParser.h"

using namespace std;

using namespace CmdNs; 
using namespace Cli; 

//----------------------------------------------------------------------
//    Extrenal funcitons
//----------------------------------------------------------------------
void mybeep();
char mygetc(istream&);
ParseChar checkChar(char, istream&);


//----------------------------------------------------------------------
//    Member Function for class Parser
//----------------------------------------------------------------------
void
CmdParser::readCmd()
{
   if (_dofile.is_open()) {
      readCmdInt(_dofile);
      _dofile.close();
   }
   else
      readCmdInt(cin);
}

void
CmdParser::readCmdInt(istream& istr)
{
   resetBufAndPrintPrompt();

   while (1) {

      char ch = mygetc(istr);
      ParseChar pch = checkChar(ch, istr);
      //if (pch == INPUT_END_KEY) break;
      switch (pch) {
         case LINE_BEGIN_KEY :
         case HOME_KEY       : moveBufPtr(_readBuf); break;
         case LINE_END_KEY   :
         case END_KEY        : moveBufPtr(_readBufEnd); break;
         case BACK_SPACE_KEY : 
                               moveBufPtr(_readBufPtr - 1);
                               deleteChar();
                               break; 
         case DELETE_KEY     : deleteChar(); break;
         case NEWLINE_KEY    : cout << char(NEWLINE_KEY);
                               if(!execLine()) return; // execute command 
                               addHistory();
                               printPrompt(); break;
         case ARROW_UP_KEY   : moveToHistory(_historyIdx - 1); break;
         case ARROW_DOWN_KEY : moveToHistory(_historyIdx + 1); break;
         case ARROW_RIGHT_KEY: 
                               moveBufPtr(_readBufPtr + 1);break;                                     
         case ARROW_LEFT_KEY : 
                               moveBufPtr(_readBufPtr - 1);break;                                      
         case PG_UP_KEY      : moveToHistory(_historyIdx - PG_OFFSET); break;
         case PG_DOWN_KEY    : moveToHistory(_historyIdx + PG_OFFSET); break;
         case TAB_KEY        : if(!autoComplete()) mybeep(); break;
         case INSERT_KEY     : // not yet supported; fall through to UNDEFINE
         case UNDEFINED_KEY:   mybeep(); break;
         default:  // printable character
            insertChar(char(pch)); break;
      }
      #ifdef TA_KB_SETTING
      taTestOnly();
      #endif  
   }
}


// This function moves _readBufPtr to the "ptr" pointer
// It is used by left/right arrowkeys, home/end, etc.
//
// Suggested steps:
// 1. Make sure ptr is within [_readBuf, _readBufEnd].
//    If not, make a beep sound and return false. (DON'T MOVE)
// 2. Move the cursor to the left or right, depending on ptr
// 3. Update _readBufPtr accordingly. The content of the _readBuf[] will
//    not be changed
//
// [Note] This function can also be called by other member functions below
//        to move the _readBufPtr to proper position.
bool
CmdParser::moveBufPtr(char* const ptr)
{
  if(ptr > _readBufEnd || ptr < _readBuf)
  {
  	mybeep();
        return false;
  }
  else
  {
  	while(_readBufPtr > ptr)
        {
        	cout << VT100_CSRB;
                _readBufPtr--;
        }
        while(_readBufPtr < ptr)
        {
        	cout << VT100_CSRF;
                _readBufPtr++;
        }
  	return true;
  }
}


// [Notes]
// 1. Delete the char at _readBufPtr
// 2. mybeep() and return false if at _readBufEnd
// 3. Move the remaining string left for one character
// 4. The cursor should stay at the same position
// 5. Remember to update _readBufEnd accordingly.
// 6. Don't leave the tailing character.
// 7. Call "moveBufPtr(...)" if needed.
//
// For example,
//
// cmd> This is the command
//              ^                (^ is the cursor position)
//
// After calling deleteChar()---
//
// cmd> This is he command
//              ^
//
bool
CmdParser::deleteChar()
{
   if(_readBufPtr == _readBufEnd)
   {
     mybeep();
     return false;
   }
   char *oldptr;
   oldptr = _readBufPtr;
   for(int i = (int)(oldptr - _readBuf); i < (int)(_readBufEnd - _readBuf); i++)
   {
   	_readBuf[i] = _readBuf[i+1];
        cout << _readBuf[i];
        _readBufPtr++;
   } 
   cout << " ";
   _readBuf[(int)(_readBufEnd - _readBuf)]=0;
   moveBufPtr(oldptr);
   _readBufEnd--;
   return true;
}


// 1. Insert character 'ch' at _readBufPtr
// 2. Move the remaining string right for one character
// 3. The cursor should move right for one position afterwards
//
// For example,
//
// cmd> This is the command
//              ^                (^ is the cursor position)
//
// After calling insertChar('k') ---
//
// cmd> This is kthe command
//               ^
//
void
CmdParser::insertChar(char ch, int rep)
{
   _readBufEnd++;
   cout << ch;
   for(int i =(int)(_readBufEnd - _readBuf - 1); i >(int)(_readBufPtr - _readBuf); i--)
   	_readBuf[i] = _readBuf[i-1];
   _readBuf[(int)(_readBufPtr - _readBuf)]=ch;
   _readBufPtr++;
   cout << VT100_CSRS;
   for(int i =(int)(_readBufPtr - _readBuf); i < (int)(_readBufEnd - _readBuf); i++)
   	cout << _readBuf[i];
   cout << VT100_CSRR;
   
//   _readBufEnd++;
}


// 1. Delete the line that is currently shown on the screen
// 2. Reset _readBufPtr and _readBufEnd to _readBuf
// 3. Make sure *_readBufEnd = 0
//
// For example,
//
// cmd> This is the command
//              ^                (^ is the cursor position)
//
// After calling deleteLine() ---
//
// cmd>
//      ^
//
void
CmdParser::deleteLine()
{
   moveBufPtr(_readBufEnd);
   while(_readBuf != _readBufEnd)
   {
   	moveBufPtr(_readBufPtr -1);
   	deleteChar();
   }
}



// This functions moves _historyIdx to index and display _history[index]
// on the screen. The _historySize should NOT be changed.
//
// Need to consider:
// If moving up... (i.e. index < _historyIdx)
// 1. If already at top (i.e. _historyIdx == 0), beep and do nothing.
// 2. If at bottom, temporarily record _readBuf to history.
//    (Do not remove spaces, and set _tempCmdStored to "true")
// 3. If index < 0, let index = 0.
//
// If moving down... (i.e. index > _historyIdx)
// 1. If already at bottom, beep and do nothing
// 2. If index >= _history.size(), let index = _history.size() - 1.
//
// Assign _historyIdx to index at the end.
//
// [Note] index should not = _historyIdx
//
void
CmdParser::moveToHistory(int index)
{
   if(_historyIdx == _history.size())
   {
   	_tempCmdStored = true;
        string  string_current(_readBuf,(int)(_readBufEnd - _readBuf));
        _history.push_back(string_current);
   }
   if(index < _historyIdx)
   {
   	if(_historyIdx == 0)
        {

   	}
        else if(index < 0)
   		index = 0;
   	else
   	{
		deleteLine();
		cout << _history[index];
   		strcpy(_readBuf,_history[index].c_str());
  		_readBufEnd = _readBuf + _history[index].length();
   		_readBufPtr = _readBufEnd;
   		_historyIdx = index;
	}
   }
   else if(index > _historyIdx)
   {
   	if(index >= _history.size())
		index = _history.size() - 1;
	else if(_historyIdx == _history.size() - 1)
	{
		deleteLine();
		cout << _history.back();
	}
	else if(index >= _history.size())
		index = _history.size() - 1;
	else
        {
                deleteLine();
                cout << _history[index];
                strcpy(_readBuf,_history[index].c_str());
                _readBufEnd = _readBuf + _history[index].length();
                _readBufPtr = _readBufEnd;
                _historyIdx = index;
        }
   } 
   
   
 
}


// This function adds the string in _readBuf to the _history[_historySize]
//
// 1. Remove ' ' at the beginning and end of _readBuf
// 2. If not a null string, add string to _history.
//    Be sure you are adding to the right entry of _history.
// 3. If it is a null string, don't add anything to _history.
// 4. Make sure to clean up "temp recorded string" (added earlier by up/pgUp,
//    and reset _tempCmdStored to false
// 5. Reset _historyIdx to _history.size() // for future insertion
// 6. Reset _readBufPtr and _readBufEnd to _readBuf
// 7. Make sure *_readBufEnd = 0 ==> _readBuf becomes null string
//
void
CmdParser::addHistory()
{
   if(_readBuf == _readBufEnd)
   {

   }
   else
   {
    	string  string_current(_readBuf,(int)(_readBufEnd - _readBuf));
	if(_tempCmdStored == true)
		_history.pop_back();
        _history.push_back(string_current);
    	_historyIdx = _history.size();
	
        for(int i=0; i <= (int)(_readBufEnd - _readBuf); i++)
              _readBuf[i] = 0;
	_readBufEnd = _readBuf;
	_readBufPtr = _readBuf;
	*_readBufEnd = 0;
  	_tempCmdStored = false;
    }	
}


// 1. Replace current line with _history[_historyIdx] on the screen
// 2. Set _readBufPtr and _readBufEnd to end of line
//
// [Note] Do not change _historySize.
//
void
CmdParser::retrieveHistory()
{
   deleteLine();
   strcpy(_readBuf, _history[_historyIdx].c_str());
   cout << _readBuf;
   _readBufPtr = _readBufEnd = _readBuf + _history[_historyIdx].size();
}

bool
CmdParser::execLine() { 
   //TODO: Remove ' ' at the beginning and end of _readBuf

   if(_readBuf == _readBufEnd) 
        return CmdMgr::NOP; 

   string cmdStr(_readBuf, _readBufEnd - _readBuf); 

   CmdMgr::Result res = _cmdMgr->exec(cmdStr); 
   if(res==CmdMgr::EXIT) return false; 
   else if(res==CmdMgr::NOT_EXIST) 
      cout << "**Error CmdParser::execLine(): command not found\n"; 

   return true; 
}

bool 
CmdParser::autoComplete() {
    //TODO: implement the auto-completion functionality. 

    return false; 
}
