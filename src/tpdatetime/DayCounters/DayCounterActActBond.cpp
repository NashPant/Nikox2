#include "DayCounterActActBond.h"

#include "TPTools/TPException.h"
#include "TPDateTime/schedule/conventions/StandardDateConvention.h"

#include <fmt/format.h>
#include <fmt/ostream.h>

#include <utility>

namespace TP::date_time::day_count
{
	//using namespace schedule::convention;

	DayCounterActActBond::DayCounterActActBond(const Duration& period, DateConventionConstPtr dateConvention)
		: frequency_(period), tenor_(period), dateConvention_(std::move(dateConvention))
	{
	}

	double DayCounterActActBond::calculateDcf(const Date& from, const Date& to, const OptDate& referenceStartDate, const OptDate& referenceEndDate) const
	{
		if(!referenceEndDate.hasValue())
			THROW("DayCounterActActBond: day count calculation requires a reference period end date");
		auto end = referenceEndDate.value();
		//auto end = referenceEndDate
		//	.valueOrThrow<TPException>("DayCounterActActBond: day count calculation requires a reference period end date");

		const auto start = end - tenor_;
		end = dateConvention_->rollDate(end);
		const auto basis = static_cast<double>((end - start).count());
		const auto period = static_cast<double>((to - from).count());
		return period / basis / frequency_.count();
	}

	std::string DayCounterActActBond::toString() const
	{
		return fmt::format("ACTACTBOND: {}:{}", frequency_.toDuration(), dateConvention_->calendar()->code().underlying());
	}

	DayCounterUSWITActActBond::DayCounterUSWITActActBond(const Duration& period, DateConventionConstPtr dateConvention)
		: frequency_(period), tenor_(period), dateConvention_(std::move(dateConvention))
	{
	}

	double DayCounterUSWITActActBond::calculateDcf(const Date& from, const Date& to, const OptDate& referenceStartDate, const OptDate& referenceEndDate) const
	{
		if (!referenceEndDate.hasValue())
			THROW("DayCounterActActBond: day count calculation requires a reference period end date");
		auto end = referenceEndDate.value();

		//auto end = referenceEndDate
		//	.valueOrThrow<TPException>("DayCounterActActBond: day count calculation requires a reference period end date");

		const auto start = end - tenor_;
		end = dateConvention_->rollDate(end);
		const auto basis = static_cast<double>((end - start).count());
		const auto period = static_cast<double>((to - from).count());
		const auto dcf = period / basis / frequency_.count();
		return std::round(dcf*std::pow(10, 6)) / std::pow(10, 6);
	}

	std::string DayCounterUSWITActActBond::toString() const
	{
		return fmt::format("USWITACTACTBOND: {}:{}", frequency_.toDuration(), dateConvention_->calendar()->code().underlying());
	}

}
