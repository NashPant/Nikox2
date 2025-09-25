#pragma once

#include "DayCounter.h"

namespace TP::date_time::day_count
{
	class DayCounterNL365 : public DayCounter
	{
	public:
		double calculateDcf(const Date& from, const Date& to, const OptDate& referenceStartDate = std::nullopt, const OptDate& referenceEndDate = std::nullopt) const override;

		std::string toString() const override
		{
			return "NL365";
		}

		DayCounter::Code type() const override {
			return +DayCounter::Code::NL365;
		}
	};
}
