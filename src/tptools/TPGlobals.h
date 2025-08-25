////////////////////////////////////////////////////////////////////////////
//
//  This program is the CONFIDENTIAL and PROPRIETARY property of'
//   Athanasios Pantas. Unauthorized use and reproduction'
//  of this program is strictly prohibited.
//
// Copyright (C) 2003  - Nikopolis
//  Copyright (C) 2003 - Athanasios Pantas.
//  ALL RIGHTS RESERVED.
/////////////////////////////////////////////////////////////////////////////


#ifndef TP_GLOBALS
#define TP_GLOBALS

#include <TPNikopolisUtils.h>
#include <string>
//#include "TPMarket/TemplateStore.h"


class  TPGlobals
{

public:
	//time and date operations
         TPNIKOPOLIS_IMPEXP static std::string getDateString();
         TPNIKOPOLIS_IMPEXP static std::string getTimeString();

	//error file handler
         TPNIKOPOLIS_IMPEXP static std::string getErrorFileHandle();
		 TPNIKOPOLIS_IMPEXP static std::string getMathErrorFileHandle()
		 {
			 return ourMathErrorFileHandle;
		 }
		 // warning file handler
		 TPNIKOPOLIS_IMPEXP static std::string getWarningFileHandle()
		 {
			 return ourPotentiallyCriticalWarningFileHandle;
		 }
	//Holidays file handle

		 TPNIKOPOLIS_IMPEXP static std::string getHolidaysFileHandle();
		 TPNIKOPOLIS_IMPEXP static std::string getHolidaysFileSeperator();


	//string operations
	 TPNIKOPOLIS_IMPEXP static std::string LongToString(const  long x);
	 TPNIKOPOLIS_IMPEXP static int    StringToInt(const char* sequence);

	TPNIKOPOLIS_IMPEXP static const double POS_INFINITY;
	TPNIKOPOLIS_IMPEXP static const double NEG_INFINITY;
	TPNIKOPOLIS_IMPEXP static bool isFinite(double x);

	//type conversion operations
	 TPNIKOPOLIS_IMPEXP static bool isInt(const double x);
	 TPNIKOPOLIS_IMPEXP static bool isNum(const double x);
	 TPNIKOPOLIS_IMPEXP static double makeNAN();
	 TPNIKOPOLIS_IMPEXP static bool isNAN(const double x);

     //get the string value of an environmental variable
     TPNIKOPOLIS_IMPEXP static std::string getEnvVar(std::string const & key);
	 /*
	  *	create a global Template store
	  */
	// TPNIKOPOLIS_IMPEXP static TP::Market::TemplateStoreConstPtr getTemplates(std::string const & key);
private:
	TPGlobals(){}
	~TPGlobals(){}

         static std::string ourErrorFileHandle;
		 static std::string ourPotentiallyCriticalWarningFileHandle;
		 static std::string ourMathErrorFileHandle;
};

//neat memory management for excel interface

template <class T>
inline void TPDeleteArray( T *&p ){
    if (p) {
        delete[] p;
        p = 0;
    }
}

template <class T>
inline void TPDeleteOne( T *&p ){
    if (p) {
        delete p;
        p = 0;
    }
}

//end of excel interface memory management function

TPNIKOPOLIS_IMPEXP std::string ToString(int a) ;
TPNIKOPOLIS_IMPEXP std::string ToString(long a) ;
TPNIKOPOLIS_IMPEXP std::string ToString(double a);
TPNIKOPOLIS_IMPEXP std::string ToString(bool a) ;



#endif