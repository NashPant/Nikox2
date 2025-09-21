////////////////////////////////////////////////////////////////////////////
//
//  This program is the  property of 
//   Athanasios Pantas. Feel free to use, but no guarantees from me!
//  
//
// Copyright (C) 2003  - Nikopolis
//  Copyright (C) 2003 - Athanasios Pantas.
//  ALL RIGHTS RESERVED.
/////////////////////////////////////////////////////////////////////////////

#ifndef TP_MATH_EXCEPTION_CLASS_TP_
#define TP_MATH_EXCEPTION_CLASS_TP_

//The idea behind this class is to send TPExceptiob.Why() to the Display 
//like for instance Excel Sheets, and to log TPException.getFullMessage
//into an error file.
//TODO complete the log into an error file. DONE
//TODO we can do this inside the constructor, i.e. get the errorfile. DONE.
//name from TPGlobals, open it for appending, and log the error message.
// or, an alternative option would be to add this in the macro definition
//at the end of this file.

//#include <TPNikopolisUtils.h>
#include "TPException.h"
#include <string>



static const char *ermsg[9] = {
"unknown",      // error code 0 
"domain",       //  error code 1 
"singularity",  // error code 2
"overflow",   // error code 3
"underflow",  // error code 4
"total loss of precision",	// error code 5
"partial loss of precision" // error code 6
"failed to solve"			// error code 7
"failed to calibrate"     // error code 8
"index outside bounds" // error code 9
"zero value" // error code 10
};


class   TPMathException:public TPException
{

public:

	TPMathException(const std::string& error, const char * file, int line,int code,
									bool iscritial=true):TPException(error,file,line)
        {
			m_str_errorcode =std::string(ermsg[code]);
			m_error_code_ = code;
			m_bl_critical_  =iscritial;


        }

	TPMathException(const TPMathException& rhs):TPException(rhs)
        {
	            m_str_errorcode = rhs.m_str_errorcode;
				m_error_code_	= rhs.m_error_code_;
				m_bl_critical_		= rhs.m_bl_critical_;

        }
        const TPMathException& operator=(const TPMathException& rhs)
        {
                if (this != &rhs)
                {
					TPException::operator =(rhs);
					m_str_errorcode = rhs.m_str_errorcode;
					m_error_code_  = rhs.m_error_code_;
					m_bl_critical_		= rhs.m_bl_critical_;
                }
                return *this;
        }

        virtual ~TPMathException(){}

		std::string getMathErrorMessage() const
        {
                return m_str_errorcode;
        }
		bool	isCritical() const	{ return m_bl_critical_;}

private:
		 std::string  m_str_errorcode;
		 int		m_error_code_;
		 bool	  m_bl_critical_;  // boolean to indicate whether the error is critical for further program execution.
};

#define MATHERROR(error, error_code, critical)   {TPMathException error_( (error), __FILE__ , __LINE__, (error_code),critical );\
												std::ofstream myFile(TPGlobals::getMathErrorFileHandle().c_str(), std::ios::app); \
                                               myFile << std::endl << error_.getFullMessage(); \
                                                throw error_ ; \
                                                }

/// 
// throw a math error if the given pre-condition is not verified
#define TP_MATH_REQUIRE(condition,description,error_code,critical) \
  do { \
  if (!(condition)) \
  MATHERROR(description,error_code,critical) \
  }while (false)
#endif


