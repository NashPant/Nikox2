#include "DateInterface.h"

#include "Time.h"
#include "DayCounters/DayCounter.h"
#include "TPDateTime/IMMIterators.h"


namespace TP::date_time
{
	TPDate IMMHelper::nextIMMDate(const Months& duration, const WeekdayIndexed& weekdayIndex) const
	{
		auto it = IMMIterator(asof_, duration, weekdayIndex);
		return *++it;
	}

	TPDate IMMHelper::prevIMMDate(const Months& duration, const WeekdayIndexed& weekdayIndex) const
	{
		auto it = IMMIterator(asof_, duration, weekdayIndex);
		return *--it;
	}

	Weekday IMMHelper::parseWeekday(const std::string& weekstr)
	{
		Formatter<Weekday> weekfmter{};
		return weekfmter.tryParse(weekstr).value();
	}

	bool IMMHelper::isIMMDate(const Months& duration, const WeekdayIndexed& weekdayIndex)
	{
		auto it = IMMIterator(asof_, duration, weekdayIndex);
		return it.isIMMDate(asof_);
	}


	Date IMMHelper::nextWeekday(const Date& asof, const Weekday& whichDay)
	{
		if (asof.weekday() == whichDay)
			return asof + Days{ 7 };
		else
		{
			auto d1 = asof + Days{ 1 };
			while (d1.weekday() != whichDay)
			{
				d1 += Days{ 1 };
			}
			return d1;
		}
	}

	Date IMMHelper::nextWeekday(const Date& asof, const std::string& weekstr)
	{
		auto whichDay = parseWeekday(weekstr);
		if (asof.weekday() == whichDay)
			return asof + Days{ 7 };
		else
		{
			auto d1 = asof + Days{ 1 };
			while (d1.weekday() != whichDay)
			{
				d1 += Days{ 1 };
			}
			return d1;
		}
	}

	Date IMMHelper::prevWeekday(const Date& asof, const Weekday& whichDay)
	{
		if (asof.weekday() == whichDay)
			return asof - Days{ 7 };
		else
		{
			auto d1 = asof - Days{ 1 };
			while (d1.weekday() != whichDay)
			{
				d1 -= Days{ 1 };
			}
			return d1;
		}
	}

	Date IMMHelper::prevWeekday(const Date& asof, const std::string& weekstr)
	{
		auto whichDay = parseWeekday(weekstr);
		return prevWeekday(asof, whichDay);
	}

	
	//double timeDayCount(const Date& expiry, const Cut& when, const day_count::DayCounter& dc,
	//	const std::optional<Date>& asof)
	//{
	//	Date today;
	//	if (asof == std::nullopt)
	//		today = Date::sysToday();
	//	else
	//		today = asof.value();
	//	TP_REQUIRE(expiry >= today, "Expecting Expiry date to be greater than equal to today");
	//	auto dcf_days = dc.calculateDcf(today, expiry);
	//	Cut start_of_day(when.tz, Minutes{ 0 });
	//	Time t1(expiry, when);
	//	Time t2(expiry, start_of_day);
	//	return dcf_days + t1.dcf() - t2.dcf();
	//}

	size_t IMMHelper::whichWeek(const Date& asof)
	{
		auto w = asof.weekday();
		Date d2(YearMonthWeekday(asof.year(), asof.month(), WeekdayIndexed(w, 1)));
		size_t which_week = 0;

		size_t i = 1;
		while (i < 6) // max week
		{
			if (d2 == asof)
			{
				which_week = i;
				break;
			}
			d2 = IMMHelper::nextWeekday(d2, w);
			i++;
		}

		TP_REQUIRE(which_week > 0, "Cannot find the week in the given date " + asof.toString());
		return which_week;
	}
}