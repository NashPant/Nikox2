#pragma once

#include "Tptools/objectstore/ObjectStore.h"
#include "TPDateTime\DayCounters\DayCounter.h"
#include <memory>

namespace TP {
	namespace object_store
	{
		using date_time::day_count::DayCounter;
	
		class DayCounterStore : public ObjectStore<std::string, const DayCounter>
		{
		public:

			static std::shared_ptr<const DayCounter> getDayCounter(const std::string& cal);
			static void addDayCounter(const std::string& cal, std::shared_ptr<const DayCounter>& dc);

		};

	}
}
