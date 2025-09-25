#include "CalendarStore.h"
#include <boost\algorithm\string.hpp>
#include <vector>
#include "TPTools/TPException.h"

namespace TP {
	namespace object_store
	{

		TP::Optional<CalendarConstPtr> CalendarStore::maybeGetCalendar(const Calendar::Code& code)
		{
			//const auto caltouse = boost::to_upper_copy(cal);
			const auto result = getObject<const Calendar>(code.underlying());
			if (result != nullptr)
				return CalendarConstPtr(result);

			std::vector<std::string> calendars;
			boost::split(calendars, code.underlying(), boost::is_any_of("+"));
			//if (calendars.size() == 1)
			//{
			//	return nullptr;
			//}

			std::vector<CalendarConstPtr> calVector;
			for (const auto& cal : calendars)
			{
				const auto calendar = getObject<const Calendar>(cal);
				if (!calendar)
					return std::nullopt;
				calVector.push_back(CalendarConstPtr(calendar));
			}

			const auto mergedCdr = Calendar::merge(calVector);
			//CalendarConstPtr calendarptr = std::make_shared<const Calendar>(calVector);

			addCalendar(mergedCdr);

			return mergedCdr;
		}

		CalendarConstPtr CalendarStore::getCalendar(const Calendar::Code& cal)
		{
			auto calendar = maybeGetCalendar(cal);
			//	.valueOrThrow<TPException>("Missing Calendar [" + cal.underlying() + "] in CalendarStore");
			if (!calendar.hasValue())
				THROW("Missing Calendar [" + cal.underlying() + "] in CalendarStore");

			return calendar.value();
		}

		void CalendarStore::addCalendar(const CalendarConstPtr&  cal)
		{
			addObject(cal->code(), cal.asNullable());
		}

	}
}