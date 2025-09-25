#include "DayCounter30E360.h"

using namespace TP::date_time::day_count;

double DayCounter30E360::calculateDcf(const Date& from, const Date& to, 
	const OptDate& referenceStartDate, const OptDate& referenceEndDate) const
{
	auto d1 = from.day();
	if (d1 == 31d)
	{
		d1 = 30d;
	}
	auto d2 = to.day();
	if (d2 == 31d)
	{
		d2 = 30d;
	}

	const auto ym2 = to.year() / to.month();
	const auto ym1 = from.year() / from.month();
	return ((d2 - d1).count() + 30 * (ym2 - ym1).count()) / 360.0;
}
