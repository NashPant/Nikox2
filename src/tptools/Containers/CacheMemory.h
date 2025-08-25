#pragma once
/*
 @brief. New Cache memory object based on shared_ptr
*/

/////////////////////////////////////////////////////////////////////////////
//
//  This program is the CONFIDENTIAL and PROPRIETARY property of'
//   Athanasios Pantas. Unauthorized use and reproduction'
//  of this program is strictly prohibited.
//
// Copyright (C) 2003  - Nikopolis
//  Copyright (C) 2003 - Athanasios Pantas.
//  ALL RIGHTS RESERVED.
/////////////////////////////////////////////////////////////////////////////

#include "TPTools/TPIdentifier.h"
#include "TPTools/TPException.h"
#include <fmt/format.h>
#include <map>
#include <memory>
#include <shared_mutex>
#include <string>
#include <vector>

#ifndef TPSTATIC
#ifndef CACHE_MEMORY_API
#define CACHE_MEMORY_API __declspec(dllimport)
#endif
#else
#define CACHE_MEMORY_API 
#endif

namespace TP
{
	template <typename T>
	class CacheMemory
	{
	public:

		static TPIdentifier insert(const std::shared_ptr<const T>& tobeinserted, const TPIdentifier& id);
		static TPIdentifier save(const std::shared_ptr<const T>& tobeinserted, const TPIdentifier& id);
		static const std::shared_ptr<const T>& find(const TPIdentifier& id);
		static void erase(const TPIdentifier& id);
		static void clear();

		using cachememorymap_t = typename std::map <TPIdentifier, std::shared_ptr<const T> >;
		using const_iterator= typename cachememorymap_t::const_iterator;
		using iterator = typename cachememorymap_t::iterator;

		static const_iterator begin() { return cachemap().begin(); }
		static const_iterator end() { return cachemap().end(); }

		// return all the Keys in the map
		static  std::vector<TPIdentifier>  AllKeys();

		virtual ~CacheMemory() = default;

		// Initialise cache memory
		static void initCachememory()
		{
			if (!myCacheMap)
			{
				//	 myCacheMap = new cachememorymap_t();
				myCacheMap = std::make_shared<cachememorymap_t>();
			}
		}
	private:
		// Accessor to the cache map
		static cachememorymap_t& cachemap()
		{
			if (!myCacheMap)
			{
				myCacheMap = std::make_shared<cachememorymap_t>();
			}
			return *myCacheMap;
		}

		//this is a singleton
		CacheMemory() {}

		//common static null pointer for everyone to refer to
		CACHE_MEMORY_API static std::shared_ptr<const T>	null_;

		//iterate internally over non const
		typedef __typename cachememorymap_t::iterator							      iterator;

		//our map
		CACHE_MEMORY_API static	std::shared_ptr<cachememorymap_t> myCacheMap;
	};

	template <typename T>
	TPIdentifier CacheMemory<T>::insert(const std::shared_ptr<const T>& tobeinserted, const TPIdentifier& id)
	{
		//cachemap().insert(cachememorymap_t::value_type(id, tobeinserted));
		cachemap().insert({ id,tobeinserted });
		return id;
	}

	//============================================
	// Returns all the Keys in the map
	//=======================================
	template <typename T>
	std::vector<TPIdentifier> CacheMemory<T>::AllKeys()
	{

		std::vector<TPIdentifier> myres;
		iterator iter = cachemap().begin();
		for (; iter != cachemap().end(); iter++)
		{
			myres.push_back((*iter).first);
		}
		return myres;
	}

	template <typename T>
	TPIdentifier CacheMemory<T>::save(const std::shared_ptr<const T>& tobeinserted, const TPIdentifier& id)
	{
		iterator iter = cachemap().find(id);

		TPIdentifier retID = id;

		if (iter != cachemap().end())
		{
			retID = (*iter).first.nextVersion();
			cachemap().erase(id);
		}

		return insert(tobeinserted, retID);
	}

	template <typename T>
	const std::shared_ptr<const T>& CacheMemory<T>::find(const TPIdentifier& id)
	{

		//std::shared_ptr<const T>  *ret = &null_;

		iterator iter = cachemap().find(id);

		if (iter != cachemap().end())
		{
			return iter->second;
		}
		else
			return null_;
	}

	template <typename T>
	void CacheMemory<T>::erase(const TPIdentifier& id)
	{
		//std::shared_ptr<const T> ret = find(id);
		cachemap().erase(id);
	}

	template <typename T>
	void CacheMemory<T>::clear()
	{
		cachemap().clear();
	}
}