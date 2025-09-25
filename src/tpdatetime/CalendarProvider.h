#pragma once

#include "Calendar.h"
#include <vector>
#include <map>
#include <unordered_map>
#include <boost\filesystem.hpp>
//#include "TPTools/TPSmartPointer.h"
#include <nlohmann/json.hpp>
#include "TPTools/TPException.h"
#include "TPDateTime/DateInterface.h"

namespace TP
{
	namespace date_time
	{
		class CalendarProvider;
		using CalendarProviderConstPtr = std::shared_ptr<const CalendarProvider>;

		class   CalendarProvider : public std::unordered_map<std::string, CalendarConstPtr>
		{
		public:
			/*
			  @brief. Note that we cannot TPNIKOPOLIS_IMPEXP the entire class. Some weird compilation errors since it inherits from
					  STL hash_map. It has something to do with the fact that CalendarConstPtr is notNull ptr and therefore has no default
					  constructor availble. You don't want to have a null calendar for sure but it restricts the class that it cannot be exported.
			*/
			bool  TPNIKOPOLIS_IMPEXP hasCalendar(const std::string& name) const;

			static  TPNIKOPOLIS_IMPEXP CalendarProviderConstPtr create(const std::vector<std::string>& names,
				const std::vector<CalendarConstPtr>& cals);

			static  TPNIKOPOLIS_IMPEXP CalendarProviderConstPtr create(const std::map<std::string,	CalendarConstPtr>& cals);


			CalendarConstPtr  TPNIKOPOLIS_IMPEXP  getCalendar(const std::string& name) const;
			
			 //static TPNIKOPOLIS_IMPEXP TPSmartPointer<const Calendar> loadCalendar(const boost::filesystem::path& fullpath);

			std::vector<std::string> TPNIKOPOLIS_IMPEXP allCalendars() const;
		private:
			CalendarProvider() = default;
			

		};

		/*
	   @brief. Static memeber function to load a calendar from the conf directory.
			   It assumes that the json file has the right structure, i.e.
			   "DAY_OF_WEEKS"
			   "HOLIDAYS"
			   "CITY"
		*/
		template<typename  T>
		static  T loadCalendar(const boost::filesystem::path& fullpath)
		{
			nlohmann::json j;

			std::ifstream readfile;
			readfile.open(fullpath.string());
			if (!readfile.is_open())
				THROW("Cannot open calendar file in path:" + fullpath.string());

			j = nlohmann::json::parse(readfile);
			auto hols = j.at("HOLIDAYS");
			std::set<Date> holidays;
			for (const auto& hh : hols)
			{
				auto d = hh.get<std::string>();
				TPDate dt;
				auto r = TPDate::fromString(d, dt);
				if (r)
					holidays.emplace(Date(dt)); // constructor will take care of transformation from TPDate to hinnant Date
			}

			auto DAY_OF_WEEKS = j.at("DAY_OF_WEEKS");
			std::vector<Weekday> weekend;
			for (const auto& d : DAY_OF_WEEKS)
			{
				auto d_str = d.get<std::string>();
				weekend.emplace_back(IMMHelper::parseWeekday(d_str));
			}
			auto city = j.at("CITY").get<std::string>();
			//return Calendar::construct_nikoptr(Calendar::Code(city), holidays, weekend);
			return Calendar::constructT<T>(Calendar::Code(city), holidays, weekend);
		}
	}
}
