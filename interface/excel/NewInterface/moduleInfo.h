#pragma once
#include "../TPXLInterfaceUtils.h"
#include <string>

namespace TP
{
	namespace Excel
	{
		TPXLINTERFACE_IMPEXP const std::string& modulePath();
		TPXLINTERFACE_IMPEXP const std::string& moduleName();
		TPXLINTERFACE_IMPEXP	const std::string& installationPath(void);
		TPXLINTERFACE_IMPEXP	 void DisplayError(const char* lpszFunction);
	}
}
