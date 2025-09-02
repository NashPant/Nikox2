#ifndef TP_EXCEL_INTERFACE_FUNCTIONS_H
#define TP_EXCEL_INTERFACE_FUNCTIONS_H

#include "TPXLInterfaceUtils.h"
#include "TPMaths/TPMatrix.h"
//#include "TPTools/TPDate.h"
//#include "TPTools/TPAny.h"
#include "TPTools/TPAbort.h"
#include "TPExcelHelp.h"
#include <exception>
#include <windows.h>
#include <vector>
#include <string>
#include "XLCall.h"

using namespace std;
// Return time of XLL functions:
#define TP_RET_TO_EXCEL __declspec(dllexport) LPXLOPER WINAPI

// Use this macro to avoid evaluation of worksheet functions inside the function wizard.
#define TP_NOT_IN_EXCELS_FUNCTION_WIZARD if (TPExcelInterfaceFn::IAmCalledByFunctionWizard()) return TPExcelInterfaceFn::suppressedInFunctionWizard();

// Values to insert in blank cells of 2D Vectors:
const double BLANK_CELL_DOUBLE = -9999.0;
const long   BLANK_CELL_LONG   = -9999L;
const int    BLANK_CELL_INT	   = -9999;
const string BLANK_CELL_STRING = "";
//const TPDate BLANK_CELL_DATE   = TPDate::invalidDate();

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Description:
//		This class holds the type conversion functions to/from XLOPER.
//
// Six primitive types are supported:
//		double
//		long
//		int
//		string
//		TPDate
//		TPAny
//	
// The class definition:


class TPXLINTERFACE_IMPEXP TPExcelInterfaceFn
{
  public:
	// Constructors and Destructor:
	TPExcelInterfaceFn();
	TPExcelInterfaceFn( const TPExcelInterfaceFn& );

	virtual ~TPExcelInterfaceFn();

    // Wrapper for the call into Excel via Excel4().
    static int cdecl Excel(int xlfn, LPXLOPER pxResult, int count, ...);

    static long ExcelStatusBarMessage(const string &msg);
    static long MessageWindow(const char *msg,const char *caption);
    static long IAmCalledByFunctionWizard(void);
    static string callingCellId(void);
    static LPXLOPER suppressedInFunctionWizard(void);
    static bool abortHasBeenRequested(void);

    // Data type inspections:
	static bool isMissing( LPXLOPER );
	static bool isDouble ( LPXLOPER );
	static bool isString ( LPXLOPER );
	static bool isDate   ( LPXLOPER );
	static bool isNumber ( LPXLOPER );
	static bool isLong   ( LPXLOPER );
	static bool isBool   ( LPXLOPER );
	static bool isMulti  ( LPXLOPER );
	static bool isErr    ( LPXLOPER );
	static bool isErr    ( LPXLOPER, LPXLOPER  );
	static bool isErr    ( LPXLOPER, LPXLOPER, LPXLOPER  );
	static bool isErr    ( LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER  );
	static bool isErr    ( LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER  );
	static bool isErr    ( LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER  );
	static bool isErr    ( LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER  );
	static bool isErr    ( LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER  );
	static bool isErr    ( LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER  );
	static bool isErr    ( LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER  );
	static bool isErr    ( LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER  );
	static bool isErr    ( LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER  );
	static bool isErr    ( LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER  );
	static bool isErr    ( LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER  );
	static bool isErr    ( LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER  );
	static bool isErr    ( LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER  );
	static bool isErr    ( LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER  );
	static bool isErr    ( LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER  );
	static bool isErr    ( LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER  );
	static bool isErr    ( LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER, LPXLOPER  );

    //
    // Quick check that the xloper is not err, missing, or nil.
	static bool isUsable ( LPXLOPER );

    static bool isDoubleVector ( LPXLOPER );
	static bool isStringVector ( LPXLOPER );
	static bool isDateVector   ( LPXLOPER );
	static bool isNumberVector ( LPXLOPER );
	static bool isLongVector   ( LPXLOPER );
	static bool isBoolVector   ( LPXLOPER );

	// Data retrieval:
	// Single value
	static bool      getBool  ( LPXLOPER, const string & argIdentity );
	static double    getDouble(	LPXLOPER, const string &argIdentity );
	static double    getDouble(	LPXLOPER, const string &argIdentity, double defaultValue );
	static long      getLong  ( LPXLOPER, const string & argIdentity );
	static long      getLong  ( LPXLOPER, const string & argIdentity, long defaultValue );
	static int       getInt   (	LPXLOPER, const string & argIdentity );
	static string    getString( LPXLOPER, const string &argIdentity );
	static string    getString( LPXLOPER, const string &argIdentity, string defaultValue );
	//static TPDate    getDate  ( LPXLOPER, const string & argIdentity );
 //   static TPAny getValue (	LPXLOPER, const string & argIdentity );

	// One dimensional Vectors
	// Behaviour: Ignores any blank or error cell inputs (i.e. truncates the Vector)
	static vector<bool>      getBoolVector  ( LPXLOPER, bool bRemoveEmptyAtEnd , const string & argIdentity );
	static vector<double>    getDoubleVector( LPXLOPER, bool bRemoveEmptyAtEnd , const string & argIdentity );
	static vector<long>      getLongVector  ( LPXLOPER, bool bRemoveEmptyAtEnd , const string & argIdentity );
	static vector<int>       getIntVector   ( LPXLOPER, bool bRemoveEmptyAtEnd , const string & argIdentity );
	static vector<string>    getStringVector( LPXLOPER, bool bRemoveEmptyAtEnd , const string & argIdentity );
	//static vector<TPDate>    getDateVector  ( LPXLOPER, bool bRemoveEmptyAtEnd , const string & argIdentity );
	//static vector<TPAny> getValueVector ( LPXLOPER, const string & argIdentity );

	// Two dimensional Vectors
	// Behaviour: For any blank or error cell inputs it inserts the default blank value as #define-d above
    static TPMatrix<double>    getDoubleMatrix( LPXLOPER, const string & argIdentity );
	static TPMatrix<long>      getLongMatrix  (	LPXLOPER, const string & argIdentity );
	static TPMatrix<int>       getIntMatrix   (	LPXLOPER, const string & argIdentity );
	static TPMatrix<string>    getStringMatrix( LPXLOPER, const string & argIdentity );
	//static TPMatrix<TPDate>    getDateMatrix  (	LPXLOPER, const string & argIdentity );
 //   static TPMatrix<TPAny> getValueMatrix ( LPXLOPER, const string & argIdentity );

    // Read a 2-colum range of <Name,Value> pairs
    //static vector< pair< string, TPAny> > getNameValuePairVector( LPXLOPER inXLoper, const char* argIdentity );

    // size and 
    // Extracting columns
	static vector<double> getDoubleColumn ( LPXLOPER inXLoper, int col, const char* argIdentity );
	static vector<long>   getLongColumn   ( LPXLOPER inXLoper, int col, const char* argIdentity );
	static vector<int>    getIntColumn    ( LPXLOPER inXLoper, int col, const char* argIdentity );
	static vector<string> getStringColumn ( LPXLOPER inXLoper, int col, const char* argIdentity );
	//static vector<TPDate> getDateColumn   ( LPXLOPER inXLoper, int col, const char* argIdentity );

    //static TPAny  getValue(LPXLOPER inXLoper, int r, int c, const string & argIdentity );
    static int rows   (LPXLOPER inXLoper) {return inXLoper->val.array.rows;}
    static int columns(LPXLOPER inXLoper) { return inXLoper->val.array.columns;}

	// Functions to return to XL
	static LPXLOPER xlReturn(const double  arg);
	static LPXLOPER xlReturn(const long    arg);
	static LPXLOPER xlReturn(const string& arg);
	static LPXLOPER xlReturn(const char*   arg);
	static LPXLOPER xlReturn(const bool    arg);
	//static LPXLOPER xlReturn(const TPAny&      arg);
	static LPXLOPER xlReturn(const vector<double>& arg);
	static LPXLOPER xlReturn(const vector<string>& arg);
	//static LPXLOPER xlReturn(const vector<TPAny>&  arg);
	static LPXLOPER xlReturn(const TPMatrix<double>& arg);
	static LPXLOPER xlReturn(const TPMatrix<string>& arg);
	//static LPXLOPER xlReturn(const TPMatrix<TPAny>& arg);

	// Overloaded operators:
	const TPExcelInterfaceFn& operator= ( const TPExcelInterfaceFn& );

private:

    // Call back registration
    static const TPAbort ourExcelCallBackForAbortRequests;

	// Private Member Functions:
	static int  getRangeIndex   ( LPXLOPER, const int&, const int& );
	static bool removeReTPrences( unsigned, LPXLOPER );

    static void setEmpty(XLOPER& xlOper);

   	static void set(const int   arg, XLOPER& xlOper);
   	static void set(const long  arg, XLOPER& xlOper);
   	static void set(const double  arg, XLOPER& xlOper);
	static void set(const string& arg, XLOPER& xlOper);
	static void set(const bool    arg, XLOPER& xlOper);
	/*static void set(const TPDate& arg, XLOPER& xlOper);
	static void set(const TPAny&      arg, XLOPER& xlOper);*/
	static void set(const vector<double>& arg, XLOPER& xlOper);
	static void set(const vector<string>& arg, XLOPER& xlOper);
	//static void set(const vector<TPAny>&  arg, XLOPER& xlOper);
	static void set(const TPMatrix<double>& arg, XLOPER& xlOper);
	static void set(const TPMatrix<string>& arg, XLOPER& xlOper);
	//static void set(const TPMatrix<TPAny>& arg, XLOPER& xlOper);

	// Housekeeping:
	void copyFrom( const TPExcelInterfaceFn& );
};

// Define a Macro to catch exceptions and pass them to Excel
#define XLCATCHEXCEPTION                                            \
catch(TPException msg)                                              \
{                                                                   \
    const char *str = msg.getFullMessage().c_str();                  \
    return ( TPExcelInterfaceFn::xlReturn(str) );                               \
}                                                                   \
catch(std::exception &msg)                                          \
{                                                                   \
    const char *str = msg.what();                                   \
    return ( TPExcelInterfaceFn::xlReturn(str) );                               \
}                                                                   \
catch(...)                                                          \
{                                                                   \
    return ( TPExcelInterfaceFn::xlReturn("Unknown Exception") );                               \
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#endif 
