#include "TPExcelArgumentArray.h"


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//Member functions and constructors
TPExcelArgumentArray::TPExcelArgumentArray(TPExcelArgument arg1,
                                           TPExcelArgument arg2,
                                           TPExcelArgument arg3,
                                           TPExcelArgument arg4,
                                           TPExcelArgument arg5,
                                           TPExcelArgument arg6,
                                           TPExcelArgument arg7,
                                           TPExcelArgument arg8,
                                           TPExcelArgument arg9,
                                           TPExcelArgument arg10,
                                           TPExcelArgument arg11,
                                           TPExcelArgument arg12,
                                           TPExcelArgument arg13,
                                           TPExcelArgument arg14,
                                           TPExcelArgument arg15,
                                           TPExcelArgument arg16,
                                           TPExcelArgument arg17,
                                           TPExcelArgument arg18,
										   TPExcelArgument arg19,
										   TPExcelArgument arg20)
    : myArguments(20)
    // Number of arguments must not exceed 20.
    // This is because when we register a worksheet function through a call to
    //   Excel(int xlfn, LPXLOPER pxResult, int count, ...)
    // the variable  count  must not exceed 30. The total count, however, is given
    // by all of the registration details (DLL name, function name in DLL, argument type list,
    // function display name, argument name list, function type, function category, shortcut key,
    // help topic, and the function description) plus the argument list given above.
    // Since the registration details comprise a list of 10 elements, there is only space for 20
    // argument descriptions above.
{
    // for each non empty argument add it to the list
    myArguments.at(0)  = arg1;
    myArguments.at(1)  = arg2;
    myArguments.at(2)  = arg3;
    myArguments.at(3)  = arg4;
    myArguments.at(4)  = arg5;
    myArguments.at(5)  = arg6;
    myArguments.at(6)  = arg7;
    myArguments.at(7)  = arg8;
    myArguments.at(8)  = arg9;
    myArguments.at(9)  = arg10;
    myArguments.at(10) = arg11;
    myArguments.at(11) = arg12;
    myArguments.at(12) = arg13;
    myArguments.at(13) = arg14;
    myArguments.at(14) = arg15;
    myArguments.at(15) = arg16;
    myArguments.at(16) = arg17;
    myArguments.at(17) = arg18;
    myArguments.at(18) = arg19;
    myArguments.at(19) = arg20;

    // Populate the types string
    myTypes = "";

    std::vector<TPExcelArgument>::const_iterator it = myArguments.begin();
    while (it != myArguments.end())
    {
        if( it->type() != TP_ExcelEmpty )
        {
            // XLOper should be registered as "R" but since things appear to work, I leave this the way it is for now.
            // See "Microsoft Excel Developer's Kit", page 224, ISBN 1-57231-498-2
            myTypes += "P";
        }
        ++it;
    }

    // Add one for the return type
    myTypes += "P";
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
