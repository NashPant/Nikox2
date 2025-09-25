#include "FXDate.h"

#include "TPDateTime/CalendarProvider.h"
#include "TPDateTime/Tenor.h"
#include "Lags.h"

namespace TP::date_time
{

	class FxDateCalculator::Impl
	{
	public:
		Impl(CalendarProviderConstPtr&& prov, Calendar::Code&& ccy) :
			provider_(std::move(prov)),
			usd(std::move(ccy))
		{}
		Impl(const Impl& other) = default;
		Impl(Impl&& other) = default;
		Impl& operator=(const Impl& rhs) = default;
		Impl& operator=(Impl&& rhs) noexcept = default;
		virtual ~Impl() = default;

		[[nodiscard]]  DateAdjustmentConstPtr spot(const std::optional<BusinessDays>& lag = std::nullopt) const;

		CalendarProviderConstPtr provider_;
		Calendar::Code  usd;
	};

	/*DateAdjustmentConstPtr  FxDateCalculator::Impl::spot(const std::optional<BusinessDays>& lag ) const
	{

	}*/
}