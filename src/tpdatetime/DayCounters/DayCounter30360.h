#pragma once

#include "DayCounter.h"

namespace TP::date_time::day_count
{
	class DayCounter30360 : public DayCounter
	{
	public:
		double calculateDcf(const Date& from, const Date& to, const OptDate& referenceStartDate = std::nullopt, const OptDate& referenceEndDate = std::nullopt) const override;

		std::string toString() const override
		{
			return "30360";
		}

		DayCounter::Code type() const override {
			return +DayCounter::Code::ISDA30360;
		}
	};
}