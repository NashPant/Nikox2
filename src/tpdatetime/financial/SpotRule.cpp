#include "SpotRule.h"

#include "TPTools/TPException.h"
#include "TPDateTime/financial/DateAdjustment.h"

#include <fmt/format.h>

namespace TP::date_time
{
	class FxSpotRule final : public SpotRule
	{
	public:
		FxSpotRule(DateAdjustmentConstPtr adj, const BusinessDays& lag) :
			spotDateAdjustment_(std::move(adj)),
			lag_(lag)
		{}

		[[nodiscard]] BusinessDays lag() const override { return lag_; }
		
		[[nodiscard]]  SpotDate toSpotDate(const FixingDate& fix) const override
		{
			return spotDateAdjustment_->adjust(static_cast<Date>(fix));
		}
		[[nodiscard]] FixingDate toFixingDate(const SpotDate& spot) const override
		{
			const static auto weekdcal = Calendar::isoWeekday();
			const auto spotdate = static_cast<Date>(spot);
			auto fix = weekdcal->shift(spotdate, -lag_);
			auto d = spotDateAdjustment_->adjust(fix);
			while (d > spotdate)
			{
				fix = weekdcal->previous(fix);
				d = spotDateAdjustment_->adjust(fix);
			}
			if (d != spotdate)
				THROW(fmt::format("invalid fx spot date {}. Cannot find fixing date to result in given spot date", spotdate.toString()));

			return fix;
		}

	private:
		DateAdjustmentConstPtr spotDateAdjustment_;
		BusinessDays lag_;
	};

	SpotRuleConstPtr fx(const CalendarProviderConstPtr& ref, const CurrencyLag& ccy,
		const Calendar::Code& code)
	{
		return utils::makeNotNullShared<FxSpotRule>(DateAdjustment::fxSpot(ref, ccy, code), ccy.lag);
	}

	static SpotRuleConstPtr fx(const CalendarProviderConstPtr& ref, const CurrencyLag& left, const CurrencyLag& right,
		const Calendar::Code& code);

}