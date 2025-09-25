#pragma once

#include <TPNikopolisUtils.h>
#include "TPTools/Time/TPDate.h"
//#include "TPDateTime/Cut.h"
#include "TPDateTime/YearMonthDay.h"
#include "TPDateTime/Durations.h"
#include "TPDateTime/Date.h"

namespace TP::date_time
{
	namespace day_count
	{
		class DayCounter;
	}

	/*
	  @brief.  HElper class to export the iterators because for whatever reason i cannot export
				  classes designed with CRTP above
	*/
	class TPNIKOPOLIS_IMPEXP IMMHelper final
	{
	public:
		IMMHelper(const TPDate& asof) :asof_(asof)
		{}

		
		TPDate nextIMMDate(const Months& duration, const WeekdayIndexed& weekdayIndex) const;
		TPDate prevIMMDate(const Months& duration, const WeekdayIndexed& weekdayIndex) const;

		bool isIMMDate(const Months& duration, const WeekdayIndexed& weekdayIndex);
	//	static TPDate toTPDate(const Date& asof)
	//	{
	//		return TPDate(asof.toString("%Y-%m-%d"));
	//	}
		static Weekday parseWeekday(const std::string& weekstr);
		static Date nextWeekday(const Date& asof, const std::string& whichDay);
		static Date nextWeekday(const Date& asof, const Weekday& whichDay);
		static Date prevWeekday(const Date& asof, const std::string& whichDay);
		static Date prevWeekday(const Date& asof, const Weekday& whichDay);
		//
		/*
		 *  returns the week that the day corresponnds to, ie. week1, week2, week3, week4 or week5
		 */
		static size_t whichWeek(const Date& asof);

	private:
		TPDate  asof_;
	};

	/*
	 * @brief. We have to generalise the way we compute day count fraction between the expiry date + Cut and now.
	 */
	//TPNIKOPOLIS_IMPEXP double timeDayCount(const Date& expiry, const Cut& when, const day_count::DayCounter& dc, 
	//	const std::optional<Date>& asof = std::nullopt);
}