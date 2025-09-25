#pragma once
#include "TPDateTime/durations.h"
#include "TPDateTime/Calendar.h"

namespace TP::date_time
{
	struct BusinessDaysLag
	{
		BusinessDaysLag( BusinessDays l, Calendar::Code  c) :
			lag(std::move(l)),
			calendar(std::move(c))
		{}

		BusinessDays const lag;
		Calendar::Code const calendar;
	};

	struct DurationLag
	{
		DurationLag(Duration d, Calendar::Code  c, bool b) :
			lag(std::move(d)),
			calendar(std::move(c)),
			eom(b)
		{}

		Duration const lag;
		Calendar::Code const calendar;
		bool const eom;// preserves the endof month
	};

	struct CurrencyLag
	{
		CurrencyLag(BusinessDays l, Calendar::Code  c, bool b=false) :
			lag(std::move(l)),
			calendar(std::move(c)),
			isLatam(b)
		{}
		CurrencyLag() :
			lag(0_B),
			calendar("NONE"),
			isLatam(false)
		{}

		BusinessDays const lag;
		Calendar::Code const calendar;
		bool const isLatam; // speical LatAm currency

		CurrencyLag withLag(const BusinessDays l) const {
			return { l,calendar, isLatam };		}
	};
}