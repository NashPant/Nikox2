#pragma once

#include <algorithm>
#include <iostream>
#include <shared_mutex>
#include <string>
#include <unordered_map>
#include <vector>
//#include "TPTools\TPIdentifier.h"


namespace TP {
	namespace object_store
	{
		template<class KEY, class OBJECT>
		class ObjectStore
		{
		public:

			static size_t size()
			{
				auto& store = getStore();
				std::unique_lock<std::shared_mutex> lock(store.mutex);
				return store.map.size();
			}

			static void printKeys()
			{
				auto& store = getStore();
				std::unique_lock<std::shared_mutex> lock(store.mutex);
				std::for_each(store.map.begin(), store.map.end(), [](const Map::value_type& e)
				{
					std::cout << e.first << std::endl;
				});
			}

			static void setVersion(const std::string& v)
			{
				auto& store = getStore();
				std::unique_lock<std::shared_mutex> lock(store.mutex);
				store.version = v;
			}

			static std::string getVersion()
			{
				auto& store = getStore();
				std::unique_lock<std::shared_mutex> lock(store.mutex);
				return store.version;
			}

			static std::vector<std::string> getKeys()
			{
				auto& store = getStore();
				std::unique_lock<std::shared_mutex> lock(store.mutex);
				std::vector<std::string> res;
				for(const auto& v: store.map)
				{
					res.push_back(v.first);
				};
				return res;
			}

			static void clear()
			{
				auto& store = getStore();
				std::unique_lock<std::shared_mutex> lock(store.mutex);
				store.map.clear();
			}


		protected:
			// add and remove objects in the Store
			static void removeObject(const KEY& key)
			{
				auto& store = getStore();
				std::unique_lock<std::shared_mutex> lock(store.mutex);
				store.map.erase(key);
			}

			static void addObject(const KEY& key, const std::shared_ptr<OBJECT>& ob)
			{
				auto& store = getStore();
				std::unique_lock<std::shared_mutex> lock(store.mutex);
				store.map[key] = ob;
			}

			static std::vector<KEY> getAllKeys()
			{
				auto& store = getStore();
				std::unique_lock<std::shared_mutex> lock(store.mutex);
				std::vector<KEY> res;
				res.reserve(store.map.size());
				for (const auto& v : store.map)
				{
					res.push_back(v.first);
				};

				/*std::transform(store.map.cbegin(),store.map.cend(),std::back_inserter(res),
					[](const auto& it)
				{
					return it.first;
				});*/
				return res;
			}

		// dynamic cast to concrete object
		template<typename CONCRETE_OBJECT>
		static std::shared_ptr<CONCRETE_OBJECT> getObject(const KEY& key)
		{
			return std::dynamic_pointer_cast<CONCRETE_OBJECT>(getStoreObject(key));
		}

		private:
			using Map = std::unordered_map<KEY, std::shared_ptr<OBJECT>>;

			struct Store
			{
				Map map;
				std::shared_mutex mutex;
				std::string version;
			};

			static std::shared_ptr<OBJECT> getStoreObject(const KEY& key)
			{
				Store& store = getStore();
				std::shared_lock<std::shared_mutex>  lock(store.mutex);
				const auto i = store.map.find(key);
				//const std::shared_ptr<OBJECT> i = store.map.find(key);
				return i == store.map.end() ? nullptr : i->second;
			}

			static Store& getStore();

		};

	}
}
