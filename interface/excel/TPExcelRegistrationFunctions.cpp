#pragma warning(disable : 4786)
#include "TPExcelRegistrationFunctions.h"
#include "framework.h"

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//define statics
TPExcelRegisterFunction::ourArrayType* TPExcelRegisterFunction::ourArray = 0;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
TPExcelRegisterFunction::TPExcelRegisterFunction(const std::string &               xllName,
                                                 const std::string &               functionName,
                                                 const std::string &               displayName,
                                                 const TPExcelArgumentArray & arguments,
                                                 const std::string &               helpString,
                                                 const std::string &               category,
                                                 int                          helpTopic,
                                                 const std::string &               shortcut,
                                                 const std::string &               functionType,
                                                 bool  thisFunctionIsVolatile )
                                                 : myXllName(xllName),
                                                 myFunctionName(functionName),
                                                 myDisplayName(displayName),	
                                                 myArguments(arguments),
                                                 myHelpString(helpString),
                                                 myCategory(category),
                                                 myHelpTopic(helpTopic),
                                                 myShortcut(shortcut),
                                                 myFunctionType(functionType),
                                                 myVolatileCalculationFlag(thisFunctionIsVolatile)
{
  init();
  populateArgumentNames();
  ourArray->insert(ourArrayType::value_type(xllName, this));
}

///////////////////////////////////////////////////////////////////////////////
void TPExcelRegisterFunction::populateArgumentNames(void)
{
  myArgumentNames = "";
  
  std::vector<TPExcelArgument>::const_iterator it = myArguments.arguments().begin();
  while(it != myArguments.arguments().end())
  {
    if( it->type() != TP_ExcelEmpty)
    {
      if(myArgumentNames.size() > 0)
      {
        myArgumentNames += ",";
      }
      
      myArgumentNames += it->name();
    }
    
    ++it;
  }
}

///////////////////////////////////////////////////////////////////////////////
void TPExcelRegisterFunction::registerAll(const std::string &xllToRegister)
{
  XLOPER xDLL;
  //TPExcelInterface::callExcel(xlGetName, &xDLL, 0);
  Excel(xlGetName, &xDLL, 0);
  XLOPER xlret;
  
  // iterate through the list of functions for this xll registering each function
  init();
  std::pair<ourArrayIteratorType, ourArrayIteratorType> startEnd = ourArray->equal_range(xllToRegister);
  ourArrayIteratorType it = startEnd.first;
  
  while(it != startEnd.second)
  {
    TPExcelRegisterFunction& fn = *((*it).second);
    
    // Create named XLOper objects to avoid temporaries
    XLOper xll_oper = XLOper(fn.functionName());
    XLOper type_oper = fn.isVolatile() ? XLOper(fn.arguments().types() + "!") : XLOper(fn.arguments().types());
    XLOper display_oper = XLOper(fn.displayName());
    XLOper arg_names_oper = XLOper(fn.argumentNames());
    XLOper fn_type_oper = XLOper(fn.functionType());
    XLOper category_oper = XLOper(fn.category());
    XLOper shortcut_oper = XLOper(fn.shortcut());
    XLOper empty_oper = XLOper("");
    XLOper help_oper = XLOper(fn.helpString());

    // Create XLOper objects for argument descriptions (up to 20, as in original code)
    // Pre-allocate array for argument descriptions
    std::vector<XLOper> arg_desc;
    for (int i = 0; i < 20; ++i) {
        if (i < fn.noOfArguments()) {
            arg_desc.emplace_back(fn.arguments()[i].description());
        }
        else {
            arg_desc.emplace_back(""); // Use empty string for unused slots
        }

    }
    //XLOper arg_desc[20];
    //for (int i = 0; i < fn.noOfArguments() && i < 20; ++i)
    //{
    //    arg_desc[i] = XLOper(fn.arguments()[i].description());
    //}

	//Excel(xlfRegister, &xlret, 10 + fn.noOfArguments(),
	//	(LPXLOPER)&xDLL,
	//	&XLOper(fn.functionName()),
	//	fn.isVolatile() ? &XLOper(fn.arguments().types() + "!") : &XLOper(fn.arguments().types()),
	//	&XLOper(fn.displayName()),
	//	&XLOper(fn.argumentNames()),
	//	&XLOper(fn.functionType()),
	//	&XLOper(fn.category()),
	//	&XLOper(fn.shortcut()),
	//	&XLOper(""),
	//	&XLOper(fn.helpString()),
	//	&XLOper(fn.arguments()[0].description()),
	//	&XLOper(fn.arguments()[1].description()),
	//	&XLOper(fn.arguments()[2].description()),
	//	&XLOper(fn.arguments()[3].description()),
	//	&XLOper(fn.arguments()[4].description()),
	//	&XLOper(fn.arguments()[5].description()),
	//	&XLOper(fn.arguments()[6].description()),
	//	&XLOper(fn.arguments()[7].description()),
	//	&XLOper(fn.arguments()[8].description()),
	//	&XLOper(fn.arguments()[9].description()),
	//	&XLOper(fn.arguments()[10].description()),
	//	&XLOper(fn.arguments()[11].description()),
	//	&XLOper(fn.arguments()[12].description()),
	//	&XLOper(fn.arguments()[13].description()),
	//	&XLOper(fn.arguments()[14].description()),
	//	&XLOper(fn.arguments()[15].description()),
	//	&XLOper(fn.arguments()[16].description()),
	//	&XLOper(fn.arguments()[17].description()),
	//	&XLOper(fn.arguments()[18].description()),
	//	&XLOper(fn.arguments()[19].description())
	//);
    // Call Excel with pointers to named XLOper objects
    Excel(xlfRegister, &xlret, 10 + fn.noOfArguments(),
        (LPXLOPER)&xDLL,
        &xll_oper,
        &type_oper,
        &display_oper,
        &arg_names_oper,
        &fn_type_oper,
        &category_oper,
        &shortcut_oper,
        &empty_oper,
        &help_oper,
        &arg_desc[0],
        &arg_desc[1],
        &arg_desc[2],
        &arg_desc[3],
        &arg_desc[4],
        &arg_desc[5],
        &arg_desc[6],
        &arg_desc[7],
        &arg_desc[8],
        &arg_desc[9],
        &arg_desc[10],
        &arg_desc[11],
        &arg_desc[12],
        &arg_desc[13],
        &arg_desc[14],
        &arg_desc[15],
        &arg_desc[16],
        &arg_desc[17],
        &arg_desc[18],
        &arg_desc[19]);

    //	    if(ret!=xlretSuccess || ( ret==xlretSuccess && xlret.xltype != xltypeNum ) )
    
    
    ++it;
  }
  
  // Free the XLL filename //
  //TPExcelInterface::callExcel(xlFree, 0, 1, (LPXLOPER) &xDLL);
  Excel(xlFree, 0, 1, (LPXLOPER)&xDLL);
}

///////////////////////////////////////////////////////////////////////////////
void TPExcelRegisterFunction::unregisterAll(const std::string &xllToUnregister)
{
  XLOPER xDLL,xRegId;
  Excel(xlGetName, &xDLL, 0);
//  TPExcelInterface::callExcel(xlGetName, &xDLL, 0);
  
  // may not work (according to excel documentation)
  // iterate through the list of functions for this xll unregistering each function
  init();
  std::pair<ourArrayIteratorType, ourArrayIteratorType> startEnd = ourArray->equal_range(xllToUnregister);
  ourArrayIteratorType it = startEnd.first;
  
  while(it != startEnd.second)
  {
    //TPExcelInterface::callExcel(xlfRegisterId,&xRegId,2,(LPXLOPER)&xDLL,&XLOper((*it).second->functionName().data()));
      XLOper functionNameOp((*it).second->functionName().data());
	 Excel(xlfRegisterId, &xRegId, 2, (LPXLOPER)&xDLL, &functionNameOp);
    //TPExcelInterface::callExcel(xlfUnregister, 0, 1, (LPXLOPER) &xRegId);
	 Excel(xlfUnregister, 0, 1, (LPXLOPER)&xRegId);
    
    ++it;
  }
  
  // remove the entries
  ourArray->erase(startEnd.first, startEnd.second);
  
  //TPExcelInterface::callExcel(xlFree, 0, 1,  (LPXLOPER) &xDLL);
  Excel(xlFree, 0, 1, (LPXLOPER)&xDLL);

}
