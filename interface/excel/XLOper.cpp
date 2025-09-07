#include "XLOper.h"
#include <list>
#include <vector>
#include <TPTools/TPGlobals.h>
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Constructors and Destructor:
XLOper::XLOper(const char *s) {
	val.str = 0;
	xltype = xltypeStr;
	unsigned long l = strlen(s) + 1;
	if (l>256) l = 256; // Truncate to maximum Pascal std::string length.
	val.str = new char[l + 1];
	strncpy(val.str + 1, s, l);
	val.str[0] = l - 1;
}

XLOper::XLOper(const std::string &str) {
	const char *s = str.c_str();
	val.str = 0;
	xltype = xltypeStr;
	unsigned long l = strlen(s) + 1;
	if (l>256) l = 256; // Truncate to maximum Pascal std::string length.
	val.str = new char[l + 1];
	strncpy(val.str + 1, s, l);
	val.str[0] = l - 1;
}

XLOper::~XLOper() {
	if ((xltype & xltypeStr) && val.str)
	{
		//xltype |= xlbitDLLFree;
		xLLFree(*this);
		val.str = nullptr;
	}
}

XLOper::XLOper(int i) {
	xltype = xltypeInt;
	val.w = i;
}

XLOper::XLOper(const double& x) {
	xltype = xltypeNum;
	val.num = x;
}

XLOper::XLOper(bool b) {
	xltype = xltypeBool;
	val.xbool = b;
}

///////////////////////////////////////////////////////////////////////////////
// SaTP recursive delete of an XLOper
void XLOper::xLLFree(XLOPER &p) {
	if (p.xltype & xlbitDLLFree) {
		switch (p.xltype ^ xlbitDLLFree) {
		case xltypeStr:
			TPDeleteArray(p.val.str);
			break;
		case xltypeMulti:
		{
			unsigned short i, j, k = p.val.array.columns;
			for (i = 0; i<p.val.array.rows; ++i) {
				for (j = 0; j<p.val.array.columns; ++j) {
					xLLFree(p.val.array.lparray[i*k + j]);
				}
			}
			TPDeleteArray(p.val.array.lparray);
		}
		break;
		default:
			break;
		}
		// The following statement is a saTPty precaution.
		p.xltype = xltypeNil;
	}
	return;
}

void XLOper::xLLFree(LPXLOPER p) {
	if (p) {
		xLLFree(*p);
		//TPDeleteOne(p);
	}
	return;
}
