#include "ObjectStore.h"
#include "Object.h"
//#include "TPDateTime\Calendar.h"
//#include "TPDateTime\DayCounters\DayCounter.h"
//#include "TPIndex/IndexNew/Index.h"
#include "TPNikopolisUtils.h"

namespace TP {
	namespace object_store
	{
		//using namespace date_time;
		//using TP::Index::Index;

		template<>
		ObjectStore<std::string, const Object>::Store& ObjectStore<std::string, const Object>::getStore()
		{
			static Store map;
			return map;
		}

		//template<> 
		//ObjectStore<Calendar::Code, const Calendar>::Store& ObjectStore<Calendar::Code, const Calendar>::getStore()
		//{
		//	static Store map;
		//	return map;
		//}

		//template<>
		//ObjectStore<std::string, const day_count::DayCounter>::Store& ObjectStore<std::string, const day_count::DayCounter>::getStore()
		//{
		//	static Store map;
		//	return map;
		//}

		//template<>
		//ObjectStore<std::string, const Index>::Store& ObjectStore<std::string, const Index>::getStore()
		//{
		//	static Store map;
		//	return map;
		//}


	}
}

