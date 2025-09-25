#pragma once
#include <TPNikopolisUtils.h>
#include "ObjectStore.h"
#include "Object.h"
#include <string>
#include <memory>


namespace TP 
{
	namespace object_store
	{
		using ObjectConstPtr = std::shared_ptr<const Object> ;
		using TObjectStore = ObjectStore<std::string, const Object>;

		class TPNIKOPOLIS_IMPEXP GeneralObjectStore :public TObjectStore
		{
		public:
			template<typename T>
			static std::shared_ptr<const T> getObject(const std::string& key)
			{
				return TObjectStore::getObject<const T>(key);
			}

			static void addObject(const std::string& key, const ObjectConstPtr& o);

			static void removeObject(const std::string& key);
		};


	}
}

