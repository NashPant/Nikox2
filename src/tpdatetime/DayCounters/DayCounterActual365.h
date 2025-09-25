#pragma once

#include "DayCounter.h"

namespace TP::date_time::day_count
{
	class DayCounterAct365 : public DayCounter
	{
	public:
		double calculateDcf(const Date& from, const Date& to, const OptDate& referenceStartDate = std::nullopt, const OptDate& referenceEndDate = std::nullopt) const override
		{
			return (to - from).count() / 365.0;
		}

		std::string toString() const override {
			return "ACT365";
		}

		DayCounter::Code type() const override {
			return +DayCounter::Code::ACT365;
		}
	};
}
