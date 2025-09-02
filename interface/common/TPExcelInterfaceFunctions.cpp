#include "TPExcelInterfaceFunctions.h"
#include <TPTools/TPException.h>
#include <list>
#include <vector>

#if defined(WIN32) || defined(_WINDOWS)

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//
// Static callback registration.
//
const TPAbort TPExcelInterfaceFn::ourExcelCallBackForAbortRequests(TPExcelInterfaceFn::abortHasBeenRequested);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Constructors and Destructor:
TPExcelInterfaceFn::TPExcelInterfaceFn( )
{
}

///////////////////////////////////////////////////////////////////////////////
TPExcelInterfaceFn::TPExcelInterfaceFn( const TPExcelInterfaceFn& sourceObj )
{
	*this = sourceObj;
}

///////////////////////////////////////////////////////////////////////////////
TPExcelInterfaceFn::~TPExcelInterfaceFn()
{
}

///////////////////////////////////////////////////////////////////////////////
// Public Member functions:
// Data type inspections:
bool TPExcelInterfaceFn::isBool( LPXLOPER inXLoper )
{
	return( (inXLoper->xltype & xltypeType) == xltypeBool );
}

///////////////////////////////////////////////////////////////////////////////
bool TPExcelInterfaceFn::isDouble( LPXLOPER inXLoper )
{
	return( (inXLoper->xltype & xltypeType) == xltypeNum || (inXLoper->xltype & xltypeType) == xltypeInt );
}

///////////////////////////////////////////////////////////////////////////////
bool TPExcelInterfaceFn::isNumber( LPXLOPER inXLoper )
{
	return( (inXLoper->xltype & xltypeType) == xltypeNum || (inXLoper->xltype & xltypeType) == xltypeInt );
}

///////////////////////////////////////////////////////////////////////////////
bool TPExcelInterfaceFn::isLong( LPXLOPER inXLoper )
{
	return( (inXLoper->xltype & xltypeType) == xltypeNum || (inXLoper->xltype & xltypeType) == xltypeInt );
}

///////////////////////////////////////////////////////////////////////////////
bool TPExcelInterfaceFn::isString( LPXLOPER inXLoper )
{
	return( (inXLoper->xltype & xltypeType) == xltypeStr );
}

///////////////////////////////////////////////////////////////////////////////
bool TPExcelInterfaceFn::isDate( LPXLOPER inXLoper )
{
    return ( isNumber( inXLoper ) && TPDate( getLong( inXLoper, "isDate(): " ) ).isValid() );
}

///////////////////////////////////////////////////////////////////////////////
bool TPExcelInterfaceFn::isMissing( LPXLOPER inXLoper )
{
	return( ( (inXLoper->xltype & xltypeType) == xltypeMissing ) || (inXLoper->xltype  & xltypeType) == xltypeNil );
}

///////////////////////////////////////////////////////////////////////////////
bool TPExcelInterfaceFn::isMulti( LPXLOPER inXLoper )
{
    return (inXLoper->xltype  & xltypeType) == xltypeMulti;
}

///////////////////////////////////////////////////////////////////////////////
bool TPExcelInterfaceFn::isErr( LPXLOPER inXLoper )
{
    return (inXLoper->xltype  & xltypeType) == xltypeErr;
}

bool TPExcelInterfaceFn::isErr( LPXLOPER x1, LPXLOPER x2 ){
    return isErr(x1)||isErr(x2);
}

bool TPExcelInterfaceFn::isErr( LPXLOPER x1, LPXLOPER x2, LPXLOPER x3 ){
    return isErr(x1)||isErr(x2)||isErr(x3);
}

bool TPExcelInterfaceFn::isErr( LPXLOPER x1, LPXLOPER x2, LPXLOPER x3, LPXLOPER x4 ){
    return isErr(x1)||isErr(x2)||isErr(x3)||isErr(x4);
}

bool TPExcelInterfaceFn::isErr( LPXLOPER x1, LPXLOPER x2, LPXLOPER x3, LPXLOPER x4, LPXLOPER x5 ){
    return isErr(x1)||isErr(x2)||isErr(x3)||isErr(x4)||isErr(x5);
}

bool TPExcelInterfaceFn::isErr( LPXLOPER x1, LPXLOPER x2, LPXLOPER x3, LPXLOPER x4, LPXLOPER x5, LPXLOPER x6 ){
    return isErr(x1)||isErr(x2)||isErr(x3)||isErr(x4)||isErr(x5)||isErr(x6);
}

bool TPExcelInterfaceFn::isErr( LPXLOPER x1, LPXLOPER x2, LPXLOPER x3, LPXLOPER x4, LPXLOPER x5, LPXLOPER x6, LPXLOPER x7 ){
    return isErr(x1)||isErr(x2)||isErr(x3)||isErr(x4)||isErr(x5)||isErr(x6)||isErr(x7);
}

bool TPExcelInterfaceFn::isErr( LPXLOPER x1, LPXLOPER x2, LPXLOPER x3, LPXLOPER x4, LPXLOPER x5, LPXLOPER x6, LPXLOPER x7, LPXLOPER x8 ){
    return isErr(x1)||isErr(x2)||isErr(x3)||isErr(x4)||isErr(x5)||isErr(x6)||isErr(x7)||isErr(x8);
}

bool TPExcelInterfaceFn::isErr( LPXLOPER x1, LPXLOPER x2, LPXLOPER x3, LPXLOPER x4, LPXLOPER x5, LPXLOPER x6, LPXLOPER x7, LPXLOPER x8, LPXLOPER x9 ){
    return isErr(x1)||isErr(x2)||isErr(x3)||isErr(x4)||isErr(x5)||isErr(x6)||isErr(x7)||isErr(x8)||isErr(x9);
}

bool TPExcelInterfaceFn::isErr( LPXLOPER x1, LPXLOPER x2, LPXLOPER x3, LPXLOPER x4, LPXLOPER x5, LPXLOPER x6, LPXLOPER x7, LPXLOPER x8, LPXLOPER x9, LPXLOPER x10 ){
    return isErr(x1)||isErr(x2)||isErr(x3)||isErr(x4)||isErr(x5)||isErr(x6)||isErr(x7)||isErr(x8)||isErr(x9)||isErr(x10);
}

bool TPExcelInterfaceFn::isErr( LPXLOPER x1, LPXLOPER x2, LPXLOPER x3, LPXLOPER x4, LPXLOPER x5, LPXLOPER x6, LPXLOPER x7, LPXLOPER x8, LPXLOPER x9, LPXLOPER x10, LPXLOPER x11 ){
    return isErr(x1)||isErr(x2)||isErr(x3)||isErr(x4)||isErr(x5)||isErr(x6)||isErr(x7)||isErr(x8)||isErr(x9)||isErr(x10)||isErr(x11);
}

bool TPExcelInterfaceFn::isErr( LPXLOPER x1, LPXLOPER x2, LPXLOPER x3, LPXLOPER x4, LPXLOPER x5, LPXLOPER x6, LPXLOPER x7, LPXLOPER x8, LPXLOPER x9, LPXLOPER x10, LPXLOPER x11, LPXLOPER x12 ){
    return isErr(x1)||isErr(x2)||isErr(x3)||isErr(x4)||isErr(x5)||isErr(x6)||isErr(x7)||isErr(x8)||isErr(x9)||isErr(x10)||isErr(x11)||isErr(x12);
}

bool TPExcelInterfaceFn::isErr( LPXLOPER x1, LPXLOPER x2, LPXLOPER x3, LPXLOPER x4, LPXLOPER x5, LPXLOPER x6, LPXLOPER x7, LPXLOPER x8, LPXLOPER x9, LPXLOPER x10, LPXLOPER x11, LPXLOPER x12, LPXLOPER x13 ){
    return isErr(x1)||isErr(x2)||isErr(x3)||isErr(x4)||isErr(x5)||isErr(x6)||isErr(x7)||isErr(x8)||isErr(x9)||isErr(x10)||isErr(x11)||isErr(x12)||isErr(x13);
}

bool TPExcelInterfaceFn::isErr( LPXLOPER x1, LPXLOPER x2, LPXLOPER x3, LPXLOPER x4, LPXLOPER x5, LPXLOPER x6, LPXLOPER x7, LPXLOPER x8, LPXLOPER x9, LPXLOPER x10, LPXLOPER x11, LPXLOPER x12, LPXLOPER x13, LPXLOPER x14 ){
    return isErr(x1)||isErr(x2)||isErr(x3)||isErr(x4)||isErr(x5)||isErr(x6)||isErr(x7)||isErr(x8)||isErr(x9)||isErr(x10)||isErr(x11)||isErr(x12)||isErr(x13)||isErr(x14);
}

bool TPExcelInterfaceFn::isErr( LPXLOPER x1, LPXLOPER x2, LPXLOPER x3, LPXLOPER x4, LPXLOPER x5, LPXLOPER x6, LPXLOPER x7, LPXLOPER x8, LPXLOPER x9, LPXLOPER x10, LPXLOPER x11, LPXLOPER x12, LPXLOPER x13, LPXLOPER x14, LPXLOPER x15 ){
    return isErr(x1)||isErr(x2)||isErr(x3)||isErr(x4)||isErr(x5)||isErr(x6)||isErr(x7)||isErr(x8)||isErr(x9)||isErr(x10)||isErr(x11)||isErr(x12)||isErr(x13)||isErr(x14)||isErr(x15);
}

bool TPExcelInterfaceFn::isErr( LPXLOPER x1, LPXLOPER x2, LPXLOPER x3, LPXLOPER x4, LPXLOPER x5, LPXLOPER x6, LPXLOPER x7, LPXLOPER x8, LPXLOPER x9, LPXLOPER x10, LPXLOPER x11, LPXLOPER x12, LPXLOPER x13, LPXLOPER x14, LPXLOPER x15, LPXLOPER x16 ){
    return isErr(x1)||isErr(x2)||isErr(x3)||isErr(x4)||isErr(x5)||isErr(x6)||isErr(x7)||isErr(x8)||isErr(x9)||isErr(x10)||isErr(x11)||isErr(x12)||isErr(x13)||isErr(x14)||isErr(x15)||isErr(x16);
}

bool TPExcelInterfaceFn::isErr( LPXLOPER x1, LPXLOPER x2, LPXLOPER x3, LPXLOPER x4, LPXLOPER x5, LPXLOPER x6, LPXLOPER x7, LPXLOPER x8, LPXLOPER x9, LPXLOPER x10, LPXLOPER x11, LPXLOPER x12, LPXLOPER x13, LPXLOPER x14, LPXLOPER x15, LPXLOPER x16, LPXLOPER x17 ){
    return isErr(x1)||isErr(x2)||isErr(x3)||isErr(x4)||isErr(x5)||isErr(x6)||isErr(x7)||isErr(x8)||isErr(x9)||isErr(x10)||isErr(x11)||isErr(x12)||isErr(x13)||isErr(x14)||isErr(x15)||isErr(x16)||isErr(x17);
}

bool TPExcelInterfaceFn::isErr( LPXLOPER x1, LPXLOPER x2, LPXLOPER x3, LPXLOPER x4, LPXLOPER x5, LPXLOPER x6, LPXLOPER x7, LPXLOPER x8, LPXLOPER x9, LPXLOPER x10, LPXLOPER x11, LPXLOPER x12, LPXLOPER x13, LPXLOPER x14, LPXLOPER x15, LPXLOPER x16, LPXLOPER x17, LPXLOPER x18 ){
    return isErr(x1)||isErr(x2)||isErr(x3)||isErr(x4)||isErr(x5)||isErr(x6)||isErr(x7)||isErr(x8)||isErr(x9)||isErr(x10)||isErr(x11)||isErr(x12)||isErr(x13)||isErr(x14)||isErr(x15)||isErr(x16)||isErr(x17)||isErr(x18);
}

bool TPExcelInterfaceFn::isErr( LPXLOPER x1, LPXLOPER x2, LPXLOPER x3, LPXLOPER x4, LPXLOPER x5, LPXLOPER x6, LPXLOPER x7, LPXLOPER x8, LPXLOPER x9, LPXLOPER x10, LPXLOPER x11, LPXLOPER x12, LPXLOPER x13, LPXLOPER x14, LPXLOPER x15, LPXLOPER x16, LPXLOPER x17, LPXLOPER x18, LPXLOPER x19 ){
    return isErr(x1)||isErr(x2)||isErr(x3)||isErr(x4)||isErr(x5)||isErr(x6)||isErr(x7)||isErr(x8)||isErr(x9)||isErr(x10)||isErr(x11)||isErr(x12)||isErr(x13)||isErr(x14)||isErr(x15)||isErr(x16)||isErr(x17)||isErr(x18)||isErr(x19);
}

bool TPExcelInterfaceFn::isErr( LPXLOPER x1, LPXLOPER x2, LPXLOPER x3, LPXLOPER x4, LPXLOPER x5, LPXLOPER x6, LPXLOPER x7, LPXLOPER x8, LPXLOPER x9, LPXLOPER x10, LPXLOPER x11, LPXLOPER x12, LPXLOPER x13, LPXLOPER x14, LPXLOPER x15, LPXLOPER x16, LPXLOPER x17, LPXLOPER x18, LPXLOPER x19, LPXLOPER x20 ){
    return isErr(x1)||isErr(x2)||isErr(x3)||isErr(x4)||isErr(x5)||isErr(x6)||isErr(x7)||isErr(x8)||isErr(x9)||isErr(x10)||isErr(x11)||isErr(x12)||isErr(x13)||isErr(x14)||isErr(x15)||isErr(x16)||isErr(x17)||isErr(x18)||isErr(x20);
}

///////////////////////////////////////////////////////////////////////////////
bool TPExcelInterfaceFn::isUsable( LPXLOPER inXLoper )
{
    return ! ( (inXLoper->xltype & xltypeType) & ( xltypeMissing & xltypeErr & xltypeNil ) );
}

///////////////////////////////////////////////////////////////////////////////
bool TPExcelInterfaceFn::isDoubleVector( LPXLOPER inXLoper )
{
    bool rtn = false;

    if( isMulti(inXLoper) )
    {
        rtn = isDouble( inXLoper->val.array.lparray );
    }
    else 
    {
        rtn = isDouble(inXLoper);
    }

    return rtn;
}

///////////////////////////////////////////////////////////////////////////////
bool TPExcelInterfaceFn::isStringVector( LPXLOPER inXLoper )
{
    bool rtn = false;

    if( isMulti(inXLoper) )
    {
        rtn = isString( inXLoper->val.array.lparray );       
    }
    else 
    {
        rtn = isString(inXLoper);
    }

    return rtn;
}

///////////////////////////////////////////////////////////////////////////////
bool TPExcelInterfaceFn::isDateVector( LPXLOPER inXLoper )
{
    bool rtn = false;

    if( isMulti(inXLoper) )
    {
        rtn = isDate( inXLoper->val.array.lparray );       
    }
    else 
    {
        rtn = isDate(inXLoper);
    }

    return rtn;
}

///////////////////////////////////////////////////////////////////////////////
bool TPExcelInterfaceFn::isNumberVector( LPXLOPER inXLoper )
{
    bool rtn = false;

    if( isMulti(inXLoper) )
    {
        rtn = isNumber( inXLoper->val.array.lparray );        
    }
    else 
    {
        rtn = isNumber(inXLoper);
    }

    return rtn;
}

///////////////////////////////////////////////////////////////////////////////
bool TPExcelInterfaceFn::isLongVector( LPXLOPER inXLoper )
{
    bool rtn = false;

    if( isMulti(inXLoper) )
    {
        rtn = isLong( inXLoper->val.array.lparray );        
    }
    else 
    {
        rtn = isLong(inXLoper);
    }

    return rtn;
}

///////////////////////////////////////////////////////////////////////////////
bool TPExcelInterfaceFn::isBoolVector( LPXLOPER inXLoper )
{
    bool rtn = false;

    if( isMulti(inXLoper) )
    {
        rtn = isBool( inXLoper->val.array.lparray );        
    }
    else 
    {
        rtn = isBool(inXLoper);
    }

    return rtn;
}

////////////////////////////////////////////////////////////////////////////////////
// Data retrieval from an XLOPER:
bool TPExcelInterfaceFn::getBool( LPXLOPER inXLoper, const string & argIdentity )
{
	if( (inXLoper->xltype & xltypeType) != xltypeBool )		// Wrong data type
    {
	   THROW2( argIdentity," : Invalid Bool" )
    }

	return( inXLoper->val.xlbool );
}

////////////////////////////////////////////////////////////////////////////////////
double TPExcelInterfaceFn::getDouble( LPXLOPER inXLoper, const string & argIdentity )
{
    if (inXLoper){
        switch ( inXLoper->xltype & xltypeType ){
        case xltypeNum:
            return( inXLoper->val.num );
        case xltypeInt:
            return( inXLoper->val.w );
        default:
            ;
        }
    }
    THROW2( argIdentity," : Invalid or missing floating point number" );
    return( TP_IEEEUtils::NaN );
}

////////////////////////////////////////////////////////////////////////////////////
double TPExcelInterfaceFn::getDouble( LPXLOPER inXLoper, const string & argIdentity, double defaultValue )
{
    if (inXLoper){
        switch ( inXLoper->xltype & xltypeType ){
        case xltypeNum:
            return( inXLoper->val.num );
        case xltypeInt:
            return( inXLoper->val.w );
        case xltypeMissing:
        case xltypeNil:
            return( defaultValue );
        default:
            ;
        }
    }
    THROW2( argIdentity," : Invalid floating point number" );
    return( TP_IEEEUtils::NaN );
}

///////////////////////////////////////////////////////////////////////////////
long TPExcelInterfaceFn::getLong( LPXLOPER inXLoper, const string & argIdentity )
{
    if (inXLoper){
        switch ( inXLoper->xltype & xltypeType ){
        case xltypeNum:
            return( (long) inXLoper->val.num );
        case xltypeInt:
            return( (long) inXLoper->val.w );
        default:
            ;
        }
    }
    THROW2( argIdentity," : Invalid Long" );
    return LONG_MIN;
}

///////////////////////////////////////////////////////////////////////////////
long TPExcelInterfaceFn::getLong( LPXLOPER inXLoper, const string & argIdentity, long defaultValue )
{
    if (inXLoper){
        switch ( inXLoper->xltype & xltypeType ){
        case xltypeNum:
            return( (long) inXLoper->val.num );
        case xltypeInt:
            return( (long) inXLoper->val.w );
        default:
            ;
        }
    }
    return defaultValue;
}

///////////////////////////////////////////////////////////////////////////////
int TPExcelInterfaceFn::getInt( LPXLOPER inXLoper, const string & argIdentity )
{
	if( !isLong(inXLoper) )
    {
	   THROW2( argIdentity," : Invalid Integer" )
    }

	return( (int) inXLoper->val.num );
}

///////////////////////////////////////////////////////////////////////////////
TPDate TPExcelInterfaceFn::getDate( LPXLOPER inXLoper, const string & argIdentity )
{
	if( !isDate( inXLoper ) )
    {
	   THROW2(argIdentity," : Invalid Date")
    }

	return( TPDate( (const long) inXLoper->val.num ) );
}

////////////////////////////////////////////////////////////////////////////////////
string TPExcelInterfaceFn::getString( LPXLOPER inXLoper, const string &argIdentity )
{
    if (inXLoper){
        switch ( inXLoper->xltype & xltypeType ){
        case xltypeStr:
            // Excel uses the old Pascal string format. This means,
            // the first byte of the inXLoper's data is the string length.
            return( string( &inXLoper->val.str[1] , (inXLoper->val.str[0] & 255) ) );
        default:
            ;
        }
    }
    THROW2( argIdentity," : Invalid or missing string" );
    return( "An invalid or no string was specified." );
}

////////////////////////////////////////////////////////////////////////////////////
string TPExcelInterfaceFn::getString( LPXLOPER inXLoper, const string &argIdentity, string defaultValue )
{
    if (inXLoper){
        switch ( inXLoper->xltype & xltypeType ){
        case xltypeStr:
            // Excel uses the old Pascal string format. This means,
            // the first byte of the inXLoper's data is the string length.
            return( string( &inXLoper->val.str[1] , (inXLoper->val.str[0] & 255) ) );
        case xltypeMissing:
        case xltypeNil:
            return( defaultValue );
        default:
            ;
        }
    }
    THROW2( argIdentity," : Invalid string" );
    return( "An invalid string was specified." );
}

///////////////////////////////////////////////////////////////////////////////
TPAny TPExcelInterfaceFn::getValue(LPXLOPER inXLoper, const string & argIdentity)
{
  if(isDouble(inXLoper))
  {
    return getDouble( inXLoper, argIdentity );
  }
  else if(isBool(inXLoper))
  {
    return getBool( inXLoper, argIdentity );
  }
  else if(isString(inXLoper))
  {
    return getString( inXLoper, argIdentity );
  }
  // else

  return TPAny();
}

///////////////////////////////////////////////////////////////////////////////
//  The 1D Vectors:
// Specific case of Boolean, No Default value in case of Blank cells.
vector<bool> TPExcelInterfaceFn::getBoolVector( LPXLOPER inXLoper, bool bRemoveEmptyAtEnd, const string & argIdentity )
{
	if( !isMulti(inXLoper) )					// Needed when inputting through the function wizard
    {
	    if( inXLoper->xltype == xltypeBool ) 
        {
		    return( vector<bool>( 1, getBool(inXLoper,argIdentity) ) );  
        }
	    else						// Wrong data type
        {
	        THROW2( argIdentity, " : Invalid Bool as part of Vector" )
        }
    }

	// Determine if the input array is a column or a row
	int numElements;
	if( inXLoper->val.array.rows == 1 )
    {
	    numElements = inXLoper->val.array.columns;
    }
	else
    {
	    numElements = inXLoper->val.array.rows;
    }

    // If needed, ignore all the empty cells at the end
    int index = numElements - 1;
    if ( bRemoveEmptyAtEnd )
    {
        while ( inXLoper->val.array.lparray[ index ].xltype & xltypeNil )
        {
            index --;
            if ( index == 0 )
                break;
        }
    }
    
    // Extract all the doubles starting from the end
    bool foundValidEntry = false;
	list<bool> tmpList;
    for( ; index >= 0; index -- ) 
    {
        // ignore blank cells and error cells
        if( !(inXLoper->val.array.lparray[ index ].xltype & (xltypeNil | xltypeErr)))
        {
           // Gets the value
            tmpList.push_front( getBool( &(inXLoper->val.array.lparray[ index ]), argIdentity ) );
            foundValidEntry = true;
        }
        else
        {
            // THROW2 an error if a valid entry was found previously
            if ( foundValidEntry )
            {
	            THROW2( argIdentity, " : Invalid Bool as part of Vector" )
            }
        }
    }

    // Dumps the list into a vector<>
    vector<bool> outVector( tmpList.size() );
    list<bool>::const_iterator it = tmpList.begin();
    for ( int i = 0; it != tmpList.end() ; it ++ , i++)
    {
        outVector[i] = (*it);
    }

	return( outVector );			// return by value
}

///////////////////////////////////////////////////////////////////////////////
vector<double> TPExcelInterfaceFn::getDoubleVector( LPXLOPER inXLoper,  bool bRemoveEmptyAtEnd , const string & argIdentity )
{
	// Needed when inputing through the function wizard
    if( inXLoper->xltype != xltypeMulti )
    {
	    if( inXLoper->xltype == xltypeNum ) 
        {
		    return( vector<double>( 1, getDouble(inXLoper,argIdentity) ) );  
        }
	    else						// Wrong data type
        {
	        THROW2( argIdentity, " : Invalid Bool as part of Vector" )
        }
    }

	// Determine if the input array is a column or a row
	int numElements;
	if( inXLoper->val.array.rows == 1 )
    {
	    numElements = inXLoper->val.array.columns;
    }
	else
    {
	    numElements = inXLoper->val.array.rows;
    }

    // If needed, ignore all the empty cells at the end
    int index = numElements - 1;
    if ( bRemoveEmptyAtEnd )
    {
        while ( inXLoper->val.array.lparray[ index ].xltype & xltypeNil )
        {
            index --;
            if ( index == 0 )
                break;
        }
    }
    
    // Extract all the doubles starting from the end
    bool foundValidEntry = false;
	list<double> tmpList;
    for( ; index >= 0; index -- ) 
    {
        switch ( inXLoper->val.array.lparray[ index ].xltype )
        {
        case xltypeErr: 
            // THROW2 an error if a valid entry was found previously
            if ( foundValidEntry )
            {
	            THROW2( argIdentity, " : Invalid Entry as part of Vector" )
            }
            break;
        case xltypeNil: 
            // Default any (intermediate) blank value 
            tmpList.push_front( BLANK_CELL_DOUBLE );
            break;
        default: 
            // Gets the value
            tmpList.push_front( getDouble( &(inXLoper->val.array.lparray[ index ]), argIdentity ) );
            foundValidEntry = true;
            break;
        }
    }

    // Dumps the list into a vector<>
    vector<double> outVector( tmpList.size() );
    list<double>::const_iterator it = tmpList.begin();
    for ( int i = 0; it != tmpList.end() ; it ++ , i++)
    {
        outVector[i] = (*it);
    }

	return( outVector ); // return by value
}

///////////////////////////////////////////////////////////////////////////////
vector<long> TPExcelInterfaceFn::getLongVector( LPXLOPER inXLoper, bool bRemoveEmptyAtEnd , const string & argIdentity )
{
    if( inXLoper->xltype != xltypeMulti )					
    {
        if( inXLoper->xltype == xltypeNum ) 
        {
            return( vector<long>( 1, getLong(inXLoper, argIdentity ) ) );  
        }
        else													
        {
            THROW2( argIdentity, " : Invalid Long as part of Vector" )
        }
    }

	int numElements;
	if( inXLoper->val.array.rows == 1 )
    {
	    numElements = inXLoper->val.array.columns;
    }
	else
    {
	    numElements = inXLoper->val.array.rows;
    }

    // If needed, ignore all the empty cells at the end
    int index = numElements - 1;
    if ( bRemoveEmptyAtEnd )
    {
        while ( inXLoper->val.array.lparray[ index ].xltype & xltypeNil )
        {
            index --;
            if ( index == 0 )
                break;
        }
    }
    
    // Extract all the longs starting from the end
    bool foundValidEntry = false;
	list<long> tmpList;
    for( ; index >= 0; index -- ) 
    {
        switch ( inXLoper->val.array.lparray[ index ].xltype )
        {
        case xltypeErr: 
            // THROW2 an error if a valid entry was found previously
            if ( foundValidEntry )
            {
	            THROW2( argIdentity, " : Invalid Entry as part of Vector" )
            }
            break;
        case xltypeNil: 
            // Default any (intermediate) blank value 
            tmpList.push_front( BLANK_CELL_LONG );
            break;
        default: 
            // Gets the value
            tmpList.push_front( getLong( &(inXLoper->val.array.lparray[ index ]), argIdentity ) );
            foundValidEntry = true;
            break;
        }
    }

    // Dumps the list into a vector<>
    vector<long> outVector( tmpList.size() );
    list<long>::const_iterator it = tmpList.begin();
    for ( int i = 0; it != tmpList.end() ; it ++ , i++)
    {
        outVector[i] = (*it);
    }

	return( outVector );
}

///////////////////////////////////////////////////////////////////////////////
vector<int> TPExcelInterfaceFn::getIntVector( LPXLOPER inXLoper, bool bRemoveEmptyAtEnd , const string & argIdentity )
{

    if( inXLoper->xltype != xltypeMulti )					
    {
        if( inXLoper->xltype == xltypeNum ) 
        {
            return( vector<int>( 1, getInt(inXLoper, argIdentity ) ) );  
        }
        else													
        {
            THROW2( argIdentity, " : Invalid Integer as part of Vector" )
        }
    }

	int numElements;
	if( inXLoper->val.array.rows == 1 )
    {
	    numElements = inXLoper->val.array.columns;
    }
	else
    {
	    numElements = inXLoper->val.array.rows;
    }

    // If needed, ignore all the empty cells at the end
    int index = numElements - 1;
    if ( bRemoveEmptyAtEnd )
    {
        while ( inXLoper->val.array.lparray[ index ].xltype & xltypeNil )
        {
            index --;
            if ( index == 0 )
                break;
        }
    }
    
    // Extract all the longs starting from the end
    bool foundValidEntry = false;
	list<int> tmpList;
    for( ; index >= 0; index -- ) 
    {
        switch ( inXLoper->val.array.lparray[ index ].xltype )
        {
        case xltypeErr: 
            // THROW2 an error if a valid entry was found previously
            if ( foundValidEntry )
            {
	            THROW2( argIdentity, " : Invalid Entry as part of Vector" )
            }
            break;
        case xltypeNil: 
            // Default any (intermediate) blank value 
            tmpList.push_front( BLANK_CELL_INT );
            break;
        default: 
            // Gets the value
            tmpList.push_front( getInt( &(inXLoper->val.array.lparray[ index ]), argIdentity ) );
            foundValidEntry = true;
            break;
        }
    }

    // Dumps the list into a vector<>
    vector<int> outVector( tmpList.size() );
    list<int>::const_iterator it = tmpList.begin();
    for ( int i = 0; it != tmpList.end() ; it ++ , i++)
    {
        outVector[i] = (*it);
    }

	return( outVector );
}

///////////////////////////////////////////////////////////////////////////////
vector<string> 
TPExcelInterfaceFn::getStringVector( LPXLOPER inXLoper, bool bRemoveEmptyAtEnd , const string & argIdentity )
{
	// Needed when inputing through the function wizard
    if( inXLoper->xltype != xltypeMulti )					
    {
        if( isString( inXLoper ) ) 
        {
            return( vector<string>( 1, getString(inXLoper, argIdentity ) ) );  
        }
        else													
        {
            THROW2( argIdentity, " : Invalid String as part of Vector" )
        }
    }

	int numElements;
	if( inXLoper->val.array.rows == 1 )
    {
	    numElements = inXLoper->val.array.columns;
    }
	else
    {
	    numElements = inXLoper->val.array.rows;
    }


    // If needed, ignore all the empty cells at the end
    int index = numElements - 1;
    if ( bRemoveEmptyAtEnd )
    {
        while ( inXLoper->val.array.lparray[ index ].xltype & xltypeNil )
        {
            index --;
            if ( index == 0 )
                break;
        }
    }
    
    // Extract all the strings starting from the end
    bool foundValidEntry = false;
	list<string> tmpList;
    for( ; index >= 0; index -- ) 
    {
        switch ( inXLoper->val.array.lparray[ index ].xltype )
        {
        case xltypeErr: 
            // THROW2 an error if a valid entry was found previously
            if ( foundValidEntry )
            {
	            THROW2( argIdentity, " : Invalid String as part of Vector" )
            }
            break;
        case xltypeNil: 
            // Default any blank value to an empty string
            tmpList.push_front( BLANK_CELL_STRING );
            break;
        default: 
            // Gets the value
            tmpList.push_front( getString( &(inXLoper->val.array.lparray[ index ]), argIdentity ) );
            foundValidEntry = true;
            break;
        }
    }

    // Dumps the list into a vector<>
    vector<string> outVector( tmpList.size() );
    list<string>::const_iterator it = tmpList.begin();
    for ( int i = 0; it != tmpList.end() ; it ++ , i++)
    {
        outVector[i] = (*it);
    }

	return( outVector );
}

///////////////////////////////////////////////////////////////////////////////
//vector<TPDate> TPExcelInterfaceFn::getDateVector( LPXLOPER inXLoper, bool bRemoveEmptyAtEnd , const string & argIdentity )
//{
//    if( inXLoper->xltype != xltypeMulti )					// Needed when inputing through the function wizard
//    {
//        if( isDate( inXLoper ) ) 
//        {
//            return( vector<TPDate>( 1, getDate(inXLoper, argIdentity ) ) );  
//        }
//        else												
//        {
//            THROW2( argIdentity, " : Invalid Date as part of Vector" )
//        }
//    }
//
//	int numElements;
//	if( inXLoper->val.array.rows == 1 )
//    {
//        numElements = inXLoper->val.array.columns;
//    }
//	else
//    {
//        numElements = inXLoper->val.array.rows;
//    }
//
//    // If needed, ignore all the empty cells at the end
//    int index = numElements - 1;
//    if ( bRemoveEmptyAtEnd )
//    {
//        while ( inXLoper->val.array.lparray[ index ].xltype & xltypeNil )
//        {
//            index --;
//            if ( index == 0 )
//                break;
//        }
//    }
//    
//    // Extract all the TPDates starting from the end
//    bool foundValidEntry = false;
//	list<TPDate> tmpList;
//    for( ; index >= 0; index -- ) 
//    {
//        switch ( inXLoper->val.array.lparray[ index ].xltype )
//        {
//        case xltypeErr: 
//            // THROW2 an error if a valid entry was found previously
//            if ( foundValidEntry )
//            {
//	            THROW2( argIdentity, " : Invalid Entry as part of Vector" )
//            }
//            break;
//        case xltypeNil: 
//            // Default any (intermediate) blank value 
//            tmpList.push_front( BLANK_CELL_DATE );
//            break;
//        default: 
//            // Gets the value
//            tmpList.push_front( getDate( &(inXLoper->val.array.lparray[ index ]), argIdentity ) );
//            foundValidEntry = true;
//            break;
//        }
//    }
//
//    // Dumps the list into a vector<>
//    vector<TPDate> outVector( tmpList.size() );
//    list<TPDate>::const_iterator it = tmpList.begin();
//    for ( int i = 0; it != tmpList.end() ; it ++ , i++)
//    {
//        outVector[i] = (*it);
//    }
//	return( outVector );
//}

///////////////////////////////////////////////////////////////////////////////
//vector<TPAny> TPExcelInterfaceFn::getValueVector( LPXLOPER inXLoper, const string & argIdentity )
//{
//    if( inXLoper->xltype != xltypeMulti )					// Needed when inputing through the function wizard
//    {
//        return( vector<TPAny>( 1, getValue(inXLoper, argIdentity) ) );  
//    }
//
//	int numElements;
//    bool rowVectorFlag = false;;
//	if( inXLoper->val.array.rows == 1 )
//    {
//	    numElements = inXLoper->val.array.columns;
//    }
//	else
//    {
//	    numElements = inXLoper->val.array.rows;
//        rowVectorFlag = true;
//    }
//
//	vector<TPAny> outVector(numElements);
//
//    for( int index = 0; index < numElements; ++index) 
//    {
//        if(rowVectorFlag)
//        {
//            outVector[index] = getValue(inXLoper, index, 0, argIdentity);
//        }
//        else
//        {
//            outVector[index] = getValue(inXLoper, 0, index, argIdentity);
//        }
//    }
//
//	return( outVector );
//}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//  The 2D Vectors:
//  NOTE: All 2D arrays fill in the array element corresponding to a 
//        blank Excel cell with the default
//		    values defined in the header file.
//
TPMatrix<double> TPExcelInterfaceFn::getDoubleMatrix( LPXLOPER inXLoper, const string & argIdentity )
{
    if( inXLoper->xltype != xltypeMulti )
    {
        if( inXLoper->xltype == xltypeNum ) 
        {
            return( TPMatrix<double>( 1,1, getDouble(inXLoper, argIdentity) ) );  
        }
        else
        {
            THROW2( argIdentity, " : Invalid Double as part of 2D Vector" )
        }
    }

	// Create an array and populate it from the given XLOPER array
	int numColumns = inXLoper->val.array.columns;
	int numRows = inXLoper->val.array.rows;

	if( numColumns == 0 || numRows == 0 )
    {
	   THROW2( argIdentity, " : Double 2D Vector has one dimension of zero length" )
    }

	TPMatrix<double> outVector( numRows, numColumns );
	double theDouble;
	int i, j;
	int index;
	for( i = 0; i < numRows; i++ )
    {
        for( j = 0; j < numColumns; j++ ) 
        {
            index = i * numColumns + j;
            if( inXLoper->val.array.lparray[ index ].xltype & (xltypeNil | xltypeErr) ) 	// Fill in blank and error cells with a default value
            {
                theDouble = BLANK_CELL_DOUBLE;
            }
            else 
            {
                theDouble = getDouble( &(inXLoper->val.array.lparray[ index ]), argIdentity );
                outVector[i][j] = theDouble;
            }
        }
    }

	return( outVector );			// return by value
}

///////////////////////////////////////////////////////////////////////////////
TPMatrix<long> TPExcelInterfaceFn::getLongMatrix( LPXLOPER inXLoper, const string & argIdentity )
{
	if( inXLoper->xltype != xltypeMulti )					// Needed when inputing through the function wizard
    {
        if( inXLoper->xltype == xltypeNum ) 
        {
            return( TPMatrix<long>( 1,1, getLong(inXLoper,argIdentity) ) );  
        }
        else													
        {
            THROW2( argIdentity, " : Invalid Long as part of 2D Vector" )
        }
    }

	int numColumns = inXLoper->val.array.columns;
	int numRows = inXLoper->val.array.rows;

	if( numColumns == 0 || numRows == 0 ) 											
    {
	    THROW2( argIdentity, " : Long 2D Vector has one dimension of zero length" )
    }

	TPMatrix<long> outVector( numRows, numColumns );
	long theLong;
	int i, j;
	int index;
	for( i = 0; i < numRows; i++ )
    {
        for( j = 0; j < numColumns; j++ ) 
        {
            index = i * numColumns + j;
            if( inXLoper->val.array.lparray[ index ].xltype & (xltypeNil | xltypeErr) ) 	// Fill in blank and error cells with a default value
            {
                theLong = BLANK_CELL_LONG;
            }
            else 
            {
                theLong = getLong( &(inXLoper->val.array.lparray[ index ]),argIdentity );
                outVector[i][j] = theLong;
            }
        }
    }

	return( outVector );		
}

///////////////////////////////////////////////////////////////////////////////
TPMatrix<int> TPExcelInterfaceFn::getIntMatrix( LPXLOPER inXLoper, const string & argIdentity )
{
    if( inXLoper->xltype != xltypeMulti )					// Needed when inputing through the function wizard
    {
        if( inXLoper->xltype == xltypeNum ) 
        {
            return( TPMatrix<int>( 1,1, getInt(inXLoper, argIdentity) ) );  
        }
        else													
        {
            THROW2( argIdentity, " : Invalid Integer as part of 2D Vector" )
        }
    }

	int numColumns = inXLoper->val.array.columns;
	int numRows = inXLoper->val.array.rows;

    if( numColumns == 0 || numRows == 0 ) 											
    {
        THROW2( argIdentity, " : Integer 2D Vector has one dimension of zero length" )
    }

	TPMatrix<int> outVector( numRows, numColumns );
	int theInteger;
	int i, j;
	int index;
	for( i = 0; i < numRows; i++ )
    {
        for( j = 0; j < numColumns; j++ ) 
        {
            index = i * numColumns + j;
            if( inXLoper->val.array.lparray[ index ].xltype & (xltypeNil | xltypeErr) ) 	// Fill in blank and error cells with a default value
            {
                theInteger = BLANK_CELL_INT;
            }
            else 
            {
                theInteger = getInt( &(inXLoper->val.array.lparray[ index ]), argIdentity );
            }

            outVector[i][j] = theInteger;
        }
    }

	return( outVector );		
}

///////////////////////////////////////////////////////////////////////////////
TPMatrix<string> TPExcelInterfaceFn::getStringMatrix( LPXLOPER inXLoper, const string & argIdentity )
{
    if( inXLoper->xltype != xltypeMulti )					// Needed when inputing through the function wizard
    {
        if( isString( inXLoper ) ) 
        {
            return( TPMatrix<string>( 1,1, getString(inXLoper,argIdentity) ) );  
        }
        else											
        {
            THROW2( argIdentity, " : Invalid String as part of 2D Vector" )
        }
    }

	int numColumns = inXLoper->val.array.columns;
	int numRows = inXLoper->val.array.rows;

	if( numColumns == 0 || numRows == 0 ) 												
    {
	    THROW2( argIdentity, " : String 2D Vector has one dimension of zero length" )
    }

	TPMatrix<string> outVector( numRows, numColumns );
	string theString;
	int i, j;
	int index;
	for( i = 0; i < numRows; i++ )
    {
        for( j = 0; j < numColumns; j++ ) 
        {
            index = i * numColumns + j;
            if( inXLoper->val.array.lparray[ index ].xltype & (xltypeNil | xltypeErr) ) 	// Fill in blank and error cells with a default value
            {
                theString = BLANK_CELL_STRING;
            }
            else 
            {
                theString = getString( &(inXLoper->val.array.lparray[ index ]),argIdentity );
            }

            outVector[i][j] = theString;
        }
    }

	return( outVector );		
}

///////////////////////////////////////////////////////////////////////////////
//TPMatrix<TPDate> TPExcelInterfaceFn::getDateMatrix( LPXLOPER inXLoper, const string & argIdentity )
//{
//	if( inXLoper->xltype != xltypeMulti )					// Needed when inputing through the function wizard
//    {
//        if( isDate( inXLoper ) ) 
//        {
//            return( TPMatrix<TPDate>( 1,1, getDate(inXLoper,argIdentity) ) );  
//        }
//        else													
//        {
//            THROW2( argIdentity, " : Invalid Date as part of 2D Vector" )
//        }
//    }
//
//	int numColumns = inXLoper->val.array.columns;
//	int numRows = inXLoper->val.array.rows;
//
//	if( numColumns == 0 || numRows == 0 ) 											
//    {
//	    THROW2( argIdentity, " : Date 2D Vector has one dimension of zero length" )
//    }
//
//	TPMatrix<TPDate> outVector( numRows, numColumns );
//	TPDate theDate;
//	int i, j;
//	int index;
//	for( i = 0; i < numRows; i++ )
//    {
//        for( j = 0; j < numColumns; j++ ) 
//        {
//            index = i * numColumns + j;
//            if( inXLoper->val.array.lparray[ index ].xltype & (xltypeNil | xltypeErr) ) 	// Fill in blank and error cells with a default value
//            {
//                theDate = TPDate( BLANK_CELL_DATE );
//            }
//            else 
//            {
//                theDate = getDate( &(inXLoper->val.array.lparray[ index ]), argIdentity );
//            }
//
//            outVector[i][j] = theDate;
//        }
//    }
//
//	return( outVector );		
//}

////////////////////////////////////////////////////////////////////////////////////
//TPMatrix<TPAny> TPExcelInterfaceFn::getValueMatrix( LPXLOPER inXLoper, const string & argIdentity )
//{
//	if( inXLoper->xltype != xltypeMulti )					// Needed when inputing through the function wizard
//    {
//       return( TPMatrix<TPAny>( 1,1, getValue(inXLoper,argIdentity) ) );  
//    }
//
//	int numColumns = inXLoper->val.array.columns;
//	int numRows = inXLoper->val.array.rows;
//
//	if( numColumns == 0 || numRows == 0 )
//    {
//	    return( TPMatrix<TPAny>(1,1, TPAny()) );
//    }
//
//	TPMatrix<TPAny> outVector( numRows, numColumns );
//
//	int i, j;
//	for( i = 0; i < numRows; i++ )
//    {
//        for( j = 0; j < numColumns; j++ ) 
//        {
//            outVector[i][j] = getValue(inXLoper, i, j,argIdentity);
//        }
//    }
//
//	return( outVector );		
//}

////////////////////////////////////////////////////////////////////////////////////

// Reads a 2-column array where the first column contains strings (Name) and the secondcontains Values
//vector< pair< string, TPAny> >
//TPExcelInterfaceFn::getNameValuePairVector( LPXLOPER inXLoper, const char* argIdentity )
//{
//    vector< pair< string, TPAny> > emptyVector( 0 );
//    
//    // Check that the input LPXLOPER is not empty, in which case not much to do
//    if ( inXLoper->xltype & xltypeMissing )
//    {
//        return emptyVector;
//    }
//
//    // Check that the input LPXLOPER is an array indeed
//    if( inXLoper->xltype != xltypeMulti )
//    {
//	    THROW2( argIdentity, " : must be a 2-column range of <name,value> pairs" );
//        return emptyVector;
//    }
//
//
//	int numColumns = inXLoper->val.array.columns;
//	int numRows = inXLoper->val.array.rows;
//
//	if( numColumns == 0 || numRows == 0 )
//    {
//        return emptyVector;
//    }
//
//    // Check the number of columns: must be 2 ( <name,value> pairs )
//    if( numColumns != 2 )
//    {
//	    THROW2( argIdentity, " : must be a 2-column range of <name,value> pairs" );
//        return emptyVector;
//    }
//
//	vector< pair< string, TPAny> >  outVector( numRows );
//	for( int i = 0; i < numRows; i++ )
//    {
//        // Name in first column
//        TPAny vName = getValue( inXLoper, i, 0,argIdentity); 
//        if (! vName.isString() )
//        {
//	        THROW2( argIdentity, " : the first column must contain only strings (Names)" );
//            return emptyVector;
//        }
//        string szName = vName.castString();
//        
//        // Value in second column
//        TPAny vValue = getValue(inXLoper, i, 1,argIdentity);
//
//        outVector[i] = pair< string, TPAny>( szName, vValue );
//    }
//
//	return( outVector );		
//}

////////////////////////////////////////////////////////////////////////////////////
TPAny TPExcelInterfaceFn::getValue( LPXLOPER inXLoper, int r, int c, const string & argIdentity )
{
    if (r < 0 || r > rows(inXLoper))
    {
        THROW2( argIdentity, " : Requested position is out of bounds on the given XLOper." )
    }

    if (c < 0 || c > columns(inXLoper))
    {
        THROW2( argIdentity, " : Requested position is out of bounds on the given XLOper." )
    }

    int index = r * columns(inXLoper) + c;
    return getValue(&inXLoper->val.array.lparray[ index ],argIdentity);
}

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
// member functions for extracting columns of data from and LPXLOPER
vector<double> TPExcelInterfaceFn::getDoubleColumn( LPXLOPER inXLoper, int col, const char* argIdentity)
{
	if( inXLoper->xltype != xltypeMulti )											
    {
        THROW2( argIdentity, " : XLRange of Undefined Type in TPExcelInterfaceFn::getDoubleColumn()" )
    }

	// Create an array and populate it from the given XLOPER array
	int numColumns = inXLoper->val.array.columns;
	int numRows = inXLoper->val.array.rows;

	// Check the column asked for exists
	if( col >= numColumns ) 
    {
        THROW2( argIdentity, " : XLRange of Undefined Type in TPExcelInterfaceFn::getLongColumn()" )
	}
	 
	vector<double> outVector(numRows);
	double theDouble;

	int i;
	int index;
	for( i = 0; i < numRows; i++ ) 
    {
		index = i * numColumns + col;
		theDouble = getDouble( &(inXLoper->val.array.lparray[ index ]), argIdentity );
        outVector[i] = theDouble;
	}

	return outVector;
}

////////////////////////////////////////////////////////////////////////////////////
vector<long> TPExcelInterfaceFn::getLongColumn( LPXLOPER inXLoper, int col, const char* argIdentity)
{
	if( inXLoper->xltype != xltypeMulti )
    {
        THROW2( argIdentity, " : XLRange of Undefined Type in TPExcelInterfaceFn::getLongColumn()" )
    }

	// Create an array and populate it from the given XLOPER array
	int numColumns = inXLoper->val.array.columns;
	int numRows = inXLoper->val.array.rows;

    // Check the column asked for exists
    if(col >= numColumns)
    {
        THROW2( argIdentity, " : XLRange of Undefined Type in TPExcelInterfaceFn::getLongColumn()" )
    }
	 
	vector<long> outVector(numRows);
	long theLong;
	int i;
	int index;
	for( i = 0; i < numRows; i++ )
    {
        index = i * numColumns + col;
        theLong = getLong( &(inXLoper->val.array.lparray[ index ]),argIdentity );
        outVector[i] = theLong;
	}

	return outVector;
}

////////////////////////////////////////////////////////////////////////////////////
vector<int> TPExcelInterfaceFn::getIntColumn( LPXLOPER inXLoper, int col, const char* argIdentity)
{

	if( inXLoper->xltype != xltypeMulti )
    {
        THROW2( argIdentity, " : XLRange of Undefined Type in TPExcelInterfaceFn::getIntColumn()" )
    }

	// Create an array and populate it from the given XLOPER array
	int numColumns = inXLoper->val.array.columns;
	int numRows = inXLoper->val.array.rows;

    // Check the column asked for exists
    if(col >= numColumns)
    {
        THROW2( argIdentity, " : XLRange of Undefined Type in TPExcelInterfaceFn::getLongColumn()" )
    }
	 
	vector<int> outVector(numRows);
	int theInt;

	int i;
	int index;
	for( i = 0; i < numRows; i++ )
    {
	    index = i * numColumns + col;
		theInt = getInt( &(inXLoper->val.array.lparray[ index ]), argIdentity );
	    outVector[i] = theInt;
	}

	return outVector;
}

////////////////////////////////////////////////////////////////////////////////////
vector<string> TPExcelInterfaceFn::getStringColumn( LPXLOPER inXLoper, int col, const char* argIdentity )
{
  // Error Checking
	if( inXLoper->xltype != xltypeMulti )
    {
	   THROW2( argIdentity, " : XLRange of Undefined Type in TPExcelInterfaceFn::getStringColumn()" )
    }

	// Create an array and populate it from the given XLOPER array
	int numColumns = inXLoper->val.array.columns;
	int numRows = inXLoper->val.array.rows;

    // Check the column asked for exists
    if(col >= numColumns)
    {
        THROW2( argIdentity, " : XLRange of Undefined Type in TPExcelInterfaceFn::getLongColumn()" )
    }
	 
	vector<string> outVector(numRows);
	string theString;

	int i;
	int index;
	for( i = 0; i < numRows; i++ )
    {
	    index = i * numColumns + col;
		theString = getString( &(inXLoper->val.array.lparray[ index ]), argIdentity );
	    outVector[i] = theString;
	}

	return outVector;
}

////////////////////////////////////////////////////////////////////////////////////
vector<TPDate> TPExcelInterfaceFn::getDateColumn( LPXLOPER inXLoper, int col, const char* argIdentity )
{
  // Error Checking
	if( inXLoper->xltype != xltypeMulti )
    {
	   THROW2( argIdentity, " : XLRange of Undefined Type in TPExcelInterfaceFn::getDateColumn()" )
    }

	// Create an array and populate it from the given XLOPER array
	int numColumns = inXLoper->val.array.columns;
	int numRows = inXLoper->val.array.rows;

    // Check the column asked for exists
    if(col >= numColumns)
    {
        THROW2( argIdentity, " : XLRange of Undefined Type in TPExcelInterfaceFn::getLongColumn()" )
    }
	 
	vector<TPDate> outVector(numRows);
	TPDate theDate;

	int i;
	int index;
	for( i = 0; i < numRows; i++ )
    {
        index = i * numColumns + col;
        theDate = getDate( &(inXLoper->val.array.lparray[ index ]), argIdentity );
        outVector[i] = theDate;
	}

	return outVector;
}

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
// Constructing an XLOPER:
/////

////////////////////////////////////////////////////////////////////////////////////
LPXLOPER TPExcelInterfaceFn::xlReturn(const double arg)
{
	LPXLOPER lpx;

    lpx = new XLOPER;
    if(lpx == NULL)
    {
        THROW("Unable to create XLOper - out of memory.");
    }

    set(arg, *lpx);

    return lpx;
}

////////////////////////////////////////////////////////////////////////////////////
LPXLOPER TPExcelInterfaceFn::xlReturn(const long arg)
{
	LPXLOPER lpx;

    lpx = new XLOPER;
    if(lpx == NULL)
    {
        TP_THROW2EXCEPTION("Unable to create XLOper - out of memory.");
    }

    set(arg, *lpx);

    return lpx;
}

////////////////////////////////////////////////////////////////////////////////////
LPXLOPER TPExcelInterfaceFn::xlReturn(const string& arg)
{
	LPXLOPER lpx;

    lpx = new XLOPER;
    if(lpx == NULL)
    {
        TP_THROW2EXCEPTION("Unable to create XLOper - out of memory.");
    }

    set(arg, *lpx);

	return lpx;
}

////////////////////////////////////////////////////////////////////////////////////
LPXLOPER TPExcelInterfaceFn::xlReturn(const char* arg)
{
	string str = arg;

    LPXLOPER lpx = new XLOPER;
    if(lpx == NULL)
    {
        TP_THROW2EXCEPTION("Unable to create XLOper - out of memory.");
    }
    set(str, *lpx);

    return lpx;
}

////////////////////////////////////////////////////////////////////////////////////
LPXLOPER TPExcelInterfaceFn::xlReturn(const bool arg)
{
	LPXLOPER lpx;

    lpx = new XLOPER;
    if(lpx == NULL)
    {
        TP_THROW2EXCEPTION("Unable to create XLOper - out of memory.");
    }

    set(arg, *lpx);

	return lpx;
}

/*
////////////////////////////////////////////////////////////////////////////////////
LPXLOPER TPExcelInterfaceFn::xlReturn(const TPAny& arg)
{
	LPXLOPER lpx;

    lpx = new XLOPER;
    if(lpx == NULL)
    {
        TP_THROW2EXCEPTION("Unable to create XLOper - out of memory.");
    }

    set(arg, *lpx);

    return lpx;
}
*/

////////////////////////////////////////////////////////////////////////////////////
LPXLOPER TPExcelInterfaceFn::xlReturn(const vector<double>& arg)
{
	LPXLOPER lpx;

    lpx = new XLOPER;
    if(lpx == NULL)
    {
        TP_THROW2EXCEPTION("Unable to create XLOper - out of memory.");
    }

    set(arg, *lpx);

    return lpx;
}

////////////////////////////////////////////////////////////////////////////////////
LPXLOPER TPExcelInterfaceFn::xlReturn(const vector<string>& arg)
{
	LPXLOPER lpx;

    lpx = new XLOPER;
    if(lpx == NULL)
    {
        TP_THROW2EXCEPTION("Unable to create XLOper - out of memory.");
    }

    set(arg, *lpx);

    return lpx;
}

////////////////////////////////////////////////////////////////////////////////////
//LPXLOPER TPExcelInterfaceFn::xlReturn(const vector<TPAny>& arg)
//{
//	LPXLOPER lpx;
//
//    lpx = new XLOPER;
//    if(lpx == NULL)
//    {
//        TP_THROW2EXCEPTION("Unable to create XLOper - out of memory.");
//    }
//
//    set(arg, *lpx);
//
//	return lpx;
//}

////////////////////////////////////////////////////////////////////////////////////
LPXLOPER TPExcelInterfaceFn::xlReturn(const TPMatrix<double>& arg)
{
	LPXLOPER lpx;

    lpx = new XLOPER;
    if(lpx == NULL)
    {
        TP_THROW2EXCEPTION("Unable to create XLOper - out of memory.");
    }

    set(arg, *lpx);

	return lpx;
}

////////////////////////////////////////////////////////////////////////////////////
LPXLOPER TPExcelInterfaceFn::xlReturn(const TPMatrix<string>& arg)
{
	LPXLOPER lpx;

    lpx = new XLOPER;
    if(lpx == NULL)
    {
        TP_THROW2EXCEPTION("Unable to create XLOper - out of memory.");
    }

    set(arg, *lpx);

	return lpx;
}

/*
////////////////////////////////////////////////////////////////////////////////////
LPXLOPER TPExcelInterfaceFn::xlReturn(const TPMatrix<TPAny>& arg)
{
	LPXLOPER lpx;

    lpx = new XLOPER;
    if(lpx == NULL)
    {
        TP_THROW2EXCEPTION("Unable to create XLOper - out of memory.");
    }

    set(arg, *lpx);

	return lpx;
}
*/
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void TPExcelInterfaceFn::setEmpty(XLOPER& xlOper)
{
    // Displays an empty cell as an empty string
    set(  string(""), xlOper);
}

///////////////////////////////////////////////////////////////////////////////
void TPExcelInterfaceFn::set(const double arg, XLOPER& xlOper)
{
	xlOper.xltype = xltypeNum | xlbitDLLFree; // freed using xlAutoFree
	xlOper.val.num = arg;
}

///////////////////////////////////////////////////////////////////////////////
void TPExcelInterfaceFn::set(const int arg, XLOPER& xlOper)
{
	xlOper.xltype = xltypeNum | xlbitDLLFree; // freed using xlAutoFree
	xlOper.val.num = arg;
}

///////////////////////////////////////////////////////////////////////////////
void TPExcelInterfaceFn::set(const long arg, XLOPER& xlOper)
{
	xlOper.xltype = xltypeNum | xlbitDLLFree; // freed using xlAutoFree
	xlOper.val.num = arg;
}

///////////////////////////////////////////////////////////////////////////////
void TPExcelInterfaceFn::set(const string& arg, XLOPER& xlOper)
{
    int i;
	WORD len;
	
	len = arg.length();
    if (len>255) len = 255; // Truncate to maximum Pascal string length.
	xlOper.xltype = xltypeStr | xlbitDLLFree;   // freed using xlAutoFree
	xlOper.val.str = new char[len + 2];         // For debugging and recycling purposes, let's copy the trailing \0, too, even though it is not part of the Pascal string.
	xlOper.val.str[0] = len;
	for (i = 0; i <= len; ++i)
    {
		xlOper.val.str[i + 1] = arg[i];
    }
}

///////////////////////////////////////////////////////////////////////////////
void TPExcelInterfaceFn::set(const bool arg, XLOPER& xlOper)
{
    xlOper.xltype = xltypeBool | xlbitDLLFree; // freed using xlAutoFree
	xlOper.val.xlbool = arg;
}

///////////////////////////////////////////////////////////////////////////////
void TPExcelInterfaceFn::set(const TPDate& arg, XLOPER& xlOper)
{
    set(arg.excelDate(), xlOper);
}

/*
///////////////////////////////////////////////////////////////////////////////
void TPExcelInterfaceFn::set(const TPAny& arg, XLOPER& xlOper)
{
    switch (arg.type())
    {
    case TPAny::TP_Undefined:
        {
            TP_THROW2EXCEPTION("Undefined Any passed to TPExcelInterfaceFn::set()");
            break;
        }
    case TPAny::TP_Empty:
        {
            setEmpty( xlOper );
            break;
        }
    case TPAny::TP_Bool:
        {
            set(arg.castBool(), xlOper);
            break;
        }
    case TPAny::TP_Integer:
        {
            set(arg.castInt(), xlOper);
            break;
        }
    case TPAny::TP_Long:
        {
            set(arg.castLong(), xlOper);
            break;
        }
    case TPAny::TP_Double:
        {
            set(arg.castDouble(), xlOper);
            break;
        }
    case TPAny::TP_String:
        {
            set(arg.castString(), xlOper);
            break;
        }
    case TPAny::TP_Date:
        {
            set(arg.castTPDate(), xlOper);
            break;
        }
    default:
        {
            TP_THROW2EXCEPTION("Unknown Varaint Type.");
            break;
        }
    }
}
*/

///////////////////////////////////////////////////////////////////////////////
void TPExcelInterfaceFn::set(const vector<double>& arg, XLOPER& xlOper)
{
	int rows = arg.size();
	xlOper.xltype = xltypeMulti | xlbitDLLFree; // freed using xlAutoFree
	xlOper.val.array.columns = 1;
	xlOper.val.array.rows = rows;
	xlOper.val.array.lparray = new XLOPER[rows];

	for (int r = 0; r < rows; ++r)
	{
		XLOPER& x = xlOper.val.array.lparray[r];
		set(arg[r], x);
	}
}

///////////////////////////////////////////////////////////////////////////////
void TPExcelInterfaceFn::set(const vector<string>& arg, XLOPER& xlOper)
{
	int rows = arg.size();
	xlOper.xltype = xltypeMulti | xlbitDLLFree; // freed using xlAutoFree
	xlOper.val.array.columns = 1;
	xlOper.val.array.rows = rows;
	xlOper.val.array.lparray = new XLOPER[rows];

	for (int r = 0; r < rows; ++r)
	{
		XLOPER& x = xlOper.val.array.lparray[r];
		set(arg[r], x);
	}
}

/*
///////////////////////////////////////////////////////////////////////////////
void TPExcelInterfaceFn::set(const vector<TPAny>& arg, XLOPER& xlOper)
{
   	int rows = arg.size();
	xlOper.xltype = xltypeMulti | xlbitDLLFree; // freed using xlAutoFree
	xlOper.val.array.columns = 1;
	xlOper.val.array.rows = rows;
	xlOper.val.array.lparray = new XLOPER[rows];

	for (int r = 0; r < rows; ++r)
	{
		XLOPER& x = xlOper.val.array.lparray[r];
		set(arg[r], x);
	}
}
*/

///////////////////////////////////////////////////////////////////////////////
void TPExcelInterfaceFn::set(const TPMatrix<double>& arg, XLOPER& xlOper)
{
    if ( arg.empty() )
    {
        setEmpty( xlOper );
        return;
    }

	int rows = arg.size(0);
	int cols = arg.size(1);
	xlOper.xltype = xltypeMulti | xlbitDLLFree; // freed using xlAutoFree
	xlOper.val.array.columns = cols;
	xlOper.val.array.rows = rows;
	xlOper.val.array.lparray = new XLOPER[rows * cols];

	for (int r = 0; r < rows; ++r)
    {
		for (int c = 0; c < cols; ++c)
		{
			XLOPER& x = xlOper.val.array.lparray[r * cols + c];
			set(arg.at(r, c), x);
		}
    }
}

///////////////////////////////////////////////////////////////////////////////
void TPExcelInterfaceFn::set(const TPMatrix<string>& arg, XLOPER& xlOper)
{
	    if ( arg.empty() )
    {
        setEmpty( xlOper );
        return;
    }

    int rows = arg.size(0);
	int cols = arg.size(1);
	xlOper.xltype = xltypeMulti | xlbitDLLFree; // freed using xlAutoFree
	xlOper.val.array.columns = cols;
	xlOper.val.array.rows = rows;
	xlOper.val.array.lparray = new XLOPER[rows * cols];

	for (int r = 0; r < rows; ++r)
    {
		for (int c = 0; c < cols; ++c)
		{
			XLOPER& x = xlOper.val.array.lparray[r * cols + c];
			set(arg.at(r, c), x);
		}
    }
}

/*
///////////////////////////////////////////////////////////////////////////////
void TPExcelInterfaceFn::set(const TPMatrix<TPAny>& arg, XLOPER& xlOper)
{
    if ( arg.empty() )
    {
        setEmpty( xlOper );
        return;
    }

    int rows = arg.size(0);
	int cols = arg.size(1);
	xlOper.xltype = xltypeMulti | xlbitDLLFree; // freed using xlAutoFree
	xlOper.val.array.columns = cols;
	xlOper.val.array.rows = rows;
	xlOper.val.array.lparray = new XLOPER[rows * cols];

	for (int r = 0; r < rows; ++r)
    {
		for (int c = 0; c < cols; ++c)
		{
			XLOPER& x = xlOper.val.array.lparray[r * cols + c];
			set(arg.at(r, c), x);
		}
    }
}
*/

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


void TPExcelInterfaceFn::copyFrom( const TPExcelInterfaceFn& sourceObj )
{
    // Do nothing
}

///////////////////////////////////////////////////////////////////////////////
// Operators:
const TPExcelInterfaceFn& TPExcelInterfaceFn::operator= ( const TPExcelInterfaceFn& RHS )
{
	if( this != &RHS ) 
    {
	    copyFrom( RHS );
	}

	return( *this );
}


///***************************************************************************
// TPExcelInterfaceFn::Excel()
//
// Purpose: 
//         A fancy wrapper for the Excel4() function. It also
//         does the following:
//        
//          (1) Checks that none of the LPXLOPER arguments are 0,
//              In this case, it doesn't call Excel
//              but it does print a debug message.
//          (2) If an error occurs while calling Excel,
//              print a useful debug message.
//        
//
// Parameters:
//
//      int xlfn            Function number (xl...) to call
//      LPXLOPER pxResult   Pointer to a place to stuff the result,
//                          or 0 if you don't care about the result.
//      int count           Number of arguments
//      ...                 (all LPXLOPERs) - the arguments.
//
// Returns: 
//
//      A return code (Some of the xlret... values, as defined
//      in XLCALL.H, OR'ed together).
//
//
//
// Comments:
//
//      The  count  variable must not be more than 30, see "Microsoft Excel Developer's Kit", page 224, ISBN 1-57231-498-2.
//
///***************************************************************************

// I repeat:
//    The  count  variable must not be more than 30, see "Microsoft Excel Developer's Kit", page 224, ISBN 1-57231-498-2.
int cdecl TPExcelInterfaceFn::Excel(int xlfn, LPXLOPER pxResult, int count, ...) {
    if (count>30){
        TPDebugPrintf("Error! A list of more than 30 parameters is being passed to Excel. Only 30 can be handled by Excel, see \"Microsoft Excel Developer's Kit\", ISBN 1-57231-498-2, page 224.\n");
        return xlretFailed;
    }
    int xlret;
    int i;
    LPXLOPER *ppx, *plpx;
#ifdef _ALPHA_
    // On the Alpha, arguments may be passed in via registers instead of being located in a contiguous block of memory,
    // so we must use the va_arg functions to retrieve them instead of simply walking through memory.
    va_list argList;
    LPXLOPER *plpx = alloca(count*sizeof(LPXLOPER));
    // TPtch all of the LPXLOPERS and copy them into plpx. plpx is alloca'ed and will automatically be freed when
    // the function exits (this is a special TPature of the alloca() function which allocates memory on the stack,
    // NOT on the heap. Use with care.
    va_start(argList, count);
    for(i=0; i<count; ++i)
        plpx[i] = va_arg(argList,LPXLOPER);
    va_end(argList);
#else
    plpx = (LPXLOPER *) (&count + 1);
#endif
    for (ppx=plpx, i=0; i<count; ++i, ++ppx) {
        if (!*ppx) {
            TPDebugPrintf("Error! A zero parameter is being passed to Excel. Check memory...\n");
            return xlretFailed;
        }
    }
   
    xlret = Excel4v(xlfn,pxResult,count,plpx);
    
#ifndef NDEBUG
    TPDebugPrintf("Excel4v(%lu,%lu,%lu,...) returned %lu.\n",(unsigned long)xlfn,(unsigned long)pxResult,(unsigned long)count,(unsigned long)xlret);
    if (xlret != xlretSuccess) {
        string errorMsg("Error! Excel4(");
        if (xlfn & xlCommand)
            errorMsg += "xlCommand | ";
        if (xlfn & xlSpecial)
            errorMsg += "xlSpecial | ";
        if (xlfn & xlIntl)
            errorMsg += "xlIntl | ";
        if (xlfn & xlPrompt)
            errorMsg += "xlPrompt | ";
        errorMsg += "%u) callback failed:";
        // More than one error bit may be on.
        if (xlret & xlretAbort)
            errorMsg += " Macro Halted\b";
        if (xlret & xlretInvXlfn)
            errorMsg += " Invalid Function Number";
        if (xlret & xlretInvCount)
            errorMsg += " Invalid Number of Arguments";
        if (xlret & xlretInvXloper)
            errorMsg += " Invalid XLOPER";
        if (xlret & xlretStackOvfl)
            errorMsg += " Stack Overflow";
        if (xlret & xlretFailed)
            errorMsg += " Command failed";
        if (xlret & xlretUncalced)
            errorMsg += " Uncalced cell";
        errorMsg += "\n";
        TPDebugPrintf(errorMsg.c_str(),xlfn & xltypeType);
    }
#endif /* ndef NDEBUG */
    
    return xlret;
}

typedef struct {
    BOOL bFuncWiz;
    short hwndXLMain;
} EnumStruct;

BOOL CALLBACK EnumProc(HWND hwnd, EnumStruct *pEnum){
    //
    // First check the class of the window.
    // Will be szXLDialogClass if function wizard dialog is up in Excel
    //
    if (  (LOWORD((DWORD) GetParent(hwnd)) == pEnum->hwndXLMain)  )
    {
        static const long CLASS_NAME_BUFTPR = 255;
        char rgsz[CLASS_NAME_BUFTPR];
        static const char *ClassString = "bosa_sdm_XL";
        static long l = lstrlen(ClassString);
        GetClassName(hwnd, (LPSTR)rgsz, CLASS_NAME_BUFTPR);
        if ( 2 == CompareString(MAKELCID(MAKELANGID(LANG_ENGLISH,SUBLANG_ENGLISH_US),SORT_DEFAULT),NORM_IGNORECASE, (LPSTR)rgsz, lstrlen((LPSTR)rgsz) > l ? l : -1,ClassString, -1) )
        {
            static const long WINDOW_NAME_BUFTPR = 256;
            char windowName[WINDOW_NAME_BUFTPR];
            GetWindowText(hwnd,windowName,WINDOW_NAME_BUFTPR-1);
#ifdef DEBUG_OUTPUT
            TPDebugPrintf("EnumProc callback called for window with title \"%s\" of class %s.\n",windowName,rgsz);
#endif
            if (windowName[0]=='\0'){
                //
                // The function wizard has no title bar unlike some of the other possible child windows of Excel
                // that are registered with class name "bosa_sdm_XL8" and could lead here (such as the Replace dialogue).
                //
                pEnum->bFuncWiz = TRUE;
                return FALSE;
            }
        }
    }
    // No luck - continue the enumeration.
    return TRUE;
}

long
TPExcelInterfaceFn::IAmCalledByFunctionWizard(void){
    XLOPER xHwndMain;
    EnumStruct enm;
    if (TPExcelInterfaceFn::Excel(xlGetHwnd, &xHwndMain,0) == xlretSuccess){
        enm.bFuncWiz = FALSE;
        enm.hwndXLMain = xHwndMain.val.w;
        EnumWindows((WNDENUMPROC) EnumProc, (LPARAM) &enm);
        return enm.bFuncWiz;
    }
    return FALSE;
}

long
TPExcelInterfaceFn::ExcelStatusBarMessage(const string &msg){
#ifndef NDEBUG
    TPDebugPrintf("%s\n",msg.c_str());
#endif
    TPExcelInterfaceFn::Excel(xlDisableXLMsgs, 0, 0);	//	Stop Excel chattering. Doesn't seem to work.
    long l = TPExcelInterfaceFn::Excel(xlcMessage, 0, 2, &XLOper(true), &XLOper(msg.c_str()));
    TPExcelInterfaceFn::Excel(xlEnableXLMsgs, 0, 0);
    return l;
}

long
TPExcelInterfaceFn::MessageWindow(const char *msg,const char *caption){
    return MessageBox(NULL,msg,caption,MB_OKCANCEL|MB_TASKMODAL|MB_SETFOREGROUND);
}

string
TPExcelInterfaceFn::callingCellId(void){
    //
    // This is messy but appears to work.
    //
    const char * funcId = "TPExcelInterfaceFn::CallingCellId()";
    XLOPER xlCaller, xRef, xlSheet;
    Excel(xlfCaller, &xlCaller, 0);
    if ( ! ( xlCaller.xltype & xltypeSRef ) ) return 0;	//	Cell could not be identified.
    Excel(xlSheetId, &xRef, 0);
    Excel(xlSheetNm, &xlSheet, 1, &xRef);
    string sheetName = TPExcelInterfaceFn::getString(&xlSheet,funcId);
    char Row[32], Col[32];
    sprintf(Row,"%u",xlCaller.val.sref.ref.rwFirst+1);
    sprintf(Col,"%u",xlCaller.val.sref.ref.colFirst+1);
    string address = "=ADDRESS(";
    address += Row;
    address += ",";
    address += Col;
    address += ")";
    XLOPER xlAddress;
    Excel(xlTPvaluate, &xlAddress, 1, &XLOper(address));
    string cellAddress = TPExcelInterfaceFn::getString(&xlAddress,funcId);
    string cellId = sheetName + "!" + cellAddress;
    Excel(xlFree, 0, 3, &xlCaller, &xRef, &xlSheet);
    return cellId;
}

LPXLOPER
TPExcelInterfaceFn::suppressedInFunctionWizard(void){
    return xlReturn("suppressed in wizard");
}

bool
TPExcelInterfaceFn::abortHasBeenRequested(void){
  XLOPER xRes;
  //
  // xlAbort:       ask Excel if an abort has been requested, i.e. if Escape has been pressed.
  // xRes:          storage space for the result.
  // 1:             one more argument
  // XLOper(false): Do not leave the abort request on the abort request stack.
  //
  if (Excel(xlAbort,&xRes,1,&XLOper(false)))
    return 0;	//	Failure to call into Excel -> don't try to read values out of xRes.

  return (xRes.xltype==xltypeBool && xRes.val.boolean);
}

#endif // #if defined(WIN32) || defined(_WINDOWS)
