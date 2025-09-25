#pragma once

#include "DayCounter.h"

namespace TP::date_time::day_count
{
	// NOTE: this is *NOT* the ACT/ACT BOND (which requires a more complicated implementation)
	class DayCounterActAct : public DayCounter
	{
	public:
		double calculateDcf(const Date& from, const Date& to, const OptDate& referenceStartDate = std::nullopt, const OptDate& referenceEndDate = std::nullopt) const override;

		std::string toString() const override
		{
			return "ACTACT";
		}

		DayCounter::Code type() const override {
			return +DayCounter::Code::ACTACT;
		}
	};

	class DayCounterCont : public DayCounter
	{
	public:
		double calculateDcf(const Date& from, const Date& to, const OptDate& referenceStartDate = std::nullopt, const OptDate& referenceEndDate = std::nullopt) const override;

		std::string toString() const override
		{
			return "CONT";
		}

		DayCounter::Code type() const override {
			return +DayCounter::Code::CONT;
		}
	};
}