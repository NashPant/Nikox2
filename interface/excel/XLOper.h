#pragma once

#include <windows.h>
#include "XLCall.h"
#include "TPXLInterfaceUtils.h"
#include <string>
//
// A class that is identical to XLOPER.
//

class TPXLINTERFACE_IMPEXP XLOper : public xloper {
public:

	enum XLType
	{
		XLTypeNum = xltypeNum,
		XLTypeStr = xltypeStr,
		XLTypeBool = xltypeBool,
		XLTypeRef = xltypeRef,
		XLTypeErr = xltypeErr,
		XLTypeFlow = xltypeFlow,
		XLTypeMulti = xltypeMulti,
		XLTypeMissing = xltypeMissing,
		XLTypeNil = xltypeNil,
		XLTypeSRef = xltypeSRef,
		XLTypeInt = xltypeInt,
		//
		XLTypeNumDLLFree = xltypeNum | xlbitDLLFree,
		XLTypeStrDLLFree = xltypeStr | xlbitDLLFree,
		XLTypeBoolDLLFree = xltypeBool | xlbitDLLFree,
		XLTypeRefDLLFree = xltypeRef | xlbitDLLFree,
		XLTypeErrDLLFree = xltypeErr | xlbitDLLFree,
		XLTypeFlowDLLFree = xltypeFlow | xlbitDLLFree,
		XLTypeMultiDLLFree = xltypeMulti | xlbitDLLFree,
		XLTypeMissingDLLFree = xltypeMissing | xlbitDLLFree,
		XLTypeNilDLLFree = xltypeNil | xlbitDLLFree,
		XLTypeSRefDLLFree = xltypeSRef | xlbitDLLFree,
		XLTypeIntDLLFree = xltypeInt | xlbitDLLFree
	};

	XLOper(const char *);
	XLOper(const std::string &s);
	XLOper(const double&);
	XLOper(bool);
	XLOper(int);
	~XLOper();
	static void xLLFree(XLOPER &p);
	static void xLLFree(LPXLOPER p);
};
