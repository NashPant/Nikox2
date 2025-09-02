#ifndef TP_EXCEL_ARGUMENT_ARRAY_H
#define TP_EXCEL_ARGUMENT_ARRAY_H

#include "TPXLInterfaceUtils.h"
#include "TPExcelArgument.h"
#include <vector>

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// TPExcelArgumentArray
class TPXLINTERFACE_IMPEXP TPExcelArgumentArray
{
public:
    TPExcelArgumentArray(TPExcelArgument arg1 = TPExcelArgument(TP_ExcelEmpty),
                         TPExcelArgument arg2 = TPExcelArgument(TP_ExcelEmpty),
                         TPExcelArgument arg3 = TPExcelArgument(TP_ExcelEmpty),
                         TPExcelArgument arg4 = TPExcelArgument(TP_ExcelEmpty),
                         TPExcelArgument arg5 = TPExcelArgument(TP_ExcelEmpty),
                         TPExcelArgument arg6 = TPExcelArgument(TP_ExcelEmpty),
                         TPExcelArgument arg7 = TPExcelArgument(TP_ExcelEmpty),
                         TPExcelArgument arg8 = TPExcelArgument(TP_ExcelEmpty),
                         TPExcelArgument arg9 = TPExcelArgument(TP_ExcelEmpty),
                         TPExcelArgument arg10= TPExcelArgument(TP_ExcelEmpty),
                         TPExcelArgument arg11= TPExcelArgument(TP_ExcelEmpty),
                         TPExcelArgument arg12= TPExcelArgument(TP_ExcelEmpty),
                         TPExcelArgument arg13= TPExcelArgument(TP_ExcelEmpty),
                         TPExcelArgument arg14= TPExcelArgument(TP_ExcelEmpty),
                         TPExcelArgument arg15= TPExcelArgument(TP_ExcelEmpty),
                         TPExcelArgument arg16= TPExcelArgument(TP_ExcelEmpty),
                         TPExcelArgument arg17= TPExcelArgument(TP_ExcelEmpty),
                         TPExcelArgument arg18= TPExcelArgument(TP_ExcelEmpty),
                         TPExcelArgument arg19= TPExcelArgument(TP_ExcelEmpty),
                         TPExcelArgument arg20= TPExcelArgument(TP_ExcelEmpty));

    const std::vector<TPExcelArgument>& arguments(void) const {return myArguments;}
    const TPExcelArgument& operator[](size_t idx) const  {return myArguments[idx];}
    size_t   size(void) const {return myArguments.size();}
    const std::string& types(void) const { return myTypes; }

private:
    std::vector<TPExcelArgument> myArguments;    
    std::string                  myTypes;
};
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#endif