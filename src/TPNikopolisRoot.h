////////////////////////////////////////////////////////////////////////////
//
//  This program is the  property of'
//   Athanasios Pantas. Feel free to use, but no guarantees from me!
//  
//
// Copyright (C) 2003  - Nikopolis
//  Copyright (C) 2003 - Athanasios Pantas.
//  ALL RIGHTS RESERVED.
/////////////////////////////////////////////////////////////////////////////

#ifndef TP_NIKOPOLIS_ROOT
#define TP_NIKOPOLIS_ROOT

#include <string>
#include "TPNikopolisUtils.h"

//namespace NIKO {
	class TPNIKOPOLIS_IMPEXP TPNikopolisRoot
	{
	public:
		virtual ~TPNikopolisRoot() {}
		static std::string codeRoot();
		/*
		 * @brief. Configuration directory which is local to the code.
		 *		   Note this will not be the one that will be used in the production environment
		 *		   For each individual subdirectories we create a function to make things easier.
		 */
		static std::string confDir();
		static std::string TemplatesDir();
		static std::string IndicesDir();
		static std::string CalendarsDir();
		/*
		 * @brief. Checks if OLD Niko Holidays file is there. If not it will try to set env variables from
		 *		   the Dev directory, otherwise will throw. 
		 */
		static bool setNikoHolidayEnv();
		/*
		 * @brief. Checks environmental variable for Templates ID
		 */
		static bool setNikoTemplateDirEnv();
	};
//}

#endif