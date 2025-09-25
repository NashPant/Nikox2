#include "DayCounterNL365.h"

using namespace TP::date_time::day_count;

double DayCounterNL365::calculateDcf(const Date& from, const Date& to, const OptDate& referenceStartDate, const OptDate& referenceEndDate) const
{

	//Following start date and End date [d1,d2) counting convention;

	if (from.year() == to.year())
	{
		if (from.year().is_leap())
		{
			if (from <= to)
			{
				const auto totaldays = to > Date{ from.year(), Feb, 29d } && from <= Date{ from.year(), Feb, 29d } ? 1 : 0;
				return ((to - from).count() - totaldays) / 365.0;
			}
			else
			{
				const auto totaldays = from >= Date{ from.year(), Feb, 29d } && to < Date{ from.year(), Feb, 29d } ? 1 : 0;
				return ((to - from).count() + totaldays) / 365.0;
			}
		}
		return (to - from).count() / 365.00;
	}
	else
	{
		const auto yearStart = from < to ? Date{ from.year(), Dec, 31d } +1_D : Date{ from.year(), Jan, 1d } -1_D;

		const auto yearEnd = from < to ? Date{ to.year(), Jan, 1d } : Date{ to.year(), Dec, 31d };

		auto NumOfFeb29Start = 0;
		auto NumOfFeb29End = 0;

		if (from <= to)
		{
			if (from.year().is_leap())
			{
				NumOfFeb29Start = from > Date{ from.year(), Feb, 29d } ? 0 : 1;
			}
			if (to.year().is_leap())
			{
				NumOfFeb29End = to <= Date{ to.year(), Feb, 29d } ? 0 : 1;
			}
			return ((yearStart - from).count() + (yearEnd.year() - yearStart.year()).count() * 365.0 + (to - yearEnd).count() - NumOfFeb29Start - NumOfFeb29End) / 365.0;

		}
		else
		{
			if (to.year().is_leap())
			{
				NumOfFeb29End = to >= Date{ to.year(), Feb, 29d } ? 0 : 1;
			}
			if (from.year().is_leap())
			{
				NumOfFeb29Start = from < Date{ from.year(), Feb, 29d } ? 0 : 1;
			}

			return ((yearStart - from).count() + (yearEnd.year() - yearStart.year()).count() * 365.0 + (to - yearEnd).count() + NumOfFeb29Start + NumOfFeb29End) / 365.0;
		}
	}
}
