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

#ifndef TP_EXCEPTION
#define TP_EXCEPTION

//The idea behind this class is to send TPExceptiob.Why() to the Display 
//like for instance Excel Sheets, and to log TPException.getFullMessage
//into an error file.
//TODO complete the log into an error file. DONE
//TODO we can do this inside the constructor, i.e. get the errorfile. DONE.
//name from TPGlobals, open it for appending, and log the error message.
// or, an alternative option would be to add this in the macro definition
//at the end of this file.

#include "TPNikopolisUtils.h"
#include <string>
#include <iostream> 
#include "TPGlobals.h"
#ifdef TPSTATIC

#include <sstream>
#include <fstream>

#else

#ifndef WIN32
#include <fstream>
#else
#include <fstream>
#include <sstream>
//Michele #include <fstream.h>
#endif

#endif // TPSTATIC


class  TPNIKOPOLIS_IMPEXP TPException: public std::runtime_error
{
  
public:
  TPException(const std::string& error) :TPException(error, __FILE__, __LINE__) {}
  TPException(const std::string& error, const char * file, int line);
  TPException(const std::string& error, const std::string& fullerror);

  TPException(const TPException& rhs) = default;
 /* TPException(const TPException& rhs)
  {
    myMessage       = rhs.myMessage;
    myFullMessage   = rhs.myFullMessage;
  }*/
  const TPException& operator=(const TPException& rhs)
  {
    if (this != &rhs)
    {
      myMessage       = rhs.myMessage;
      myFullMessage   = rhs.myFullMessage;
    }
    return *this;
  }
  
  virtual ~TPException(){}
  
  virtual std::string Why() const
  {
    return myMessage;
  }
  
  virtual std::string getFullMessage() const
  {
   return myFullMessage;

  }
  
private:
  //to be sent to our GUI Interface (Excel or other)
	std::string myMessage;
  
  //To be logged into our error file with file name and line number.
	std::string myFullMessage;
};


class TPNIKOPOLIS_IMPEXP TPWarning
{
  
public:
  //an obvious copy paste from the above.....
  
  TPWarning(const std::string& error, const char * file, int line);
  TPWarning(const std::string& fullerror);

  
  TPWarning(const TPWarning& rhs):myFullMessage(rhs.myFullMessage){}
  
  virtual ~TPWarning(){}
  
  std::string getMessage()	const { return myFullMessage;}
private:
  
	std::string myFullMessage;
  
};     


//TPNIKOPOLIS_IMPEXP std::string stripTypeName(const std::type_info& type);

//default throw : logs full error message into error file
//then throws.

#ifdef _DEBUG

#define THROW(error)   {TPException error_( (error), __FILE__, __LINE__);\
  std::ofstream myFile(TPGlobals::getErrorFileHandle().c_str(), std::ios::app); \
  myFile << std::endl<<error_.getFullMessage(); \
  myFile.close(); \
  throw error_ ; \
}

#define THROW2(error1, error2)   {TPException error_( (std::string(std::string(error1)+std::string(error2))), __FILE__, __LINE__); \
  std::ofstream myFile(TPGlobals::getErrorFileHandle().c_str(), std::ios::app); \
  myFile << std::endl<<error_.getFullMessage(); \
  myFile.close(); \
  throw error_ ; \
}

#else

// FROM QL
#define MULTILINE_MACRO_BEGIN do {

#if defined(BOOST_MSVC) && BOOST_MSVC >= 1500
/* __pragma is available from VC++9 */
#define MULTILINE_MACRO_END \
        __pragma(warning(push)) \
        __pragma(warning(disable:4127)) \
        } while(false) \
        __pragma(warning(pop))
#else
#define MULTILINE_MACRO_END } while(false)
#endif


#define THROW(error) throw TPException( (error), __FILE__, __LINE__);
#define THROW2(error1, error2) throw TPException( (std::string(std::string(error1)+std::string(error2))), __FILE__, __LINE__);



#endif


#define  THROW_INVALID_METHOD throw TPException( std::string("Not applicable to option type."), __FILE__, __LINE__);
#define  THROW_MISSING_VALUE_EXCEPTION(msg)  throw TPException( std::string( "Nikopolis Missing Value Exception " ) + msg,  __FILE__, __LINE__);
#define  THROW_INVALID_OPERATION_EXCEPTION(msg) throw TPException( std::string( "Nikopolis Invalid Operation Exception: " ) + msg,  __FILE__, __LINE__);
#define  THROW_INVALID_INPUT_EXCEPTION(msg)  throw TPException( std::string("Nikopolis Input Excelption: " ) + msg,  __FILE__, __LINE__);
#define  THROW_NOT_SUPPORTED_EXCEPTION(msg)  throw TPException( std::string("Nikopolis Not suppoerted Excelption: " ) + msg,  __FILE__, __LINE__);


/// Add a require statements 
// throw an error if the given pre-condition is not verified
#define TP_REQUIRE(condition,description) \
  do { \
  if (!(condition)) \
  THROW(description) \
  } while (false)

// QL equivalent version
#define TP_REQUIRE_QL(condition,message) \
if (!(condition)) { \
    std::ostringstream _ql_msg_stream; \
    _ql_msg_stream << message; \
    THROW(_ql_msg_stream.str()); \
 } else 

#define TP_ENSURE(condition,message) \
if (!(condition)) { \
    std::ostringstream _ql_msg_stream; \
    _ql_msg_stream << message; \
    THROW (_ql_msg_stream.str()); \
 } else 

#define TP_THROW_IF_NULL_PTR(e,msg) \
if(nullptr ==(e)) \
 THROW(msg)


//MULTILINE_MACRO_BEGIN \

#define TP_FAIL_QL(message) \
MULTILINE_MACRO_BEGIN \
    std::ostringstream _ql_msg_stream; \
    _ql_msg_stream << message; \
    THROW(_ql_msg_stream.str()); \
MULTILINE_MACRO_END
//A variation that gives access to TPException before it gets thrown.
//I dont believe this is necessary since we can always catch exceptions
//and then call why() on them. Anyway...

#define EXCEPTION(error) TPException error__( (error), __FILE__, __LINE__);

// Example:
//
// {
//	EXCEPTION("I screwed up");
//	do_something(error__);
//	throw error__;
// }



//A Warning should never throw an error or be sent to the interface.
//Only logged into our error file for debugging. i.e stuff
//that we r not sure about or a worst case scenario we are not sure
//about handling the right way in the development phase. This
//would act as a reminder.

//DON'T SPREAD THESE ALL OVER THE CODE AND FORGET
//TO REMOVE THEM BACK. DO NOT POPULATE THEM ALL OVER THE PLACE AS 
//THESE ARE VERY TIME CONSUMING


#define WARNING(error)	{TPWarning error_( (error), __FILE__, __LINE__);\
  std::ofstream myFile(TPGlobals::getErrorFileHandle().c_str(), std::ios::app); \
  myFile << std::endl<<error_.getMessage(); \
}

#define CRITICAL_WARNING(error)	{TPWarning error_( (error), __FILE__, __LINE__);\
  std::ofstream myFile(TPGlobals::getWarningFileHandle().c_str(), std::ios::app); \
  myFile << std::endl << error_.getMessage(); \
}


# define TP_ASSERT( expr ) if( !(expr) ) THROW( "TP assertion failure : " #expr )

#endif