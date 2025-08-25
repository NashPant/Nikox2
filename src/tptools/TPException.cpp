//#include <boost/core/demangle.hpp>
#include "TPException.h"
#include "TPGlobals.h"
//#include <boost/core/demangle.hpp>

TPException::TPException(const std::string& error, const char * file, int line)
	: std::runtime_error(error)
{
    myMessage       = error;
    myFullMessage   = TPGlobals::getDateString();
    myFullMessage   +=" ";
	std::string  pipo_spy= TPGlobals::getTimeString();
    myFullMessage   += pipo_spy;
    myFullMessage   +="  ";
	std::string pipo_spy1 = std::string(file);
    myFullMessage   += pipo_spy1;
    myFullMessage   +="  ";
    
	pipo_spy = TPGlobals::LongToString(line);
    myFullMessage   += pipo_spy;
    myFullMessage   +="  ";
    myFullMessage   +=error;

    
}

TPException::TPException(const std::string& error, const std::string& fullerror ):
	std::runtime_error(error),myMessage(error),myFullMessage(fullerror)
{
}

TPWarning::TPWarning(const std::string& error, const char * file, int line)
{
    myFullMessage   = TPGlobals::getDateString();
    myFullMessage   +=" ";
    myFullMessage   +=TPGlobals::getTimeString();
    myFullMessage   +="  ";
    myFullMessage   += std::string(file);
    myFullMessage   +="  ";
    
    myFullMessage   +=TPGlobals::LongToString(line);
    myFullMessage   +="  ";
    //In Capitals so that it clearly stands out when hanving a quick look into the error file.
    myFullMessage   +="WARNING: ";
    myFullMessage   +=error;    
    
}


TPWarning::TPWarning(const std::string& fullerror ):myFullMessage(fullerror)
{
}


//std::string stripTypeName(const std::type_info& type)
//{
//	// We trim everything before the last space
//	const std::string& typeName = boost::core::demangle(type.name());
//	const char* lastColonOrSpace;
//	const char* endOfTypeName;
//
//	{
//		const char find1 = ':';
//		const char find2 = ' ';
//		const char eos = '<';
//		const char* const str = typeName.c_str();
//
//		// Finds the last occurrence of find1 or find2 before the end of the string or any occurrence of the end-of-string character (eos)
//		const char* found = nullptr;
//		const char* i = str;
//		while (*i != 0 && *i != eos)
//		{
//			if (*i == find1 || *i == find2)
//				found = i;
//			++i;
//		}
//
//		lastColonOrSpace = (found) ? found + 1 : str;
//		endOfTypeName = i;
//	}
//
//	return std::string(lastColonOrSpace, endOfTypeName);
//}
