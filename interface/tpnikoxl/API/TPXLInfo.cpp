#pragma warning(disable : 4786)
#include "../TPXLNikoUtils.h"
#include "excel/TPExcelRegistrationFunctions.h"
#include "excel/TPExcelInterface.h"
//#include "TPXLInterface_64/TPExcelHelp.h"
#include "excel/NewInterface/moduleInfo.h"


TPExcelRegisterFunction TPModulePath_
(
	TPXLNikoTag,
	"TPXLModulePath",
	"tpModulePath",
	TPExcelArgumentArray(
			),

	"Displays Information about the Nikopolis.xll module",
	"NiCoPolis : Api",
	0,
	"",
	"1",

	true               //  Volatile function
);

///////////////////////////////////////////////////////////////////////////////
extern "C" { __declspec(dllexport) LPXLOPER WINAPI
TPXLModulePath(
)
{
	try
	{
		auto res = TP::Excel::installationPath();
		return TPExcelInterface::returnToExcel(res);
	}
	catch (TPException& msg)
	{
		return TPExcelInterface::returnToExcel(msg.Why());
	}
	catch (...)
	{
		return TPExcelInterface::returnToExcel("unknown error");
	}
}
}


TPExcelRegisterFunction TPModuleName_
(
	TPXLNikoTag,
	"TPXLModuleName",
	"tpModuleName",
	TPExcelArgumentArray(
	),

	"Displays Information about the Nikopolis.xll module",
	"NiCoPolis : Api",
	0,
	"",
	"1",

	true               //  Volatile function
);

///////////////////////////////////////////////////////////////////////////////
extern "C" { __declspec(dllexport) LPXLOPER WINAPI
TPXLModuleName(
)
{
	try
	{
		auto res = TP::Excel::moduleName();
		return TPExcelInterface::returnToExcel(res);
	}
	catch (TPException& msg)
	{
		return TPExcelInterface::returnToExcel(msg.Why());
	}
	catch (...)
	{
		return TPExcelInterface::returnToExcel("unknown error");
	}
}
}


TPExcelRegisterFunction TPDisplayTest_
(
	TPXLNikoTag,
	"TPDXLisplayTest",
	"tpdisplayTest",
	TPExcelArgumentArray(
		TPExcelArgument(TP_ExcelString, "string", "What to display")
	),

	"Displays Information about the Nikopolis.xll module",
	"NiCoPolis : Api",
	0,
	"",
	"1",

	true               //  Volatile function
);

///////////////////////////////////////////////////////////////////////////////
extern "C" { __declspec(dllexport) LPXLOPER WINAPI
TPDXLisplayTest(LPXLOPER XLTesx
)
{
	try
	{
		std::string tt = TPExcelInterface::toString(XLTesx, "TPDXLisplayTest(XLConfigpath)");
		TP::Excel::DisplayError(tt.c_str());
		return TPExcelInterface::returnToExcel(0.);
	}
	catch (TPException& msg)
	{
		return TPExcelInterface::returnToExcel(msg.Why());
	}
	catch (...)
	{
		return TPExcelInterface::returnToExcel("unknown error");
	}
}
}