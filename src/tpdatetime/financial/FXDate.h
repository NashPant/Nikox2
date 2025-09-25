#pragma once


#include "TPDateTime/Date.h"
#include "DateAdjustment.h"
//#include 

namespace TP::date_time
{
	class CalendarProvider;
	using CalendarProviderConstPtr = std::shared_ptr<const CalendarProvider>;
	struct CurrencyLag;

	/*
	 *  Basic Class to calculate Dates required for various FX rules, merging of calendars, etc
	*/
	class FxDateCalculator final
	{
	public:
		/*
		  @brief. Construct USD cross
		*/
		FxDateCalculator(const CalendarProviderConstPtr& provider, const CurrencyLag& ccy, const Calendar::Code& usd);

		~FxDateCalculator() {}
	private:
		class Impl;
		class USD;
		class Cross;

	};
}