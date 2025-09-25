#include "DayCounter.h"

namespace TP::date_time::day_count
{
	class DayCounterTBillsACT365 : public DayCounter
	{
	public:
		double calculateDcf(const Date& from, const Date& to, const OptDate& referenceStartDate = std::nullopt, const OptDate& referenceEndDate = std::nullopt) const override;

		std::string toString() const override
		{
			return "TBILLSACT365";
		}

		DayCounter::Code type() const override {
			return +DayCounter::Code::TBILLSACT365;
		}
	};
}
