#pragma once

#include "CalendarBasedDayCounter.h"
#include "TPTools\Schedule\conventions\DateConvention.h"

#include "TPTools\Time\TPTenor.h"

namespace TP {
	namespace date_time {
		using date_time::Calendar;
		using TP::Core::Tenor;

		class TenorBasedDayCounter : public CalendarBasedDayCounter
		{
		public:
			const Tenor& tenor() const { return tenor_; }
			const DateConventionConstPtr& dateConvention() const {
				return dateConvention_	;			}

		protected:
			explicit TenorBasedDayCounter(const Tenor& tenor, const CalendarConstPtr& cal, const DateConventionConstPtr& dateconv):
				CalendarBasedDayCounter(cal),tenor_(tenor), dateConvention_(dateconv)
			{
			}
		private:

			const Tenor tenor_;
			const DateConventionConstPtr dateConvention_;
		};

	}
}


