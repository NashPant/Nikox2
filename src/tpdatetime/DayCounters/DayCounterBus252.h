#pragma once

#include "CalendarBasedDayCounter.h"

#include <fmt/format.h>
#include <fmt/ostream.h>

namespace TP::date_time::day_count
{
	class DayCounterBus252 final : public CalendarBasedDayCounter
	{
	public:
		explicit DayCounterBus252(const CalendarConstPtr& calendar)
			: CalendarBasedDayCounter{ calendar } {}

		[[nodiscard]] double calculateDcf(const Date& from, const Date& to, const OptDate& referenceStartDate = std::nullopt, const OptDate& referenceEndDate = std::nullopt) const override
		{
			return calendar().countBusinessDays(from, to).count() / 252.0;
		}

		[[nodiscard]] std::string toString() const override { return fmt::format("BUS252:{}", calendar().code().underlying()); }

		DayCounter::Code type() const override {
			return +DayCounter::Code::BUS252;
		}
	};
}
