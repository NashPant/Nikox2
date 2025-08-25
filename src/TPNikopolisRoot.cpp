#include "TPNikopolisRoot.h"
//#include <boost/filesystem.hpp>
#include <stdlib.h>  
#include <filesystem>
//#include <boost/filesystem/path.hpp>
//#include <boost/filesystem/operations.hpp>
#include "TPTools/TPException.h"

namespace fs = std::filesystem;
namespace 
{
	//namespace fs = boost::filesystem;
	std::string _subdir(const std::string& subdir)
	{
		//using path = boost::filesystem::path;
		using path = fs::path;
		path codeRoot_ = path(__FILE__).remove_filename().parent_path().parent_path();
		path conf("\\" + subdir);
		codeRoot_ = codeRoot_ / conf;
		return codeRoot_.string();
	}

	int setenv(const char *name, const char *value, int overwrite)
	{
		int errcode = 0;
		if (!overwrite) {
			size_t envsize = 0;
			errcode = getenv_s(&envsize, NULL, 0, name);
			if (errcode || envsize) return errcode;
		}
		return _putenv_s(name, value);
	}


	void setNewEnvVariable(const std::string varName, const std::string value)
	{
		setenv(varName.c_str(), value.c_str(), true);
	}

	std::pair<bool, std::string> hasNikoConfDir()
	{
		std::string nikodir;
		try {
			nikodir = std::string(getenv("USERPROFILE")) + "\\niko\\conf";
		}
		catch (...)
		{
			nikodir = std::string("C:\\Users\\") + std::string(getenv("username")) + "\\niko\\conf";
		}
		fs::path data_dir(nikodir);
		return { fs::exists(data_dir), nikodir };
	}


}

std::string TPNikopolisRoot::codeRoot()
{
	//using path = boost::filesystem::path;
	// Normalize to absolute path in case compiler gave us a relative one
	fs::path fullPath = fs::absolute(loc.file_name());

	// Get the directory of the source file
	fs::path codeRoot = fullPath.remove_filename().parent_path();

	return codeRoot.string();
}

std::string TPNikopolisRoot::confDir()
{
	auto p = hasNikoConfDir();
	if (p.first)
		return p.second;

	return _subdir("conf");
}

std::string TPNikopolisRoot::TemplatesDir()
{
	auto p = hasNikoConfDir();
	if (p.first)
		return p.second + "\\Templates";
	return _subdir("conf\\Templates");
}

std::string TPNikopolisRoot::IndicesDir()
{
	auto p = hasNikoConfDir();
	if (p.first)
		return p.second + "\\indices";
	return _subdir("conf\\indices");
}
std::string TPNikopolisRoot::CalendarsDir()
{
	auto p = hasNikoConfDir();
	if (p.first)
		return p.second + "\\Calendars";
	return _subdir("conf\\Calendars");
}

bool TPNikopolisRoot::setNikoHolidayEnv()
{
	const char * myHolidaysFileHandle = getenv("NIKO_HOLIDAYS_FILE");
	if (myHolidaysFileHandle == NULL) // define environmental Variable
	{
		auto ff = TPNikopolisRoot::CalendarsDir() + "\\NikoHolidays.hol";
		setNewEnvVariable("NIKO_HOLIDAYS_FILE",ff);
		// check again
		myHolidaysFileHandle = getenv("NIKO_HOLIDAYS_FILE");
		if (myHolidaysFileHandle == NULL)
			THROW("Cannot Set NikoHolidays file")
		return true;

	}
	return true;
}

//bool TPNikopolisRoot::setNikoTemplateDirEnv()
//{
//	const char * nikoTemplateDir = getenv("NIKO_TEMPLATE_DIR");
//	if (nikoTemplateDir == NULL) // define environmental Variable
//	{
//		setNewEnvVariable("NIKO_TEMPLATE_DIR", TemplatesDir());
//		// check again
//		nikoTemplateDir = getenv("NIKO_TEMPLATE_DIR");
//		if (nikoTemplateDir == NULL)
//			THROW("Cannot Set Template Directory to " + TemplatesDir());
//		return true;
//	}
//	return true;
//}