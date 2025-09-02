#include "excel/TPExcelRegistrationFunctions.h"
#include "TPXLNikoUtils.h"

/////////////////////////////////////////////////////////////////////////
///// XLL's entry point:
__declspec(dllexport) int WINAPI xlAutoOpen(void)
{
    TPExcelRegisterFunction::registerAll(TPXLNikoTag);
    return 1;
}

///////////////////////////////////////////////////////////////////////////////
__declspec(dllexport)  int  xlAutoClose();
int xlAutoClose()
{
    TPExcelRegisterFunction::unregisterAll(TPXLNikoTag);
    return 1;
}


/////////////////////////////////////////////////////////////////////////
/////
// No need to change anything here.
__declspec(dllexport) void WINAPI xlAutoFree(LPXLOPER lpx)
{
    TPExcelRegisterFunction::deleteXloper(lpx);
    return;
}