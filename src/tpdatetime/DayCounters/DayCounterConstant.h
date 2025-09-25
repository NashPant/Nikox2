#pragma once


#include "DayCounter.h"
#include "TPTools/TPException.h"

namespace TP::date_time::day_count
{
	class DayCounterConstant : public DayCounter
	{
	public:
		explicit DayCounterConstant(const double dcf) : dcf_(dcf)
		{
			if (!dcf_)
				THROW("DayCounterConstant - must have valid input dcf");
		}


		double calculateDcf(const Date& from, const Date& to, const OptDate& referenceStartDate = std::nullopt, const OptDate& referenceEndDate = std::nullopt) const override
		{
			return dcf_;
		}

		std::string toString() const override {
			return "CONSTANT";
		}

		DayCounter::Code type() const override {
			return +DayCounter::Code::CONSTANT;
		}

	private:
		const double dcf_;
	};
}