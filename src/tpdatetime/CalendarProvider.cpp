#include "CalendarProvider.h"
#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/trim.hpp>



namespace TP
{
	namespace date_time
	{

		CalendarProviderConstPtr CalendarProvider::create(const std::vector<std::string>& names,
			const std::vector<CalendarConstPtr>& cals)
		{
			std::shared_ptr<CalendarProvider> calMap(new CalendarProvider());
			std::transform(names.begin(), names.end(), cals.begin(), std::inserter(*calMap, calMap->end()),
				std::make_pair<std::string const&, CalendarConstPtr const&>);

			return calMap;
		 }

		CalendarProviderConstPtr CalendarProvider::create(const std::map<std::string, CalendarConstPtr>& cals)
		{
			std::shared_ptr<CalendarProvider> calMap(new CalendarProvider());
			std::transform(cals.begin(), cals.end(), std::inserter(*calMap, calMap->end()),
				[&](const auto& in) {return in; });

			return calMap;

 	   }

		bool CalendarProvider::hasCalendar(const std::string& name) const
		{
			const auto calupper = boost::to_upper_copy(name);
			const auto itif = this->find(calupper);
			return itif != this->end();
		}

		std::vector<std::string> CalendarProvider::allCalendars() const
		{
			std::vector<std::string> keys;
			keys.reserve(this->size());
			std::transform(this->begin(), this->end(), std::back_inserter(keys),
				[](const auto& pair) { return pair.first; });

			return keys;
		}
	   

	   CalendarConstPtr CalendarProvider::getCalendar(const std::string& name) const
	   {
		   // sometimes in the csv files you can have a leading blank in the name which can cause an issue
		   std::string str_name = name;
		   boost::trim(str_name);
		   const auto calupper = boost::to_upper_copy(str_name);
		   const auto itif = this->find(calupper);
		   if (itif != this->end())
			   return itif->second;

		   std::vector<std::string> calendars;
		   boost::split(calendars, calupper, boost::is_any_of("+"));
		   if (calendars.size() > 1)
		   {
			   std::vector<CalendarConstPtr> calvector;
			   for (const auto& cal : calendars)
			   {
				   const auto it = find(cal);
				   if (it != end())
					   calvector.emplace_back(it->second);
				   else
					   THROW("Cannot find calendar:" + cal);
			   }
				
			   return Calendar::merge(calvector);
			   //THROW("Cannot find calendar " + str_name);
			   
		   }
		   THROW("CalendarProvider:- calendar " + str_name + " not found");
		}

	   //TPSmartPointer<const Calendar> CalendarProvider::loadCalendar(const boost::filesystem::path& fullpath)

	}
}
