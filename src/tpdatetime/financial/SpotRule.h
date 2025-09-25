#pragma once

#include "TPTools/Utils/TypeDefs.h"
#include "TPDateTime/Calendar.h"
#include "Lags.h"
#include <memory>
#include "TPTools/Utils/NotNull.h"

namespace TP::date_time
{
	FWD_DECLARE_SHAREDPTR(CalendarProvider)

	TYPEDEF(Date, FixingDate)
	using PricingDate = FixingDate;
	using TradeDate = FixingDate;

	TYPEDEF(Date, SpotDate)
	using ValueDate = SpotDate;

	FWD_DECLARE_NOTNULLSHARED(SpotRule)

	class SpotRule
	{
	public:
		virtual ~SpotRule() = default;

		[[nodiscard]] virtual SpotDate toSpotDate(const FixingDate& fix) const = 0;
		[[nodiscard]] virtual FixingDate toFixingDate(const SpotDate& spot) const = 0;

		virtual BusinessDays lag() const= 0;

		static SpotRuleConstPtr fx(const CalendarProviderConstPtr& ref, const CurrencyLag& ccy, const Calendar::Code& code);

		static SpotRuleConstPtr fx(const CalendarProviderConstPtr& ref, const CurrencyLag& left, const CurrencyLag& right,
			const Calendar::Code& code);

	};

}