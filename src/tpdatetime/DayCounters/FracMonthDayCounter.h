#pragma once

#include "DayCounter.h"
#include "TPDateTime/Date.h"
#include "TPTools/TPException.h"

#include <string>
#include <optional>

namespace TP::date_time::day_count
{
	class DayCounterFracMonth : public DayCounter
	{
	public:
		explicit DayCounterFracMonth(DateConventionConstPtr dateConvention) : dateConvention_(std::move(dateConvention))
		{
			if (!dateConvention_)
				THROW("DayCounterFracMonth - must have valid date convention");
		}

		double calculateDcf(const Date& from, const Date& to, const OptDate& referenceStartDate = std::nullopt, const OptDate& referenceEndDate = std::nullopt) const override
		{
			const auto start = dateConvention_->rollDate(from);
			const auto end = dateConvention_->rollDate(to);

			auto months = (end.ym() - start.ym());
			const auto d = dateConvention_->rollDate(start.add(months, true));

			double frac = months.count();

			if (d != end)
			{
				if (d > end)
					--months;

				const auto lower = (d > end) ? dateConvention_->rollDate(start.add(months - 1_M, true)) : d;
				const auto upper = (d > end) ? d : dateConvention_->rollDate(start.add(months + 1_M, true));

				frac += static_cast<double>((end - lower).count()) / (upper - lower).count();
			}

			return frac;
		}

		std::string toString() const override
		{
			return name;
		}

		DayCounter::Code type() const override {
			return +DayCounter::Code::FRACMONTH;
		}

		inline static std::string name = "FRACMONTH";

	private:
		const DateConventionConstPtr dateConvention_;
	};
}
