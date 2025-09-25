#include "DayCounterActAct.h"

using namespace TP::date_time;

namespace
{
	double denominator(const Year& y)
	{
		return y.is_leap() ? 366.0 : 365.0;
	}
}

double day_count::DayCounterActAct::calculateDcf(const Date& from, const Date& to, const OptDate& referenceStartDate, const OptDate& referenceEndDate) const
{
	const auto fromYmd = from.ymd();
	const auto toYmd = to.ymd();

	if (fromYmd.year() == toYmd.year())
		return (to - from).count() / denominator(fromYmd.year());

	return (Date(last / Dec / fromYmd.year()) - from + (from < to ? 1_D : 0_D)).count() / denominator(fromYmd.year())
		+ (toYmd.year() - fromYmd.year() - 1_Y).count()
		+ (to - 1d / Jan / toYmd.year() + (from >= to ? 1_D : 0_D)).count() / denominator(toYmd.year());
}

double day_count::DayCounterCont::calculateDcf(const Date& from, const Date& to, const OptDate& referenceStartDate, const OptDate& referenceEndDate) const
{
	return (to - from).count() /365.25;
}

