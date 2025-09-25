#pragma once

#include "CalendarBasedDayCounter.h"
#include "TPDateTime/Frequency.h"

namespace TP::date_time::day_count
{
	//using namespace schedule::convention;
	class DayCounterActActBond : public DayCounter
	{
	public:
		DayCounterActActBond(const Duration& period, DateConventionConstPtr dateConvention);

		double calculateDcf(const Date& from, const Date& to, const OptDate& referenceStartDate = std::nullopt, const OptDate& referenceEndDate = std::nullopt) const override;

		[[nodiscard]] std::string toString() const override;

		DayCounter::Code type() const override {
			return +DayCounter::Code::ACTACTBOND;
		}

	private:
		const Frequency frequency_;
		Duration tenor_;
		const DateConventionConstPtr dateConvention_;
	};

	class DayCounterUSWITActActBond : public DayCounter
	{
	public:
		DayCounterUSWITActActBond(const Duration& period, DateConventionConstPtr dateConvention);

		double calculateDcf(const Date& from, const Date& to, const OptDate& referenceStartDate = std::nullopt, const OptDate& referenceEndDate = std::nullopt) const override;

		[[nodiscard]] std::string toString() const override;

		DayCounter::Code type() const override {
			return +DayCounter::Code::USWITACTACT;
		}

	private:
		const Frequency frequency_;
		Duration tenor_;
		const DateConventionConstPtr dateConvention_;
	};
}
