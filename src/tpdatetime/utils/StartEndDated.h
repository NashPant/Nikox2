#pragma once
#include "TPDateTime/Tenor.h"
#include "TPDateTime/Date.h"

namespace TP::date_time
{
	class  StartEndDated
	{
		virtual ~StartEndDated() = default;

		StartEndDated(const StartEndDated& rhs) = default;
		StartEndDated(StartEndDated&& rhs) noexcept= delete;

		StartEndDated& operator= (const StartEndDated& rhs) = delete;
		StartEndDated& operator = (StartEndDated&& rhs) = delete;


	protected:
		StartEndDated(const OptDate& startD,
			const OptTenor& startP,
			const OptDate& endD,
			const OptTenor& endP):
		optionalStartDate_(startD),
		optionalStartTenor_(startP),
		optionalEndDate_(endD),
		optionalEndTenor_(endP)
		{}

		Date estimateEndDate(const Date& asof)
		{
			if (optionalEndDate_)
				return *optionalEndDate_;

			auto startDate = optionalStartDate_ ? *optionalStartDate_ : asof;
			if (optionalStartTenor_)
				startDate = startDate.add(Duration(*optionalStartTenor_));

			return startDate.add(Duration(*optionalEndTenor_));
		}
	private:
		const OptDate optionalStartDate_;
		const OptTenor optionalStartTenor_;
		const OptDate optionalEndDate_;
		const OptTenor optionalEndTenor_;
	};

}