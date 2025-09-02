#include "TPExcelInterface.h"
#include "TPTools/TPException.h"
//#include "TPTools/time/TPDate.h"
//#include "TPTools/TPAny.h"
#include "TPTools/TPGlobals.h"

#include "XLOper.h"

typedef struct _EnumStruct 
{
	unsigned short	wLoword;    // LoWord of Excel's main hWnd
   HWND  hwnd;                // Excel's main hWnd
   bool bInWizard;            // true if in function wizard
} EnumStruct;

//A simple wrapper for Excel4v calls

int TPExcelInterface::callExcel(int xlfunction,
					   LPXLOPER	xlerror,
					   int count,
					   ...)
{
	//LPXLOPER xloperArgsArray[30];
	int xlret	=	Excel4v(xlfunction,
					xlerror,
					count,
					(LPXLOPER *)(&count +1));
	//Excel(xlfunction, xlerror, count, ...);
		//xloperArgsArray);
	//
	//
	//comment the following line if u dont care
	//about the return result or would like to
	//handle the failure cases urself.

//	if (xlret != xlretSuccess) THROW("Error when calling Excel4v")


	return xlret;
	//return 1;
}

//TPSmartPointer<TPDate> TPExcelInterface::myReferenceDate = NULL;
//
//const TPDate& TPExcelInterface::getRefDate() 
//{
//  if(myReferenceDate.isNull())
//  {
//    THROW("Reference date has not been set up,")
//  }
//  return *myReferenceDate;
//}
//
//void TPExcelInterface::setRefDate(const TPDate& date) 
//{
//  if(myReferenceDate == NULL)
//  {
//    myReferenceDate = new TPDate();
//  }
//  *myReferenceDate = date;
//}


//
//
//checking out types here
// We DO NOT THROW here
//


bool	TPExcelInterface::isBool(LPXLOPER rhs)
{
	return ( ( rhs->xltype & 0x0FFF) == xltypeBool);
}

bool	TPExcelInterface::isLong(LPXLOPER rhs)
{
	return isDouble(rhs) && TPGlobals::isInt(toDouble(rhs, "TPExcelInterface::isLong: "));
}

bool	TPExcelInterface::isDouble(LPXLOPER rhs)
{
	return ( (rhs->xltype & 0x0FFF) == xltypeNum || (rhs->xltype & 0x0FFF) == xltypeInt );
}

bool	TPExcelInterface::isFloat(LPXLOPER rhs)
{ 
	return isDouble(rhs); 
}

bool	TPExcelInterface::isString(LPXLOPER rhs)
{
		return ( ( rhs->xltype & 0x0FFF) == xltypeStr);
}

bool	TPExcelInterface::isInvalid(LPXLOPER rhs)
{
	return 	( (rhs->xltype  & 0x0FFF) == xltypeNil )	|| 
			( (rhs->xltype & 0x0FFF) == xltypeMissing )  ||
			 ( (rhs->xltype & 0x0FFF) == xltypeErr )  ;
      
}

bool TPExcelInterface::isMissing( LPXLOPER rhs )
{
	return( ( (rhs->xltype & 0x0FFF) == xltypeMissing ) || (rhs->xltype  & 0x0FFF) == xltypeNil );
}

bool	TPExcelInterface::istypeMulti(LPXLOPER rhs)
{

	return ( ( rhs->xltype & 0x0FFF) == xltypeMulti);
}

//bool    TPExcelInterface::isDate(LPXLOPER rhs)
//{
//    return (isLong(rhs) && !TPDate(toLong(rhs, "TPExcelInterface::isDate(): ")).isInvalid());
//}


//
//
//"Coercing and returning the casted types"
//
//

bool TPExcelInterface::toBool(LPXLOPER xxloper, const std::string& argument, const bool default_value)
{
	if(!isBool(xxloper)) return default_value;
	return xxloper->val.xbool;
}

double TPExcelInterface::toDouble(LPXLOPER xxloper, const std::string& argument, const double default_value)
{
	if(!isDouble(xxloper)) return default_value;

	switch ( xxloper->xltype & 0x0FFF )
	{
        case xltypeNum:
            return( xxloper->val.num );
        case xltypeInt:
            return( xxloper->val.w );
        default:
            return 1;//we shouldnt get here, simply to avoid annoying warnings
     }

}

float TPExcelInterface::toFloat(LPXLOPER xxloper, const std::string& argument)
{ return (float) toDouble(xxloper, argument); }


long TPExcelInterface::toLong(LPXLOPER xxloper, const std::string& argument, const long& default_value)
{
	if(!isLong(xxloper)) return default_value;

	switch ( xxloper->xltype & 0x0FFF )
	{
        case xltypeNum:
            return( (long) xxloper->val.num );
        case xltypeInt:
            return( xxloper->val.w );
        default:
            return 1;//we shouldnt get here, simply to avoid annoying warnings
     }
}

int TPExcelInterface::toInt(LPXLOPER xxloper, const std::string& argument, const int default_value)
{
	if(!isLong(xxloper)) return default_value;
	return (int)(TPExcelInterface::toLong(xxloper,argument));

}

std::string	TPExcelInterface::toString(LPXLOPER xxloper, const std::string& argument, const std::string& default_value)
{
	if(!isString(xxloper)) return default_value;
	return std::string ( &xxloper->val.str[1], xxloper->val.str[0]);
}

std::optional<std::string> TPExcelInterface::toOptionalString(LPXLOPER xxloper, const std::string &argument)
{
	if (isMissing(xxloper))
		return std::optional<std::string>();
	else
		return toString(xxloper, argument);
}

//TPDate  TPExcelInterface::toDate(LPXLOPER xxloper, const std::string& argument)
//{
//
//        if(!isDate(xxloper)) THROW(argument + " Error Casting to Date")
//
//        switch ( xxloper->xltype & 0x0FFF )
//        {
//        case xltypeNum:
//            return      TPDate( (long) xxloper->val.num );
//        case xltypeInt:
//            return      TPDate( (long) xxloper->val.w );
//        default:
//			return TPDate(); //we shouldnt get here, simply to avoid annoying warnings
//     }
//}
//
//TPAny   TPExcelInterface::toAny(LPXLOPER lppxloper, const std::string& argument)
//{
//	if (isDouble(lppxloper)) return TPAny(toDouble(lppxloper, argument));
//	if (isBool(lppxloper)) return TPAny(toBool(lppxloper, argument));
//	if (isString(lppxloper)) return TPAny(toString(lppxloper, argument));
//
//	return TPAny();
//}


//
//cast to array types
std::vector<double> TPExcelInterface::toDoubleArray(LPXLOPER xxloper, const std::string& argument)
{
	
	//handle the case of a pseudo array

	if (!istypeMulti(xxloper))
	{
		//if its of the right type then we'll 
		//make a std::vector and return it back
		

		if (xxloper->xltype == xltypeNum) 
			return std::vector<double>(1,toDouble(xxloper,argument));
		else THROW(argument+" a non double array member");
	}


	//we have an array at this stage, get number of elements

	int tmparraysize, arraysize;
	if (xxloper->val.array.columns ==1) tmparraysize = xxloper->val.array.rows;
	else tmparraysize = xxloper->val.array.columns;

	arraysize = tmparraysize -1;

	//trim off empty cells and error cells
	//TODO: we might want to throw somewhere here.
	//The convention adopted is that an array
	//must at least have its first elements valid
	//so that we can trim off after the last valid 
	//element.

	while (xxloper->val.array.lparray[ arraysize].xltype & (xltypeNil | xltypeErr | xltypeMissing))
	{
		arraysize--;
		if (arraysize == 0) break;
	}


	
	//our return variable

	std::vector<double> return_vect;

	//loop through the array elements
	//and push them into the std::vector.
	//blank cells are mapped to 0.0

	for (int i =0; i<=arraysize;i++)
	{
		if (!(xxloper->val.array.lparray[ i].xltype &  xltypeErr))
		{ 
			if ((xxloper->val.array.lparray[ i].xltype & xltypeNil)
				|| (xxloper->val.array.lparray[ i].xltype & xltypeMissing))
				return_vect.push_back((double)0.0);
			else return_vect.push_back(toDouble(&(xxloper->val.array.lparray[ i]), argument));
		}
		else THROW(argument+" a non double array member");
	}



	
	return return_vect;


}

std::vector<double> TPExcelInterface::toDoubleArrayNoErrs(LPXLOPER xxloper, const std::string& argument)
{
	
	//handle the case of a pseudo array
	if (!istypeMulti(xxloper))
	{
		//if its of the right type then we'll 
		//make a std::vector and return it back
		if (xxloper->xltype == xltypeNum) 
		{
			return std::vector<double>(1,toDouble(xxloper,argument));
		}
		else if (xxloper->xltype &  (xltypeNil | xltypeErr | xltypeMissing))
		{
			THROW(argument+" a non double array member");
		}
	}


	//we have an array at this stage, get number of elements

	int tmparraysize, arraysize;
	if (xxloper->val.array.columns ==1) tmparraysize = xxloper->val.array.rows;
	else tmparraysize = xxloper->val.array.columns;

	arraysize = tmparraysize -1;
	
	//our return variable
	std::vector<double> return_vect;
	int counter = 0;

	//loop through the array elements and push them into the std::vector.
	//blank cells, errors and missing are ignored
	for (int i =0; i<=arraysize;i++)
	{
		if (!(xxloper->val.array.lparray[i].xltype &  xltypeErr))
		{ 
			if (!(xxloper->val.array.lparray[i].xltype & xltypeNil)
				&& !(xxloper->val.array.lparray[i].xltype & xltypeMissing))
			{
				try
				{
					return_vect.push_back(toDouble(&(xxloper->val.array.lparray[i]), argument));
					counter++;
				}
				catch(...)
				{
					
				}
			}
		}
	}
	
	if (counter >= 1)
	{
		return return_vect;
	}
	else
	{
		THROW(argument+" a non double array member");
	}
}

std::vector<double> TPExcelInterface::toDoubleArrayNoErrs(LPXLOPER xxloper, const std::string& argument, const double repl)
{
	
	//handle the case of a pseudo array
	if (!istypeMulti(xxloper))
	{
		//if its of the right type then we'll 
		//make a std::vector and return it back
		if (xxloper->xltype == xltypeNum) 
		{
			return std::vector<double>(1,toDouble(xxloper,argument));
		}
		else if (xxloper->xltype &  (xltypeNil | xltypeErr | xltypeMissing))
		{
			return std::vector<double>(1,repl);
		}
		else THROW(argument+" a non double array member");
	}


	//we have an array at this stage, get number of elements

	int tmparraysize, arraysize;
	if (xxloper->val.array.columns ==1) tmparraysize = xxloper->val.array.rows;
	else tmparraysize = xxloper->val.array.columns;

	arraysize = tmparraysize -1;
	
	//our return variable
	std::vector<double> return_vect;

	//loop through the array elements and push them into the std::vector.
	//blank cells, errors and missing are mapped to repl
	for (int i =0; i<=arraysize;i++)
	{
		if (!(xxloper->val.array.lparray[i].xltype &  xltypeErr))
		{ 
			if ((xxloper->val.array.lparray[i].xltype & xltypeNil)
				|| (xxloper->val.array.lparray[i].xltype & xltypeMissing))
			{
				return_vect.push_back(repl);
			}
			else
			{
				try
				{
					return_vect.push_back(toDouble(&(xxloper->val.array.lparray[i]), argument, repl));
				}
				catch(...)
				{
					return_vect.push_back(repl);
				}
				
			}
		}
		else
		{
			return_vect.push_back(repl);
		}
	}

	return return_vect;
}

std::vector<std::string> TPExcelInterface::toStringArray(LPXLOPER xxloper, const std::string& argument)
{
	
	//handle the case of a pseudo array

	if (!istypeMulti(xxloper))
	{
		//if its of the right type then we'll 
		//make a std::vector and return it back
		

		if (isString(xxloper)) 
			return std::vector<std::string>(1,toString(xxloper,argument));
		else THROW(argument+" a non std::string array member");
	}


	//we have an array at this stage, get number of elements

	int tmparraysize, arraysize;
	if (xxloper->val.array.columns ==1) tmparraysize = xxloper->val.array.rows;
	else tmparraysize = xxloper->val.array.columns;

	arraysize = tmparraysize -1;

	//trim off empty cells and error cells
	//TODO: we might want to throw somewhere here.
	//The convention adopted is that an array
	//must at least have its first elements valid
	//so that we can trim off after the last valid 
	//element.

	while (xxloper->val.array.lparray[ arraysize].xltype & (xltypeNil | xltypeErr))
	{
		arraysize--;
		if (arraysize == 0) break;
	}

	//our return variable

	std::vector<std::string> return_vect;

	//loop through the array elements
	//and push them into the std::vector.
	//blank cells are mapped to ""

	for (int i =0; i<=arraysize;i++)
	{
		if (!(xxloper->val.array.lparray[ i].xltype  & xltypeErr))
		{ 
			if (xxloper->val.array.lparray[ i].xltype & xltypeNil)
				return_vect.push_back("");
			else return_vect.push_back(toString(&(xxloper->val.array.lparray[i]), argument));
		}
		else THROW(argument+" a non std::string array member");
	}


	return return_vect;


}

std::vector<std::string> TPExcelInterface::toStringArrayNoErrs(LPXLOPER xxloper, const std::string& argument)
{
	
	//handle the case of a pseudo array
	if (!istypeMulti(xxloper))
	{
		//if its of the right type then we'll 
		//make a std::vector and return it back
		if (xxloper->xltype == xltypeStr) 
		{
			return std::vector<std::string>(1,toString(xxloper,argument));
		}
		else if (xxloper->xltype &  (xltypeNil | xltypeErr | xltypeMissing))
		{
			THROW(argument+" a non std::string array member");
		}
	}


	//we have an array at this stage, get number of elements

	int tmparraysize, arraysize;
	if (xxloper->val.array.columns ==1) tmparraysize = xxloper->val.array.rows;
	else tmparraysize = xxloper->val.array.columns;

	arraysize = tmparraysize -1;
	
	//our return variable
	std::vector<std::string> return_vect;
	int counter = 0;

	//loop through the array elements and push them into the std::vector.
	//blank cells, errors and missing are ignored
	for (int i =0; i<=arraysize;i++)
	{
		if (!(xxloper->val.array.lparray[i].xltype &  xltypeErr))
		{ 
			if (!(xxloper->val.array.lparray[i].xltype & xltypeNil)
				&& !(xxloper->val.array.lparray[i].xltype & xltypeMissing))
			{
				try
				{
					return_vect.push_back(toString(&(xxloper->val.array.lparray[i]), argument));
					counter++;
				}
				catch(...)
				{
					
				}
			}
		}
	}
	
	if (counter >= 1)
	{
		return return_vect;
	}
	else
	{
		THROW(argument+" a non std::string array member");
	}
}

std::vector<std::string> TPExcelInterface::toStringArrayNoErrs(LPXLOPER xxloper, const std::string& argument, const std::string& repl)
{
	
	//handle the case of a pseudo array
	if (!istypeMulti(xxloper))
	{
		//if its of the right type then we'll 
		//make a std::vector and return it back
		if (xxloper->xltype == xltypeStr) 
		{
			return std::vector<std::string>(1,toString(xxloper,argument));
		}
		else if (xxloper->xltype &  (xltypeNil | xltypeErr | xltypeMissing))
		{
			return std::vector<std::string>(1,repl);
		}
	}


	//we have an array at this stage, get number of elements
	int tmparraysize, arraysize;
	if (xxloper->val.array.columns ==1) tmparraysize = xxloper->val.array.rows;
	else tmparraysize = xxloper->val.array.columns;

	arraysize = tmparraysize -1;
	
	//our return variable
	std::vector<std::string> return_vect;

	//loop through the array elements and push them into the std::vector.
	//blank cells, errors and missing are ignored
	for (int i =0; i<=arraysize;i++)
	{
		if (!(xxloper->val.array.lparray[i].xltype &  xltypeErr))
		{ 
			if ((xxloper->val.array.lparray[i].xltype & xltypeNil)
				|| (xxloper->val.array.lparray[i].xltype & xltypeMissing))
			{
				return_vect.push_back(repl);
			}
			else
			{
				try
				{
					return_vect.push_back(toString(&(xxloper->val.array.lparray[i]), argument));
				}
				catch(...)
				{
					return_vect.push_back(repl);
				}
				
			}
		}
		else
		{
			return_vect.push_back(repl);
		}
	}

	return return_vect;
}


//std::vector<TPDate> TPExcelInterface::toDateArray(LPXLOPER xxloper, const std::string& argument)
//{
//	
//	//handle the case of a pseudo array
//	if (!istypeMulti(xxloper))
//	{
//		//if its of the right type then we'll 
//		//make a std::vector and return it back
//		
//
//		if (isDate(xxloper)) 
//			return std::vector<TPDate>(1,toDate(xxloper,argument));
//		else THROW(argument+" a non Date array member");
//	}
//
//
//	//we have an array at this stage, get number of elements
//	int tmparraysize, arraysize;
//	if (xxloper->val.array.columns ==1) tmparraysize = xxloper->val.array.rows;
//	else tmparraysize = xxloper->val.array.columns;
//
//	arraysize = tmparraysize -1;
//
//	//trim off empty cells and error cells
//	//TODO: we might want to throw somewhere here.
//	//The convention adopted is that an array
//	//must at least have its first elements valid
//	//so that we can trim off after the last valid 
//	//element.
//
//	while (xxloper->val.array.lparray[ arraysize].xltype & (xltypeNil | xltypeErr))
//	{
//		arraysize--;
//		if (arraysize == 0) break;
//	}
//
//	//our return variable
//
//	std::vector<TPDate> return_vect;
//
//	//loop through the array elements
//	//and push them into the std::vector.
//	//Blank cells are mapped to Invalid Date
//	//TODO am not sure that the above is the
//	//best idea around. We might want to throw
//	//instead. Coz an Invalid Date will cause
//	//something to throw sooner or later.
//
//	for (int i =0; i<=arraysize;i++)
//	{
//		if (!(xxloper->val.array.lparray[ i].xltype  & xltypeErr))
//		{ 
//			if (xxloper->val.array.lparray[ i].xltype & xltypeNil)
//				return_vect.push_back(TPDate::getInvalidDate());
//			else return_vect.push_back(toDate(&(xxloper->val.array.lparray[i]), argument));
//		}
//		else THROW(argument+" a non Date array member");
//	}
//
//	return return_vect;
//}

//std::vector<TPDate> TPExcelInterface::toDateArrayNoErrs(LPXLOPER xxloper, const std::string& argument)
//{
//	
//	//handle the case of a pseudo array
//	if (!istypeMulti(xxloper))
//	{
//		//if its of the right type then we'll 
//		//make a std::vector and return it back
//		if (isDate(xxloper)) 
//		{
//			return std::vector<TPDate>(1,toDate(xxloper,argument));
//		}
//		else if (xxloper->xltype &  (xltypeNil | xltypeErr | xltypeMissing))
//		{
//			THROW(argument+" a non Date array member");
//		}
//	}
//
//
//	//we have an array at this stage, get number of elements
//
//	int tmparraysize, arraysize;
//	if (xxloper->val.array.columns ==1) tmparraysize = xxloper->val.array.rows;
//	else tmparraysize = xxloper->val.array.columns;
//
//	arraysize = tmparraysize -1;
//	
//	//our return variable
//	std::vector<TPDate> return_vect;
//	int counter = 0;
//
//	//loop through the array elements and push them into the std::vector.
//	//blank cells, errors and missing are ignored
//	for (int i =0; i<=arraysize;i++)
//	{
//		if (!(xxloper->val.array.lparray[i].xltype &  xltypeErr))
//		{ 
//			if (!(xxloper->val.array.lparray[i].xltype & xltypeNil)
//				&& !(xxloper->val.array.lparray[i].xltype & xltypeMissing))
//			{
//				try
//				{
//					return_vect.push_back(toDate(&(xxloper->val.array.lparray[i]), argument));
//					counter++;
//				}
//				catch(...)
//				{
//					
//				}
//			}
//		}
//	}
//	
//	if (counter >= 1)
//	{
//		return return_vect;
//	}
//	else
//	{
//		THROW(argument+" a non Date array member");
//	}
//}

//std::vector<TPDate> TPExcelInterface::toDateArrayNoErrs(LPXLOPER xxloper, const std::string& argument, const TPDate& repl)
//{
//	
//	//handle the case of a pseudo array
//	if (!istypeMulti(xxloper))
//	{
//		//if its of the right type then we'll 
//		//make a std::vector and return it back
//		if (isDate(xxloper)) 
//		{
//			return std::vector<TPDate>(1,toDate(xxloper,argument));
//		}
//		else if (xxloper->xltype &  (xltypeNil | xltypeErr | xltypeMissing))
//		{
//			return std::vector<TPDate>(1,repl);
//		}
//	}
//
//
//	//we have an array at this stage, get number of elements
//
//	int tmparraysize, arraysize;
//	if (xxloper->val.array.columns ==1) tmparraysize = xxloper->val.array.rows;
//	else tmparraysize = xxloper->val.array.columns;
//
//	arraysize = tmparraysize -1;
//	
//	//our return variable
//	std::vector<TPDate> return_vect;
//
//	//loop through the array elements and push them into the std::vector.
//	//blank cells, errors and missing are ignored
//	for (int i =0; i<=arraysize;i++)
//	{
//		if (!(xxloper->val.array.lparray[i].xltype &  xltypeErr))
//		{ 
//			if ((xxloper->val.array.lparray[i].xltype & xltypeNil)
//				|| (xxloper->val.array.lparray[i].xltype & xltypeMissing))
//			{
//				return_vect.push_back(repl);
//			}
//			else
//			{
//				try
//				{
//					return_vect.push_back(toDate(&(xxloper->val.array.lparray[i]), argument));
//				}
//				catch(...)
//				{
//					return_vect.push_back(repl);
//				}
//				
//			}
//		}
//		else
//		{
//			return_vect.push_back(repl);
//		}
//	}
//
//	return return_vect;
//}


//std::vector<TPAny> TPExcelInterface::toAnyArray(LPXLOPER xxloper, const std::string& argument)
//{
//	
//	//handle the case of a pseudo array
//
//	if (!istypeMulti(xxloper))
//	{
//		//if its of the right type then we'll 
//		//make a std::vector and return it back
//		
//		return std::vector<TPAny>(1,toAny(xxloper,argument));
//
//	}
//
//
//	//we have an array at this stage, get number of elements
//
//	int tmparraysize, arraysize;
//	if (xxloper->val.array.columns ==1) tmparraysize = xxloper->val.array.rows;
//	else tmparraysize = xxloper->val.array.columns;
//
//	arraysize = tmparraysize -1;
//
//	//trim off empty cells and error cells
//	//TODO: we might want to throw somewhere here.
//	//The convention adopted is that an array
//	//must at least have its first elements valid
//	//so that we can trim off after the last valid 
//	//element.
//
//	while (xxloper->val.array.lparray[ arraysize].xltype & (xltypeNil | xltypeErr))
//	{
//		arraysize--;
//		if (arraysize == 0) break;
//	}
//
//	//our return variable
//
//	std::vector<TPAny> return_vect;
//
//	//loop through the array elements
//	//and push them into the std::vector.
//	//Blank cells are mapped to Invalid Date
//	//TODO am not sure that the above is the
//	//best idea around. We might want to throw
//	//instead. Coz an Invalid Date will cause
//	//something to throw sooner or later.
//
//	for (int i =0; i<=arraysize;i++)
//	{
//		if (!(xxloper->val.array.lparray[ i].xltype  & xltypeErr))
//		{ 
//			if (xxloper->val.array.lparray[ i].xltype & xltypeNil)
//				return_vect.push_back(TPAny());
//			else return_vect.push_back(toAny(&(xxloper->val.array.lparray[ i]), argument));
//		}
//		else THROW(argument+" a non Any array member");
//	}
//
//	return return_vect;
//}

//std::vector<TPAny> TPExcelInterface::toAnyArrayNoErrs(LPXLOPER xxloper, const std::string& argument)
//{
//	
//	//handle the case of a pseudo array
//	if (!istypeMulti(xxloper))
//	{
//		//if its of the right type then we'll 
//		//make a std::vector and return it back
//		if (!isInvalid(xxloper))
//		{
//			return std::vector<TPAny>(1,toAny(xxloper,argument));
//		}
//		else
//		{
//			THROW(argument+" a non Any array member");
//		}
//	}
//
//
//	//we have an array at this stage, get number of elements
//
//	int tmparraysize, arraysize;
//	if (xxloper->val.array.columns ==1) tmparraysize = xxloper->val.array.rows;
//	else tmparraysize = xxloper->val.array.columns;
//
//	arraysize = tmparraysize -1;
//	
//	//our return variable
//	std::vector<TPAny> return_vect;
//	int counter = 0;
//
//	//loop through the array elements and push them into the std::vector.
//	//blank cells, errors and missing are ignored
//	for (int i =0; i<=arraysize;i++)
//	{
//		if (!(xxloper->val.array.lparray[i].xltype &  xltypeErr))
//		{ 
//			if (!(xxloper->val.array.lparray[i].xltype & xltypeNil)
//				&& !(xxloper->val.array.lparray[i].xltype & xltypeMissing))
//			{
//				try
//				{
//					return_vect.push_back(toAny(&(xxloper->val.array.lparray[ i]), argument));
//					counter++;
//				}
//				catch(...)
//				{
//					
//				}
//			}
//		}
//	}
//	
//	if (counter >= 1)
//	{
//		return return_vect;
//	}
//	else
//	{
//		THROW(argument+" a non Any array member");
//	}
//}

//std::vector<TPAny> TPExcelInterface::toAnyArrayNoErrs(LPXLOPER xxloper, const std::string& argument, const TPAny& repl)
//{
//	
//	//handle the case of a pseudo array
//	if (!istypeMulti(xxloper))
//	{
//		//if its of the right type then we'll 
//		//make a std::vector and return it back
//		if (!isInvalid(xxloper))
//		{
//			return std::vector<TPAny>(1,toAny(xxloper,argument));
//		}
//		else
//		{
//			return std::vector<TPAny>(1,repl);
//		}
//	}
//
//
//	//we have an array at this stage, get number of elements
//
//	int tmparraysize, arraysize;
//	if (xxloper->val.array.columns ==1) tmparraysize = xxloper->val.array.rows;
//	else tmparraysize = xxloper->val.array.columns;
//
//	arraysize = tmparraysize -1;
//	
//	//our return variable
//	std::vector<TPAny> return_vect;
//
//	//loop through the array elements and push them into the std::vector.
//	//blank cells, errors and missing are ignored
//	for (int i =0; i<=arraysize;i++)
//	{
//		if (!(xxloper->val.array.lparray[i].xltype &  xltypeErr))
//		{ 
//			if ((xxloper->val.array.lparray[i].xltype & xltypeNil)
//				|| (xxloper->val.array.lparray[i].xltype & xltypeMissing))
//			{
//				return_vect.push_back(repl);
//			}
//			else
//			{
//				try
//				{
//					return_vect.push_back(toAny(&(xxloper->val.array.lparray[ i]), argument));
//				}
//				catch(...)
//				{
//					return_vect.push_back(repl);
//				}
//				
//			}
//		}
//		else
//		{
//			return_vect.push_back(repl);
//		}
//	}
//
//	return return_vect;
//}

TPMatrix<double> TPExcelInterface::toDoubleMatrix(LPXLOPER xxloper, const std::string& argument)
{
	// handle the case of a pseudo array
	if (!istypeMulti(xxloper))
	{
		// if its of the right type then we'll make a std::vector and return it back
		if (isDouble(xxloper))
		{
			TPMatrix<double> return_vect(1,1,toDouble(xxloper,argument));
			return return_vect;
		}
		else
		{
			THROW(argument+" a non Double Matrix member");
		}
	}


	int rows = xxloper->val.array.rows, columns = xxloper->val.array.columns, l,k;

	if ((rows == 0) || (columns == 0)) THROW(argument+" Matrix should not be a std::vector")

	TPMatrix<double> return_vect;

	int true_nb_columns = 0;
	l=0;
  
	return_vect.push_back(std::vector<double>(),0);
	
	for (k=0;k<columns;k++)
	{
		if (!(xxloper->val.array.lparray[ l*columns+k].xltype  & xltypeErr))
		{ 
			if (xxloper->val.array.lparray[l*columns+k].xltype & xltypeNil)
			{
  				break;
			}
			else 
			{
				double spy = toDouble(&(xxloper->val.array.lparray[ l*columns+k]), argument);
				return_vect[l].push_back(spy);
				++true_nb_columns;
			}
		}
		else
		{
			THROW(argument+" a non Double matrix member");
		}
	}

	for(l=1;l<rows;l++)
	{
		for (k=0;k<true_nb_columns;k++)
		{
			if (!(xxloper->val.array.lparray[ l*columns+k].xltype  & xltypeErr))
			{ 
				if (xxloper->val.array.lparray[l*columns+k].xltype & xltypeNil)
				{
  					break;
				}
				else 
				{
					if(k==0)
					{
						return_vect.push_back(std::vector<double>(true_nb_columns),0);
					}
					
					return_vect[l][k] = toDouble(&(xxloper->val.array.lparray[ l*columns+k]), argument);
				}
			}
			else
			{
				THROW(argument+" a non Double matrix member");
			}
		}
	}

	return return_vect;
}

TPMatrix<double> TPExcelInterface::toDoubleMatrixNoErrs(LPXLOPER xxloper, const std::string& argument)
{
	// handle the case of a pseudo array
	if (!istypeMulti(xxloper))
	{
		// if its of the right type then we'll make a std::vector and return it back
		if (xxloper->xltype == xltypeNum) 
		{
			TPMatrix<double> return_vect(1,1,toDouble(xxloper,argument));
			return return_vect;
		}
		else if (xxloper->xltype &  (xltypeNil | xltypeErr | xltypeMissing))
		{
			THROW(argument+" a non double matrix member");
		}
	}


	int rows = xxloper->val.array.rows, columns = xxloper->val.array.columns, l,k;

	if ((rows == 0) || (columns == 0)) THROW(argument+" Matrix should not be a std::vector")

	TPMatrix<double> return_vect;

	int true_nb_columns = 0;
	l=0;
  
	return_vect.push_back(std::vector<double>(),0);
	
	for (k=0;k<columns;k++)
	{
		if (!(xxloper->val.array.lparray[l*columns+k].xltype  & xltypeErr))
		{ 
			if (!(xxloper->val.array.lparray[l*columns+k].xltype & xltypeNil)
					&& !(xxloper->val.array.lparray[l*columns+k].xltype & xltypeMissing))
			{
				try
				{
					double spy = toDouble(&(xxloper->val.array.lparray[l*columns+k]), argument);
					return_vect[l].push_back(spy);
					++true_nb_columns;
				}
				catch(...)
				{

				}
			}
		}
	}

	for(l=1;l<rows;l++)
	{
		for (k=0;k<true_nb_columns;k++)
		{
			if (!(xxloper->val.array.lparray[l*columns+k].xltype  & xltypeErr))
			{ 
				if (!(xxloper->val.array.lparray[l*columns+k].xltype & xltypeNil)
					&& !(xxloper->val.array.lparray[l*columns+k].xltype & xltypeMissing))
				{
					try
					{
						if(k==0)
						{
							return_vect.push_back(std::vector<double>(true_nb_columns),0);
						}
					
						return_vect[l][k] = toDouble(&(xxloper->val.array.lparray[l*columns+k]), argument);
					}
					catch(...)
					{
						
					}
				}
			}
		}
	}

	return return_vect;
}

TPMatrix<double> TPExcelInterface::toDoubleMatrixNoErrs(LPXLOPER xxloper, const std::string& argument, const double repl)
{
	// handle the case of a pseudo array
	if (!istypeMulti(xxloper))
	{
		// if its of the right type then we'll make a std::vector and return it back
		if (xxloper->xltype == xltypeNum) 
		{
			TPMatrix<double> return_vect(1,1,toDouble(xxloper,argument));
			return return_vect;
		}
		else if (xxloper->xltype &  (xltypeNil | xltypeErr | xltypeMissing))
		{
			TPMatrix<double> return_vect(1,1,repl);
			return return_vect;
		}
		else
		{
			THROW(argument+" a non double matrix member");
		}
	}

	int rows = xxloper->val.array.rows, columns = xxloper->val.array.columns, l,k;

	if ((rows == 0) || (columns == 0)) THROW(argument+" Matrix should not be a std::vector")

	TPMatrix<double> return_vect;

	int true_nb_columns = 0;
	l=0;
  
	return_vect.push_back(std::vector<double>(),0);
	
	for (k=0;k<columns;k++)
	{
		if (!(xxloper->val.array.lparray[l*columns+k].xltype  & xltypeErr))
		{ 
			if ((xxloper->val.array.lparray[l*columns+k].xltype & xltypeNil)
				|| (xxloper->val.array.lparray[l*columns+k].xltype & xltypeMissing))
			{
				return_vect[l].push_back(repl);
			}
			else
			{
				try
				{
					double spy = toDouble(&(xxloper->val.array.lparray[l*columns+k]), argument);
					return_vect[l].push_back(spy);
					++true_nb_columns;
				}
				catch(...)
				{

				}
			}
		}
	}

	for(l=1;l<rows;l++)
	{
		for (k=0;k<true_nb_columns;k++)
		{
			if (!(xxloper->val.array.lparray[l*columns+k].xltype  & xltypeErr))
			{ 
				if ((xxloper->val.array.lparray[l*columns+k].xltype & xltypeNil)
					|| (xxloper->val.array.lparray[l*columns+k].xltype & xltypeMissing))
				{
					if(k==0)
					{
						return_vect.push_back(std::vector<double>(true_nb_columns),repl);
					}
					
					return_vect[l][k] = repl;
				}
				else
				{
					try
					{
						if(k==0)
						{
							return_vect.push_back(std::vector<double>(true_nb_columns),0);
						}
					
						return_vect[l][k] = toDouble(&(xxloper->val.array.lparray[l*columns+k]), argument);
					}
					catch(...)
					{
						
					}
				}
			}
			else
			{
				if(k==0)
				{
					return_vect.push_back(std::vector<double>(true_nb_columns),repl);
				}
				
				return_vect[l][k] = repl;
			}
		}
	}

	return return_vect;
}

TPMatrix<std::string> TPExcelInterface::toStringMatrix(LPXLOPER xxloper, const std::string& argument)
{
	//handle the case of a pseudo array
	if (!istypeMulti(xxloper))
	{
		// if its of the right type then we'll make a std::vector and return it back
		if (isString(xxloper))
		{
			TPMatrix<std::string> return_vect(1,1,toString(xxloper,argument));
			return return_vect;
		}
		else
		{
			THROW(argument+" a non std::string Matrix member");
		}
	}


	int rows = xxloper->val.array.rows, columns = xxloper->val.array.columns, l,k;

	if ((rows == 0) || (columns == 0)) THROW(argument+" Matrix should not be a std::vector")

	TPMatrix<std::string> return_vect(rows, columns);


	for(l=0;l<rows;l++)
	{
		for (k=0;k<columns;k++)
		{
			if (!(xxloper->val.array.lparray[ l*columns+k].xltype  & xltypeErr))
			{ 
				if (xxloper->val.array.lparray[l*columns+k].xltype & xltypeNil)
				return_vect[l][k] = std::string();
				else return_vect[l][k] = toString(&(xxloper->val.array.lparray[ l*columns+k]), argument);
			}
			else
			{
				THROW(argument+" a non std::string matrix member");
			}
		}
	}

	return return_vect;
}

TPMatrix<std::string> TPExcelInterface::toStringMatrixNoErrs(LPXLOPER xxloper, const std::string& argument)
{
	//handle the case of a pseudo array
	if (!istypeMulti(xxloper))
	{
		// if its of the right type then we'll make a std::vector and return it back
		if (isString(xxloper))
		{
			TPMatrix<std::string> return_vect(1,1,toString(xxloper,argument));
			return return_vect;
		}
		else if (xxloper->xltype &  (xltypeNil | xltypeErr | xltypeMissing))
		{
			THROW(argument+" a non std::string Matrix member");
		}
	}


	int rows = xxloper->val.array.rows, columns = xxloper->val.array.columns, l,k;

	if ((rows == 0) || (columns == 0)) THROW(argument+" Matrix should not be a std::vector")

	TPMatrix<std::string> return_vect(rows, columns);


	for(l=0;l<rows;l++)
	{
		for (k=0;k<columns;k++)
		{
			if (!(xxloper->val.array.lparray[l*columns+k].xltype  & xltypeErr))
			{
				if (!(xxloper->val.array.lparray[l*columns+k].xltype & xltypeNil)
					&& !(xxloper->val.array.lparray[l*columns+k].xltype & xltypeMissing))
				{
					try
					{
						return_vect[l][k] = toString(&(xxloper->val.array.lparray[l*columns+k]), argument);
					}
					catch(...)
					{

					}
				}
			}
		}
	}

	return return_vect;
}

TPMatrix<std::string> TPExcelInterface::toStringMatrixNoErrs(LPXLOPER xxloper, const std::string& argument, const std::string& repl)
{
	//handle the case of a pseudo array
	if (!istypeMulti(xxloper))
	{
		// if its of the right type then we'll make a std::vector and return it back
		if (isString(xxloper))
		{
			TPMatrix<std::string> return_vect(1,1,toString(xxloper,argument));
			return return_vect;
		}
		else if (xxloper->xltype &  (xltypeNil | xltypeErr | xltypeMissing))
		{
			TPMatrix<std::string> return_vect(1,1,repl);
			return return_vect;
		}
		else
		{
			THROW(argument+" a non std::string Matrix member");
		}
	}


	int rows = xxloper->val.array.rows, columns = xxloper->val.array.columns, l,k;

	if ((rows == 0) || (columns == 0)) THROW(argument+" Matrix should not be a std::vector")

	TPMatrix<std::string> return_vect(rows, columns);


	for(l=0;l<rows;l++)
	{
		for (k=0;k<columns;k++)
		{
			if (!(xxloper->val.array.lparray[l*columns+k].xltype  & xltypeErr))
			{
				if ((xxloper->val.array.lparray[l*columns+k].xltype & xltypeNil)
					|| (xxloper->val.array.lparray[l*columns+k].xltype & xltypeMissing))
				{
					return_vect[l][k] = repl;
				}
				else
				{
					try
					{
						return_vect[l][k] = toString(&(xxloper->val.array.lparray[l*columns+k]), argument);
					}
					catch(...)
					{

					}
				}
			}
			else
			{
				return_vect[l][k] = repl;				
			}
		}
	}

	return return_vect;
}

//TPMatrix<TPAny> TPExcelInterface::toAnyMatrix(LPXLOPER xxloper, const std::string& argument)
//{
//	// handle the case of a pseudo array
//	if (!istypeMulti(xxloper))
//	{
//		// if its of the right type then we'll make a std::vector and return it back
//		TPMatrix<TPAny> return_vect(1,1,toAny(xxloper,argument));
//		return return_vect;
//	}
//
//	int rows = xxloper->val.array.rows, columns = xxloper->val.array.columns, l,k;
//
//	if ((rows == 0) || (columns == 0)) THROW(argument+" Matrix should not be a std::vector")
//
//	TPMatrix<TPAny> return_vect(rows, columns);
//
//	for(l=0;l<rows;l++)
//	{
//		for (k=0;k<columns;k++)
//		{
//			if (!(xxloper->val.array.lparray[ l*columns+k].xltype  & xltypeErr))
//			{ 
//				 return_vect[l][k] = toAny(&(xxloper->val.array.lparray[ l*columns+k]), argument);
//			}
//			else 
//			{
//				return_vect[l][k] = TPAny();
//			}
//		}
//	}
//
//	return return_vect;
//}

std::vector<std::pair<double, double>>	TPExcelInterface::toPairArray(LPXLOPER xxloper, const std::string &argument)
{
	// handle the case of a pseudo array
	if (!istypeMulti(xxloper))
	{
		THROW("It's not a pair or vector of pairs")
	}
	int rows = xxloper->val.array.rows, columns = xxloper->val.array.columns, l;
	TP_REQUIRE(columns == 2, "Expecting 2 columns only in a vector of pairs");
	std::vector<std::pair<double, double>> res;
	for (l = 0; l < rows; l++)
	{
		double v1, v2;
		v1 = toDouble(&(xxloper->val.array.lparray[l*columns + 0]), argument);
		v2 = toDouble(&(xxloper->val.array.lparray[l*columns + 1]), argument);
		res.emplace_back(std::make_pair(v1,v2));
	}
	return res;
}

//TPMatrix<TPAny> TPExcelInterface::toAnyMatrixNoErrs(LPXLOPER xxloper, const std::string& argument)
//{
//	// handle the case of a pseudo array
//	if (!istypeMulti(xxloper))
//	{
//		// if its of the right type then we'll make a std::vector and return it back
//		if (xxloper->xltype &  (xltypeNil | xltypeErr | xltypeMissing))
//		{
//			THROW(argument+" a non type Matrix member");
//		}
//		else
//		{
//			TPMatrix<TPAny> return_vect(1,1,toAny(xxloper,argument));
//			return return_vect;
//		}
//	}
//
//	int rows = xxloper->val.array.rows, columns = xxloper->val.array.columns, l,k;
//
//	if ((rows == 0) || (columns == 0)) THROW(argument+" Matrix should not be a std::vector")
//
//	TPMatrix<TPAny> return_vect(rows, columns);
//
//	for(l=0;l<rows;l++)
//	{
//		for (k=0;k<columns;k++)
//		{
//			if (!(xxloper->val.array.lparray[l*columns+k].xltype  & xltypeErr))
//			{
//				if (!(xxloper->val.array.lparray[l*columns+k].xltype & xltypeNil)
//					&& !(xxloper->val.array.lparray[l*columns+k].xltype & xltypeMissing))
//				{
//					try
//					{
//						return_vect[l][k] = toAny(&(xxloper->val.array.lparray[ l*columns+k]), argument);
//					}
//					catch(...)
//					{
//
//					}
//				}
//			}
//		}
//	}
//
//	return return_vect;
//}

//TPMatrix<TPAny> TPExcelInterface::toAnyMatrixNoErrs(LPXLOPER xxloper, const std::string& argument, const TPAny& repl)
//{
//	// handle the case of a pseudo array
//	if (!istypeMulti(xxloper))
//	{
//		// if its of the right type then we'll make a std::vector and return it back
//		if (xxloper->xltype &  (xltypeNil | xltypeErr | xltypeMissing))
//		{
//			TPMatrix<TPAny> return_vect(1,1,repl);
//			return return_vect;
//		}
//		else
//		{
//			TPMatrix<TPAny> return_vect(1,1,toAny(xxloper,argument));
//			return return_vect;
//		}
//	}
//
//	int rows = xxloper->val.array.rows, columns = xxloper->val.array.columns, l,k;
//
//	if ((rows == 0) || (columns == 0)) THROW(argument+" Matrix should not be a std::vector")
//
//	TPMatrix<TPAny> return_vect(rows, columns);
//
//	for(l=0;l<rows;l++)
//	{
//		for (k=0;k<columns;k++)
//		{
//			if (!(xxloper->val.array.lparray[l*columns+k].xltype  & xltypeErr))
//			{
//				if ((xxloper->val.array.lparray[l*columns+k].xltype & xltypeNil)
//					|| (xxloper->val.array.lparray[l*columns+k].xltype & xltypeMissing))
//				{
//					return_vect[l][k] = repl;
//				}
//				else
//				{
//					try
//					{
//						return_vect[l][k] = toAny(&(xxloper->val.array.lparray[ l*columns+k]), argument);
//					}
//					catch(...)
//					{
//
//					}
//				}
//			}
//			else
//			{
//				return_vect[l][k] = repl;
//			}
//		}
//	}
//
//	return return_vect;
//}


//
//
//End of Cast section
//
//

LPXLOPER	TPExcelInterface::returnToExcel( const double	rhs)
{

	LPXLOPER	cell_ = new XLOPER;


	//1-	set type
	//2-	set it to rhs if valid num
	//3-	else set it to NAN


	cell_->xltype = xltypeNum | xlbitDLLFree;
	
	if (TPGlobals::isNum(rhs)) cell_->val.num = rhs; 
	else cell_->val.num = TPGlobals::makeNAN();  
	
	return cell_;
   
}

LPXLOPER        TPExcelInterface::returnToExcel( const long      rhs)
{

        LPXLOPER        cell_ = new XLOPER;


        //1-    set type
        //2-    set it to rhs if valid num
        //3-    else set it to NAN


        cell_->xltype = xltypeNum | xlbitDLLFree;

        if (TPGlobals::isNum(rhs)) cell_->val.num = rhs;
        else cell_->val.num = TPGlobals::makeNAN();

        return cell_;

}


LPXLOPER	TPExcelInterface::returnToExcel(const std::string&   rhs)
{

	LPXLOPER	cell_ = new XLOPER;

	unsigned length = rhs.size();
	if (length > 126) length = 126;

	cell_->xltype = xltypeStr | xlbitDLLFree;
	
	cell_->val.str = new char[length+1];
    memcpy(cell_->val.str+1,rhs.c_str(),length);
	cell_->val.str[0] = length;

	return cell_;
   
}

LPXLOPER	TPExcelInterface::returnToExcel(const char*    rhs, size_t length)
{

	return returnToExcel(std::string(rhs, length));
   
}




/*
LPXLOPER	TPExcelInterface::returnToExcel(bool   rhs)
{

	LPXLOPER	cell_ = new XLOPER;

	cell_->xltype = xltypeBool | xlbitDLLFree;
	cell_->val.xlbool =	rhs;

	return cell_;
}

  */

//LPXLOPER	TPExcelInterface::returnToExcel(const TPAny& rhs)
//{
//
//	switch(rhs.getType())
//	{
//	case TPAny::INT:	return returnToExcel((long)rhs.toInt()); break;
//	case TPAny::LONG:	return returnToExcel(rhs.toLong()); break;
//	case TPAny::DOUBLE:	return returnToExcel(rhs.toDouble()); break;
//	case TPAny::STRING:	return returnToExcel(rhs.toString()); break;
//	case TPAny::DATE:	return returnToExcel(rhs.toDate().toLong()); break;
//	case TPAny::EMPTY:	return returnToExcel(std::string("")); break;
//	case TPAny::BOOL:	return returnToExcel((long)rhs.toBool()); break;
//	case TPAny::INVALID: 
//	default:	THROW("TPExcelInterface::returnToExcel(const TPAny& rhs): Invalid TPAny structure"); break;
//	}
//
//
//}

LPXLOPER	TPExcelInterface::returnToExcel(const std::vector<double>& rhs)
{

	LPXLOPER	cell_ = new XLOPER;

	//we always return a column
	//TODO implement a method that returns a row.

	cell_->xltype = xltypeMulti | xlbitDLLFree;
	cell_->val.array.rows = rhs.size();
	cell_->val.array.lparray = new XLOPER[rhs.size()];
	cell_->val.array.columns = 1;

	for (int i=0; i< rhs.size();i++)
	{
		cell_->val.array.lparray[i].val.num = rhs[i];
		cell_->val.array.lparray[i].xltype = xltypeNum | xlbitDLLFree;
	}

	return cell_;
}

LPXLOPER	TPExcelInterface::returnToExcel(const std::vector<std::string>& rhs)
{

	LPXLOPER	cell_ = new XLOPER;

	//we always return a column
	//TODO implement a method that returns a row.

	cell_->xltype = xltypeMulti | xlbitDLLFree;
	cell_->val.array.rows = rhs.size();
	cell_->val.array.lparray = new XLOPER[rhs.size()];
	cell_->val.array.columns = 1;

	unsigned length;

	for (int i=0; i< rhs.size();++i)
	{
		length = rhs[i].size();
		if (length > 256) length = 256;

		cell_->val.array.lparray[i].val.str =new char[length+1];
		memcpy(cell_->val.array.lparray[i].val.str+1,rhs[i].c_str(),length);
		cell_->val.array.lparray[i].xltype = xltypeStr | xlbitDLLFree;
		cell_->val.array.lparray[i].val.str[0] =length;


	}

	return cell_;



}


//void processAnyCells(LPXLOPER xxloper, const TPAny& rhs)
//{
//
//	switch(rhs.getType())
//	{
//	case TPAny::DATE:
//		{
//			xxloper->xltype = xltypeNum | xlbitDLLFree;
//	
//			if (TPGlobals::isNum(rhs.toDate().toLong())) xxloper->val.num = static_cast<long>(rhs.toDate().toLong()); 
//			else xxloper->val.num = TPGlobals::makeNAN();
//
//			break;
//		}
//	case TPAny::LONG:
//	case TPAny::INT:
//	case TPAny::BOOL:
//	case TPAny::DOUBLE:
//		{
//			xxloper->xltype = xltypeNum | xlbitDLLFree;
//	
//			if (TPGlobals::isNum(rhs.toDouble())) xxloper->val.num = rhs.toDouble(); 
//			else xxloper->val.num = TPGlobals::makeNAN();
//
//			break;
//		}
//	case TPAny::STRING:
//	case TPAny::EMPTY:
//		{
//			unsigned length = (rhs.toString()).size();
//			if (length > 256) length = 256;
//
//			xxloper->xltype = xltypeStr | xlbitDLLFree;
//	
//			xxloper->val.str = new char[length+1];
//			memcpy(xxloper->val.str+1,(rhs.toString()).c_str(),length);
//			xxloper->val.str[0] = length;
//			break;
//		}
//	case TPAny::INVALID:
//	default:	THROW("TPExcelInterface::returnToExcel(const TPAny& rhs): Invalid TPAny structure"); break;
//
//	}
//
//}

//LPXLOPER	TPExcelInterface::returnToExcel(const std::vector<TPAny>& rhs)
//{
//
//	LPXLOPER	cell_ = new XLOPER;
//
//	//we always return a column
//	//TODO implement a method that returns a row.
//
//	cell_->xltype = xltypeMulti | xlbitDLLFree;
//	cell_->val.array.rows = rhs.size();
//	cell_->val.array.lparray = new XLOPER[rhs.size()];
//	cell_->val.array.columns = 1;
//
//	for (int i=0; i< rhs.size();++i)
//	{
//		processAnyCells(&cell_->val.array.lparray[i],rhs[i]);
//	}
//
//	return cell_;
//
//
//
//}



LPXLOPER	TPExcelInterface::returnToExcel(const TPMatrix<double>& rhs)
{

	LPXLOPER	cell_ = new XLOPER;

	cell_->xltype = xltypeMulti | xlbitDLLFree;
	cell_->val.array.rows = rhs.size(0);
	cell_->val.array.lparray = new XLOPER[rhs.size(0)*rhs.size(1)];
	cell_->val.array.columns = rhs.size(1);

	for (int i=0; i< rhs.size(0);++i)
	{
		for (int j = 0;j<rhs.size(1);++j)
		{
			cell_->val.array.lparray[i*rhs[0].size()+j].val.num = rhs[i][j];
			cell_->val.array.lparray[i*rhs[0].size()+j].xltype = xltypeNum | xlbitDLLFree;

		}
	}
	return cell_;
}



LPXLOPER	TPExcelInterface::returnToExcel(const TPMatrix<std::string>& rhs)
{

	LPXLOPER	cell_ = new XLOPER;

	cell_->xltype = xltypeMulti | xlbitDLLFree;
	cell_->val.array.rows = rhs.size(0);
	cell_->val.array.lparray = new XLOPER[rhs.size(0)*rhs.size(1)];
	cell_->val.array.columns = rhs.size(1);


	size_t length;

	for (int i=0; i< rhs.size(0);++i)
	{
		for (int j = 0;j<rhs.size(1);++j)
		{

			length = rhs[i][j].size();
			if (length > 256) length = 256;

			cell_->val.array.lparray[i*rhs[0].size()+j].val.str =new char[length+1];
			memcpy(cell_->val.array.lparray[i*rhs[0].size()+j].val.str+1,rhs[i][j].c_str(),length);
			cell_->val.array.lparray[i*rhs[0].size()+j].xltype = xltypeStr | xlbitDLLFree;
			cell_->val.array.lparray[i*rhs[0].size()+j].val.str[0] =length;


		}
	}
	return cell_;
}



//LPXLOPER	TPExcelInterface::returnToExcel(const TPMatrix<TPAny>& rhs)
//{
//
//	LPXLOPER	cell_ = new XLOPER;
//
//	cell_->xltype = xltypeMulti | xlbitDLLFree;
//	cell_->val.array.rows = rhs.size(0);
//	cell_->val.array.lparray = new XLOPER[rhs.size(0)*rhs.size(1)];
//	cell_->val.array.columns = rhs.size(1);
//
//	for (int i=0; i< rhs.size(0);++i)
//	{
//		for (int j = 0;j<rhs.size(1);++j)
//		{
//			processAnyCells(&cell_->val.array.lparray[i*rhs[0].size()+j],rhs[i][j]);
//
//		}
//	}
//	return cell_;
//}

//LPXLOPER	TPExcelInterface::returnToExcel(const std::vector<std::pair<double, double>>&	 rhs)
//{
//	TPMatrix<TPAny> myOutputMatrix;
//	std::vector< TPAny>	myAnyVec;
//	for (const auto& p : rhs)
//	{
//		myAnyVec.clear();
//		myAnyVec.push_back(TPAny(p.first));
//		myAnyVec.push_back(TPAny(p.second));
//		myOutputMatrix.push_back(myAnyVec, 0);
//	}
//	return returnToExcel(myOutputMatrix);
//}

BOOL CALLBACK EnumProc(HWND hwnd, EnumStruct * pEnum)
{
   const size_t CLASS_NAME_BUFFER = 50;

	// first check the class of the window.  Must be "XLMAIN"
   char className[CLASS_NAME_BUFFER];
	
   if (0 != ::GetClassName(hwnd, className, CLASS_NAME_BUFFER))
   {
      if (!lstrcmpi(className, "XLMAIN")) 
      {
		   // if that hits, check the loword of the window handle
	      if (LOWORD((DWORD) hwnd) == pEnum->wLoword) 
         {
			   // We have a match, return Excel's main hWnd
            pEnum->hwnd = hwnd;
			   return FALSE;
		   }
	   }
   }

   // no luck - continue the enumeration
	return TRUE;
}

HWND getExcelHandle()
{
   XLOPER x;
  
   //
   // xlGetHwnd only returns the LoWord of Excel's hWnd
   // so all the windows have to be enumerated to see
   // which match the LoWord retuned by xl2
   //

   TPExcelInterface::callExcel(xlGetHwnd, &x, 0);

   EnumStruct enm;
		
	enm.hwnd = NULL;
	enm.wLoword = x.val.w;

   // Docs say that EnumWindows() should return 0 on failure, but it also returns 0 if
   // the enum proc returns 0 and the enum proc must return 0 to stop enumeration!

   ::EnumWindows((WNDENUMPROC) EnumProc, (LPARAM) &enm);
		
	if (enm.hwnd == NULL) 
   {
	   THROW("Unable to locate Excel's main window handle");
   }

 

   return enm.hwnd;
}



int TPExcelInterface::DisplayDialog(
   HINSTANCE hInstance, 
   LPCTSTR lpTemplate, 
   DLGPROC lpDialogProc)
{

   int nRc = -1;


 

   HWND hWndMain = getExcelHandle();

   callExcel(xlEnableXLMsgs, 0, 0);

   nRc = ::DialogBox(hInstance, lpTemplate, hWndMain, lpDialogProc);

   if (nRc <= 0)
   {
      DWORD lastError = ::GetLastError();

      THROW("TPExcelInterface::DisplayDialog() - Failed to show dialog");
   }

   callExcel(xlDisableXLMsgs, 0, 0);

   return nRc;
}

BOOL CALLBACK IsFunctionWizardActiveEnumProc(HWND hwnd, EnumStruct *pEnum)
{
    // first check the class of the window.  Will be szXLDialogClass
    // if function wizard dialog is up in Excel
   const size_t CLASS_NAME_BUFFER = 50;
   
   char className[CLASS_NAME_BUFFER];
   
   if (0 != ::GetClassName(hwnd, className, CLASS_NAME_BUFFER))
   {
      // The window class name starts "bosa_sdm_XL" but may have a version
      // number on the end, Excel 97 has the name "bosa_sdm_XL8"
      // We just compare the bosa_sdm_XL bit...

      static LCID lcid = MAKELCID(MAKELANGID(LANG_ENGLISH,SUBLANG_ENGLISH_US),SORT_DEFAULT);

      static const char * const pWizCls = "bosa_sdm_XL";

      static const size_t lenWizCls = lstrlen(pWizCls);

      const size_t lenClassName = (lstrlen(className)<lenWizCls)?lstrlen(className):lenWizCls;

      if (2 == ::CompareString(lcid, NORM_IGNORECASE, className, lenClassName, pWizCls, lenWizCls))
      {
         if(::GetParent(hwnd) == pEnum->hwnd) 
         {
            static TCHAR windowTitle[2];


            if (0 == ::GetWindowText(hwnd, windowTitle, 2))
            {
               // Function wizard doesnt have a title bar, Replace window does...

               pEnum->bInWizard = true;
               return FALSE;
            }
         }
      }
   }

   // no luck - continue the enumeration
   return TRUE;
}



bool TPExcelInterface::IsFunctionWizardActive()
{
   try
   {
      EnumStruct enm;

      enm.bInWizard = false;
      enm.hwnd = getExcelHandle();
     
      ::EnumWindows((WNDENUMPROC) IsFunctionWizardActiveEnumProc, (LPARAM) &enm); 
        
      return enm.bInWizard;
   }
   catch(...)
   {
      // do nothing, just catch them...
   }

   return false;    //safe case: Return False if not sure
} 


int TPExcelInterface::xlAlert(std::string pMsg)
{
 XLOper xlTemp = 2;
 XLOper xlMsg = pMsg;

 return Excel4(xlcAlert, 0, 2,(LPXLOPER)&xlMsg,(LPXLOPER)&xlTemp);
}


///
// Simple conversion routine from an std::vector<double> to std::vector<TPAny>

//std::vector<TPAny>    TPExcelInterface::fromDoubletoAnyArray (const std::vector<double>& xx)
//{
//	std::vector<TPAny>  result(xx.size());
//	for(int i=0 ; i < xx.size(); ++i)
//		result[i]=TPAny(xx[i]);
//
//	return result;
//}