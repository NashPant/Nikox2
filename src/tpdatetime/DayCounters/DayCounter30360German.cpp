#include "DayCounter30360German.h"

using namespace TP::date_time::day_count;

double DayCounter30360German::calculateDcf(const Date& from, const Date& to, const OptDate& referenceStartDate, const OptDate& referenceEndDate) const
{
	const auto d1 = from == from.eom() ? 30d : from.day();
	const auto d2 = to == to.eom() ? 30d : to.day();

	const auto ym2 = to.year() / to.month();
	const auto ym1 = from.year() / from.month();
	return ((d2 - d1).count() + 30 * (ym2 - ym1).count()) / 360.0;
}
