#include "TPGlobals.h"
#include <limits>
#include <fstream>

#ifndef WIN32
#include <ctime>
#else
#include <time.h>
#endif

#ifndef TPSTATIC

#include <stdlib.h>
#include <math.h>
#include <string>
#ifndef WIN32
#include <sstream>
#else
//Michele #include <strstrea.h>
#include <sstream>
#include <stdio.h>
#endif

#include <float.h>
#include <cmath>

#ifndef WIN32
//MIchele #include <ieeefp.h>
#endif

#endif

#include "TPException.h"


#pragma warning( disable : 4723) // ignore dividing by zero  warning

// don't need this any more.
//#ifndef WIN32
//std::string TPGlobals::ourErrorFileHandle = std::string(getenv( "HOME" )) + std::string("/error/RibkaErrors.log");
//std::string TPGlobals::ourMathErrorFileHandle = std::string(getenv( "HOME" )) + std::string("/error/RibkaMathErrors.log");
//std::string TPGlobals::ourPotentiallyCriticalWarningFileHandle = std::string(getenv( "HOME" )) + std::string("/error/RibkaWarnings.log");
//#else
std::string TPGlobals::ourErrorFileHandle = "c:\\temp\\NikoErrors.log";
std::string TPGlobals::ourMathErrorFileHandle = "c:\\temp\\NikoMathErrors.log";
std::string TPGlobals::ourPotentiallyCriticalWarningFileHandle = "c:\\temp\\NikoWarnings.log";
//#endif

const double TPGlobals::POS_INFINITY = std::numeric_limits<double>::infinity();
const double TPGlobals::NEG_INFINITY = std::numeric_limits<double>::infinity() * -1.0;


std::string TPGlobals::getDateString()
{
        time_t mytime;
        struct tm * ourtime;

        time(&mytime);
        ourtime = localtime(&mytime);
		std::string ret;
	    ret = LongToString(ourtime->tm_mday);
        ret +="/" ;
	if ((ourtime->tm_mon+1) < 10) ret +="0";
        ret +=LongToString(ourtime->tm_mon+1);
        ret +="/" ;
        ret +=LongToString(ourtime->tm_year+1900);
	
        return ret;
}

std::string TPGlobals::getTimeString()
{
        time_t mytime;
        struct tm * ourtime;

        time(&mytime);
        ourtime = localtime(&mytime);
	
		std::string ret;
	if (ourtime->tm_hour<10) ret = "0"+LongToString(ourtime->tm_hour);
	else  ret = LongToString(ourtime->tm_hour);
        ret +=":";
	if (ourtime->tm_min<10) ret +="0";
	ret +=LongToString(ourtime->tm_min);
        return ret;
}

std::string TPGlobals::getErrorFileHandle()
{
  return ourErrorFileHandle;
}

std::string TPGlobals::getHolidaysFileHandle()
{

	const char * myHolidaysFileHandle = getenv("NIKO_HOLIDAYS_FILE");


	if( myHolidaysFileHandle == NULL)
	{
		THROW("TPGlobals::getHolidaysFileHandle: The path for the holidays is not set up.");
	}

	return myHolidaysFileHandle;


}


std::string TPGlobals::getHolidaysFileSeperator()
{

	char * myHolidaysFileSeperator;

	myHolidaysFileSeperator = getenv("NIKO_HOLIDAYS_FILE_SEPERATOR");

	return myHolidaysFileSeperator;
}

std::string TPGlobals::getEnvVar(std::string const & key)
{

    char * val = getenv(key.c_str());
    return val == nullptr ? std::string("") : std::string(val);
}


std::string TPGlobals::LongToString(const  long x)
{
  char tmp[255]; //Assuming long long does not have more than 255 digits!

  std::string ret;
  
#ifdef WIN32
  ret = itoa(x,tmp,10);
#else
  std::ostringstream ostr;
  ostr << x;
  ret = ostr.str();
#endif

  return ret;
}

int TPGlobals::StringToInt(const char* sequence)
{
  return atoi(sequence);
}

bool TPGlobals::isInt(const double x)
{
	//silly thing..
	//TODO get rid of the hard coded bit.

	return (x - floor(x)) < 1.0e-30;
}

bool TPGlobals::isNum(const double x)
{
#ifdef WIN32
	return (_finite(x) && !_isnan(x));
#else
	return (isFinite(x) && !isnan(x));
#endif
}

bool TPGlobals::isNAN(const double x)
{
#ifdef WIN32
	return (_isnan(x));
#else
	return (isnan(x));
#endif
}


double TPGlobals::makeNAN()
{
	double dummyDouble = 0.0;
	dummyDouble /= dummyDouble;
	return (double) dummyDouble;
}


std::string ToString(double a)
{
	std::string ret;
	char * tmp = new char[255];
#ifdef WIN32
	sprintf(tmp,"%g",a);
	ret = tmp;
#else
	std::ostringstream ostr;
	ostr << a;
	ret = ostr.str();
#endif
	return ret;
}
bool TPGlobals::isFinite(double x)
{
//#ifdef WIN32 //Microsoft Specific
        return (_finite(x));
//#else
 //       return (isFinite(x));
//#endif
};

std::string ToString(bool a) { if (a) return "true"; else return "false";}
std::string ToString(int a) { return TPGlobals::LongToString(a);}
std::string ToString(long a) { return TPGlobals::LongToString(a);}
