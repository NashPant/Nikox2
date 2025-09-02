#ifndef _TPExcelInterface
#define _TPExcelInterface

#pragma warning(disable : 4786) 


#include <windows.h>
#include "XLCall.h"


#include "Tpmaths/TPMatrix.h"
//#include "TPTools/TPSmartPointer.h"
#include "TPXLInterfaceUtils.h"

#include <string>
#include <vector>
#include <optional>

#define EXCEL_OFFSET 29221	//corresponds to 01/01/1980 in Excel
							//Summit 's sDATE_OFFSET  is 604000, i.e.
							//all dates are >604000. Also, Summit's
							//sBASE_YEAR  is 1980    

//
//
// Wrapper class around Excel.
// Simple and basic really, 
// there is a lot to be done
// to improve it.
//
//



/*class TPDate;
class TPAny*/;

class  TPXLINTERFACE_IMPEXP TPExcelInterface
{
protected:
 // static TPSmartPointer<TPDate> myReferenceDate;

public:

  //static const TPDate& getRefDate() ;
  //static void setRefDate(const TPDate& date) ;


	static int callExcel(int xlfunction,
					   LPXLOPER	xlerror,
					   int count,
					   ...);
	//Excel Inputs
	//Grab inputs as LPXLOPERs to avoid all
	//the nastiness of coercing them into
	//proper human types.

	static int DisplayDialog(
							HINSTANCE hInstance, 
							LPCTSTR lpTemplate, 
							DLGPROC lpDialogProc);

	static bool IsFunctionWizardActive();

	static int xlAlert(std::string pMsg);

	//
	//first check out type
	//

	static bool	istypeMulti	(LPXLOPER);
	static bool isInvalid	(LPXLOPER);
	static bool isMissing	(LPXLOPER);
	static bool isBool		(LPXLOPER);
	static bool isLong		(LPXLOPER);
	static bool isDouble	(LPXLOPER);
	static bool isFloat     (LPXLOPER);
	static bool isString	(LPXLOPER);
	//static bool isDate		(LPXLOPER);

	//
	//then hand it back over 
	//

	//
	//get single cell items
	//

	static	bool	toBool		(LPXLOPER, const std::string &argument, const bool default_value=false);
	static	int		toInt		(LPXLOPER, const std::string &argument, const int default_value=0);
	static	long	toLong		(LPXLOPER, const std::string &argument, const long& default_value = 0);
	static	double	toDouble	(LPXLOPER, const std::string &argument, const double default_value=0.0);
	static  float   toFloat		(LPXLOPER, const std::string &argument);
	static	std::string	toString	(LPXLOPER, const std::string &argument, const std::string& default_value=std::string(""));
	static std::optional<std::string> toOptionalString(LPXLOPER, const std::string &argument);
	//static	TPDate	toDate		(LPXLOPER, const std::string &argument);
	//static	TPAny	toAny		(LPXLOPER, const std::string& argument);

	//get arrays

	static std::vector<double>	toDoubleArray					(LPXLOPER, const std::string &argument);
	static std::vector<double>	toDoubleArrayNoErrs				(LPXLOPER, const std::string &argument);
	static std::vector<double>	toDoubleArrayNoErrs				(LPXLOPER, const std::string &argument, const double repl=0.0);
	
	static std::vector<long>		toLongArray						(LPXLOPER, const std::string &argument);
	
	static std::vector<std::string>	toStringArray					(LPXLOPER, const std::string &argument);
	static std::vector<std::string>	toStringArrayNoErrs				(LPXLOPER, const std::string &argument);
	static std::vector<std::string>	toStringArrayNoErrs				(LPXLOPER, const std::string &argument, const std::string& repl=std::string("X"));
	
	/*static std::vector<TPDate>	toDateArray						(LPXLOPER, const std::string &argument);
	static std::vector<TPDate>	toDateArrayNoErrs				(LPXLOPER, const std::string &argument);
	static std::vector<TPDate>	toDateArrayNoErrs				(LPXLOPER, const std::string &argument, const TPDate& repl);
	
	static std::vector<TPAny>	toAnyArray						(LPXLOPER, const std::string &argument);
	static std::vector<TPAny>	toAnyArrayNoErrs				(LPXLOPER, const std::string &argument);
	static std::vector<TPAny>	toAnyArrayNoErrs				(LPXLOPER, const std::string &argument, const TPAny& repl);*/

	// Simple Conversion from std::vector<double> to std::vector<TPAny>
	//static std::vector<TPAny>    fromDoubletoAnyArray			(const std::vector<double>& xx);
	//get matrices

	static TPMatrix<double>	toDoubleMatrix					(LPXLOPER, const std::string &argument);
	static TPMatrix<double>	toDoubleMatrixNoErrs			(LPXLOPER, const std::string &argument);
	static TPMatrix<double>	toDoubleMatrixNoErrs			(LPXLOPER, const std::string &argument, const double repl=0.0);
	static std::vector<std::pair<double, double>>	toPairArray(LPXLOPER, const std::string &argument);

	static TPMatrix<std::string>	toStringMatrix					(LPXLOPER, const std::string &argument);
	static TPMatrix<std::string>	toStringMatrixNoErrs			(LPXLOPER, const std::string &argument);
	static TPMatrix<std::string>	toStringMatrixNoErrs			(LPXLOPER, const std::string &argument, const std::string& repl=std::string("X"));

	//static TPMatrix<TPAny>	toAnyMatrix						(LPXLOPER, const std::string &argument);
	//static TPMatrix<TPAny>	toAnyMatrixNoErrs				(LPXLOPER, const std::string &argument);
	//static TPMatrix<TPAny>	toAnyMatrixNoErrs				(LPXLOPER, const std::string &argument, const TPAny& repl);

	//Excel output

	//
	//single cell output
	//

	static	LPXLOPER	returnToExcel(const double							rhs);
	static	LPXLOPER	returnToExcel(const long							rhs);
	static	LPXLOPER	returnToExcel(const std::string&							rhs);
	static	LPXLOPER	returnToExcel(const char*rhs,			  size_t length);
	//static	LPXLOPER	returnToExcel(const TPAny&							rhs);

	//Array cell output

	static	LPXLOPER	returnToExcel(const std::vector<double>&					rhs);
	static	LPXLOPER	returnToExcel(const std::vector<std::string>&					rhs);
	//static	LPXLOPER	returnToExcel(const std::vector<TPAny>&					rhs);

	//Matrix Cell output


	static	LPXLOPER	returnToExcel(const TPMatrix<double>&				rhs);
	//static	LPXLOPER	returnToExcel(const std::vector<std::pair<double, double>>&	 rhs);
	static	LPXLOPER	returnToExcel(const TPMatrix<std::string>&				rhs);
	//static	LPXLOPER	returnToExcel(const TPMatrix<TPAny>&				rhs);
								
		

};


// A macro to prevent the wizard from calculating the function each time you enter a new value
const int MEP_BUFSIZ = 30;

inline bool XLWrapIsFunctionWizardActive()
{
	HWND	hwndXL;

	// get the handle to XL

	hwndXL = GetActiveWindow();

	char	buf[MEP_BUFSIZ];
	int		ret;

	ret = GetClassName(hwndXL, buf, MEP_BUFSIZ);
	if (ret != 0)
	{
		if (_strnicmp(buf, "bosa_sdm_xl", 11) == 0)
		{
			return true;
		}
	}

	return false;
}

#define TPXL_CHECK_WIZARD if(XLWrapIsFunctionWizardActive()) return TPExcelInterface::returnToExcel("Wizard failed to re-calculate");





#endif
