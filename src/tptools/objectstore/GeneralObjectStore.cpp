#include "GeneralObjectStore.h"

namespace TP
{
	namespace object_store
	{
		void GeneralObjectStore::addObject(const std::string& key, const ObjectConstPtr& o)
		{
			TObjectStore::addObject(key, o);
		}

		void GeneralObjectStore::removeObject(const std::string& key)
		{
			TObjectStore::removeObject(key);
		}
	}
}