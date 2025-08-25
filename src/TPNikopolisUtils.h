#ifndef TP_NIKOPOLIS_UTILS_H
#define TP_NIKOPOLIS_UTILS_H

#include "TPNikopolis_Core.h"
// Some type-defs
#define		T_TEMPLATE		template <class T>

//#if defined(_MSC_VER) || defined(__MINGW32__) || defined(__MINGW64__)//Michele
//Michele #ifdef WIN32      //WIN stuff

//
//common pragma section to all DLLs.
//Suppress some warnings and basic
//VC++ limitations. Refer to MSDN
//for more details
//

//#if defined(WIN32) && defined(_MSC_VER)
#if  defined(_MSC_VER)

#pragma warning(disable : 4129) 
#pragma warning(disable : 4231) 
#pragma warning(disable : 4251) 
#pragma warning(disable : 4275)  
#pragma warning(disable : 4503) 
#pragma warning(disable : 4786) 
#pragma warning(disable : 4800) 
#pragma warning(disable : 4305)
#pragma warning(disable : 4047)
#pragma warning(disable : 4761)
#pragma warning(disable : 4146)
#pragma warning(disable : 4018)

#endif

//
//In case we are compiling a DLL we need to export,
//we import otherwsie


#ifdef TPNIKOPOLIS_EXPORTS
#define TPNIKOPOLIS_IMPEXP		__declspec(dllexport)	
#else
#define TPNIKOPOLIS_IMPEXP		__declspec(dllimport)
#endif 




#endif