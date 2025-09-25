#include "DayCounter30360.h"

using namespace TP::date_time::day_count;

double DayCounter30360::calculateDcf(const Date& from, const Date& to, const OptDate& referenceStartDate, const OptDate& referenceEndDate) const
{
	auto d1 = from.day();
	if (d1 == 31d)
		d1 = 30d;
	auto d2 = to.day();
	if (d2 == 31d && d1 == 30d)
		d2 = 30d;

	return static_cast<double>((d2 - d1).count() + 30 * (to.ym() - from.ym()).count()) / 360.0;
}

