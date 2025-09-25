#include "DateConvention.h"
//#include "TPMarket/TemplateStoreCacheMemory.h"


namespace TP {
	namespace date_time {
		//DateConvention::DateConvention(
		//	const std::string& calendar_str,
		//	const unsigned int businessDayLag)
		//{
		//	const auto marketConfig = MarketConfigCacheMemory::find(TPIdentifier("ConfigCollection_Store"));
		//	calendar_ = marketConfig->getCalendarProvider()->getCalendar(calendar_str);
		//	businessDayLag_ = businessDayLag;
		//}

		std::map<std::string, TPAny> DateConvention::attributes(const std::optional<std::string> prefix) const
		{
			auto prefx = prefix.value_or("");
			std::map<std::string, TPAny> res{ {prefx+ "Calendar", TPAny(calendar_->getID())},
			{prefx+"lag", TPAny(int(businessDayLag_))} };
			
			return res;
		}

	}
}
