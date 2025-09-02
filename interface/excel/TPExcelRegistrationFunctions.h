#ifndef TP_EXCEL_REGISTRATION_FUNCTIONS_H
#define TP_EXCEL_REGISTRATION_FUNCTIONS_H

// These classes are used to register functions with Excel
// and deal with the memory management.
//
#include "TPXLInterfaceUtils.h"
#include <string>

#include <map>
#include <vector>
//#include <TPTools/TPSmartPointer.h>
#include "TPExcelArgument.h"
#include "TPExcelArgumentArray.h"
#include "TPExcelInterface.h"
#include "XLOper.h"

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class TPXLINTERFACE_IMPEXP TPExcelRegisterFunction
{
public:
  TPExcelRegisterFunction(const std::string &               xllName,
    const std::string &               functionName,
    const std::string &               displayName,
    const TPExcelArgumentArray & arguments,
    const std::string &               helpString,
    const std::string &               category,
    int                          helpTopic = 0,
    const std::string &               shortcut = "",
    // "0" hides the function to be registered, "1" indicates it should be a worksheet function,
    // and "2" registers the function as a macro function, see "Microsoft Excel Developer's Kit",
    // page 220, ISBN 1-57231-498-2.
    const std::string &       functionType = "1",
    //
    // Request recalculation of this function every time anything is changed in the spreadsheet,
    // even if there is no need according to the calculation chain. This is useful for functions
    // like TPDisplayModelCache etc. that don't actually do any calculations but need to be always
    // up to date (because other functions may use them to look things up in the current state of
    // the cache).
    //
    bool thisFunctionIsVolatile = false
    );
  
  void xlregister();
  void unregister();
  static void registerAll(const std::string &xllToRegister);
  static void unregisterAll(const std::string &xllToUnRegister);
  static void registerMenu();
  static void deleteXloper(LPXLOPER lpx){ XLOper::xLLFree(lpx); };
  
  size_t noArguments() { return myArguments.size(); }
  
  const std::string& xllName() const	    { return myXllName; }	
  const std::string& functionName() const	{ return myFunctionName; }	
  const std::string& displayName() const  	{ return myDisplayName; }		
  const TPExcelArgumentArray& arguments()	const { return myArguments; }	
  size_t   noOfArguments(void) const     { return myArguments.size();}
  const std::string& helpString() const    { return myHelpString; }	
  const std::string& category() const      { return myCategory; }
  int   helpTopic() const		        { return myHelpTopic; }
  const std::string& shortcut() const		{ return myShortcut; }
  const std::string& functionType() const	{ return myFunctionType; }
  const std::string& argumentNames() const { return myArgumentNames; }
  bool isVolatile() const { return myVolatileCalculationFlag; }
  // Creates strings used in TPExcelInterfaceFn::getDouble() and friends. See its inline definition below.
  std::string argIdentity( unsigned long i );
  
  bool operator==(const TPExcelRegisterFunction& rhs) const
  {
    return (myFunctionName == rhs.myFunctionName);
  }
  
private:
  static void init(void);
  void populateArgumentNames(void);
  
  std::string myXllName;                   // xll name
  std::string myFunctionName;	            // function name in dll
  std::string myDisplayName;	            // display name in Excel
  TPExcelArgumentArray myArguments;	// XLArgArray constructed from list of XLArgument instances
  std::string myHelpString;	            // help information to display in function wizard
  std::string myCategory;	                // category in functions window
  int    myHelpTopic;	                // full path to .hlp file
  std::string myShortcut;	                // single char shortcut to assign to ctrl-shift-key
  std::string myFunctionType;	            // 0=hidden, 1=function(default), 2=command (macro sheets only)
  std::string myArgumentNames;
  bool   myVolatileCalculationFlag;
  
  typedef std::multimap<std::string, TPExcelRegisterFunction *> ourArrayType;
  typedef std::multimap<std::string, TPExcelRegisterFunction *>::iterator ourArrayIteratorType;
  static ourArrayType* ourArray;
};
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////


// inline member functions
inline
void TPExcelRegisterFunction::init(void)
{
  // bool to force to initialise the static pointer ourArray at the right time
  // even with a static lib.
  static bool isInitialised = false;

  if(!isInitialised)
  {
    ourArray = new ourArrayType();
    isInitialised = true;
  }
}

/*
inline
std::string TPExcelRegisterFunction::argIdentity( unsigned long i ){
char numberString[64];
sprintf(numberString,"%lu",i);
return displayName()+"( argument #" + numberString + " : " + arguments().arguments().at(i).name() +" )";
}
*/
#endif   // TP_EXCEL_REGISTRATION_FUNCTIONS_H