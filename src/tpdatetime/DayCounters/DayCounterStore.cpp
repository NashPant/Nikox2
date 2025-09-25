#include "DayCounterStore.h"

namespace TP {
	namespace object_store
	{

		std::shared_ptr<const DayCounter> DayCounterStore::getDayCounter(const std::string& cal)
		{
			return getObject<const DayCounter>(cal);
		}
		void DayCounterStore::addDayCounter(const std::string& cal, std::shared_ptr<const DayCounter>& dc)
		{
			addObject(cal, dc);
		}


	}
}