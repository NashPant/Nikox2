#pragma once

#include "Tptools/objectstore/ObjectStore.h"
#include "TPDateTime\Calendar.h"

namespace TP {
	namespace object_store
	{
		using date_time::Calendar;
		using date_time::CalendarConstPtr;

		class TPNIKOPOLIS_IMPEXP CalendarStore: public ObjectStore<Calendar::Code, const Calendar>
		{
		public:
			static CalendarConstPtr getCalendar(const Calendar::Code& cal);
			static TP::Optional<CalendarConstPtr> maybeGetCalendar(const Calendar::Code& cal);

			static void addCalendar(const CalendarConstPtr&  cal);
			static void addCalendar(const std::string& calname, CalendarConstPtr  cal)
			{
				addObject(calname, cal.asNullable());
			}

			static void removeCalendar(const Calendar::Code& calname)
			{
				removeObject(calname);
			}
		};


	}
}