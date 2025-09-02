#ifndef TP_XL_INTERFACE_H
#define TP_XL_INTERFACE_H

#ifndef TPSTATIC


#ifdef WIN32	//WIN stuff
//
//common pragma section to all DLLs.
//Suppress some warnings and basic
//VC++ limitations. Refer to MSDN
//for more details
//

#if defined(WIN32) && defined(_MSC_VER)

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

#endif

//
//In case we are compiling a DLL we need to export,
//we import otherwsie

#ifdef TPXLINTERFACE_EXPORTS
#define TPXLINTERFACE_IMPEXP		__declspec(dllexport)	
#else
#define TPXLINTERFACE_IMPEXP		__declspec(dllimport)
#endif 


#else

#define TPXLINTERFACE_IMPEXP	//SUN, LINUX and other platforms

#endif //end of WIN stuff


#else
#include <utility>
//using namespace std;
#ifdef TPXLINTERFACE_EXPORTS
#define TPXLINTERFACE_IMPEXP		__declspec(dllexport)	
#else
#define TPXLINTERFACE_IMPEXP		__declspec(dllimport)
#endif 

#endif // TPSTATIC

#endif