#pragma once

#include "DayCounter.h"

#include <memory>

namespace TP::date_time::day_count
{
	using date_time::Calendar;

	class CalendarBasedDayCounter;
	using CalendarBasedDayCounterPtr = std::shared_ptr<CalendarBasedDayCounter>;
	using CalendarBasedDayCounterConstPtr = std::shared_ptr<const CalendarBasedDayCounter>;

	class CalendarBasedDayCounter : public DayCounter
	{
	public:
		const Calendar& calendar() const { return *calendar_; }

	protected:
		explicit CalendarBasedDayCounter(const CalendarConstPtr& calendar)
			: calendar_(calendar)
		{
		}

	private:
		const CalendarConstPtr calendar_;
	};
}
