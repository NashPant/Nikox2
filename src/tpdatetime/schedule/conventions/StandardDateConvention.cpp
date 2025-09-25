#include "StandardDateConvention.h"


namespace TP {
	namespace date_time {


		Date StandardDateConvention::rollDate(
			const Date& date,
			const CalendarConstPtr& calendar,
			const BusinessDayConvention& rollConvention,
			unsigned int businessDayLag
		)
		{
			switch (rollConvention)
			{
			case BusinessDayConvention::None:
				return date + Days{ businessDayLag };
			case BusinessDayConvention::F:
				return calendar->businessDateAfter(date, businessDayLag);
			case BusinessDayConvention::P:
				return calendar->businessDateBefore(date, businessDayLag);
			case BusinessDayConvention::MF:
			{
				const auto businessDateAfter = calendar->businessDateAfter(date, businessDayLag);

				return businessDateAfter.month() == date.month()
					? businessDateAfter
					//: calendar->businessDateBefore(TPDate{ date.dayOfMonthEnd() , date.getMonth(),  date.getYear() }, 0U);
					: calendar->lastBusinessDay(date.ym());
			}
			case BusinessDayConvention::MP:
			{
				const auto businessDateBefore = calendar->businessDateBefore(date, businessDayLag);

				return businessDateBefore.month() == date.month()
					? businessDateBefore
					//: calendar->businessDateAfter(TPDate{ 1U,  date.getMonth(), date.getYear() }, 0U);
					: calendar->firstBusinessDay(date.ym());
			}
			default:
				THROW("TPRolling [" + rollConvention.toString() + "] is not supported");
			}
		}

		StandardDateConventionConstPtr StandardDateConvention::createStandardOptionConvention()
		{
			auto week_cal = Calendar::isoWeekday();
			auto mf = BusinessDayConvention("MF");
			return std::make_shared<const StandardDateConvention>(week_cal, mf, 0);
		}


		std::map<std::string, TPAny> StandardDateConvention::attributes(const std::optional<std::string> prefix ) const
		{
			auto pre = prefix.value_or("");
			std::map<std::string, TPAny> res= DateConvention::attributes(prefix);
			res[pre + "rolling"] = TPAny(rollConvention_.toString());
			return res;

		}
	}
}