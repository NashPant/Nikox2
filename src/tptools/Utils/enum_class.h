/*
* @brief. Wrapper around Enum class to allow easy fromString and toString operations.
*
*/

/////////////////////////////////////////////////////////////////////////////
//
//  This program is the  property of           
//   Athanasios Pantas. Feel free to use, no guarantees from me.
//	                  
//
//	Copyright (c) 2003- Athanasios Pantas.
//	ALL RIGHTS RESERVED.                                                 
/////////////////////////////////////////////////////////////////////////////
#pragma once

#include <vector>
#include <optional>
#include <string>
#include <set>
#include <map>
#include "TPTools/TPException.h"
#include <string>
#include <ranges>
#include <iterator>
#include "TPTools/StringUtils/TPStringUtils.h"
#include <type_traits>


namespace TP
{
	// We enforce case insensitive search here, otherwise it becomes very complicated.
	using CaseInsensitiveLess = TP::stringutils::ILess;
	// The core of our reflective enum implementation
	template <typename EnumType>
	class TPEnum final
	{
	public:
		TPEnum() : value() {}
		TPEnum(EnumType const& e) : value(e) {}

		std::enable_if_t<std::is_enum<EnumType>::value, EnumType> value;

		operator EnumType& ()
		{
			return value;
		}

		operator EnumType const& () const
		{
			return value;
		}

		size_t hash() const { return static_cast<size_t>(value); }
		bool operator==(const TPEnum<EnumType>& rhs) const { return value == rhs.value; }

		static std::string toString(const EnumType e);
		static std::vector<std::string> toAllStrings(const EnumType e);
		static EnumType fromString(const std::string& s);
		static bool fromString(const std::string& s, EnumType& e) noexcept(true);
		static const std::set<std::string>& enumSymbols();
		static const std::set<int>& enumRange();

		template <class Archive>
		inline std::string save_minimal(Archive const&) const
		{
			return toString(value);
		}

		template <class Archive>
		inline void load_minimal(Archive const&, std::string const& str)
		{
			value = fromString(str);
		}

	private:
		// typedef typename std::vector< std::pair< std::string, EnumType > > SymbolVector;
		using SymbolVector = std::vector< std::pair< std::string, EnumType > >;
		static const SymbolVector symbolVector_;
	};

	template <class EnumType>
	TPEnum<EnumType> make_enum(EnumType const& e)
	{
		return e;
	}

	struct EnumClassHash
	{
		template<typename EnumType>
		std::size_t operator()(EnumType const& t) const
		{
			return static_cast<std::size_t>(t);
		}
	};

	// Implementation
	template <typename K, typename V, typename Pr, typename Alloc>
	std::map<V, K> reverseMap(const std::map<K, V, Pr, Alloc>& m)
	{
		std::map<V, K> rm;
		for (const auto& x : m)
			rm[x.second] = x.first;
		return rm;
	}

	template <typename K, typename V, typename Pr, typename Alloc>
	std::map<V, std::vector<K>> reverseMultMap(const std::map<K, V, Pr, Alloc>& m)
	{
		std::map<V, std::vector<K>> rm;
		for (const auto& x : m)
			rm[x.second].emplace_back(x.first);
		return rm;
	}


	template <typename A, typename B>
	std::map<A, B, CaseInsensitiveLess> forwardMap(const std::vector<std::pair<A, B>>& v)
	{
		std::map<A, B, CaseInsensitiveLess> m;
		for (const auto& x : v)
			m[x.first] = x.second;
		return m;
	}

	template <typename A, typename B>
	std::map<B, A> reverseMap(const std::vector<std::pair<A, B>>& v)
	{
		std::map<B, A> m;
		for (const auto& x : std::views::reverse(v)) {
			m[x.second] = x.first;
		}
		return m;
	}

	template <typename EnumType>
	std::string TPEnum<EnumType>::toString(const EnumType e)
	{
		static auto enumToStringMap = reverseMap(symbolVector_);
		auto it = enumToStringMap.find(e);
		if (it != enumToStringMap.end())
			return it->second;
		THROW_MISSING_VALUE_EXCEPTION("Cannot convert enum to a string.");
	}

	template <typename EnumType>
	std::vector<std::string> TPEnum<EnumType>::toAllStrings(const EnumType e)
	{
		std::vector<std::string> res;
		for (const auto& it : symbolVector_)
		{
			if (it.second == e)
				res.emplace_back(it.first);
		}
		return res;
		/*static auto enumToStringMap = reverseMultMap(symbolVector_);
		auto it = enumToStringMap.find(e);
		if (it != enumToStringMap.end())
			return it->second;
		THROW_MISSING_VALUE_EXCEPTION("Cannot convert enum to a string.");*/
	}

	template <typename EnumType>
	bool TPEnum<EnumType>::fromString(const std::string& s, EnumType& e) noexcept(true)
	{
		static auto stringToEnumMap = forwardMap(symbolVector_);
		auto upper = TP::stringutils::trim_copy_std(s); //boost::trim_copy(s);
		auto it = stringToEnumMap.find(upper);
		if (it != stringToEnumMap.end())
		{
			e = it->second;
			return true;
		}
		return false;
	}

	template <typename EnumType>
	EnumType TPEnum<EnumType>::fromString(const std::string& s)
	{
		EnumType e;
		if (!fromString(s, e))
		{
			
			std::string result = TP::stringutils::join(enumSymbols());

			THROW_MISSING_VALUE_EXCEPTION(
				//"Cannot convert string [" + s + "] to a " + typeid(EnumType) +
				"Cannot convert string [" + s + "] to a enum" +
				". Valid values are: " + result);
			
		}
		return e;
			/*THROW_MISSING_VALUE_EXCEPTION(
				"Cannot convert string [" + s + "] to a " + stripTypeName(typeid(EnumType)) +
				". Valid values are: " + boost::algorithm::join(enumSymbols(), ", "));*/
		
	}

	template <typename TK, typename TV>
	std::set<TK> mapKeys(std::vector<std::pair<TK, TV>> const& v)
	{
		std::set<TK> retval;
		for (auto const& element : v)
		{
			retval.insert(element.first);
		}
		return retval;
	}

	template <typename TK, typename TV>
	std::set<int> mapValues(std::vector<std::pair<TK, TV>> const& v)
	{
		std::set<int> retval;
		for (auto const& element : v)
		{
			retval.insert(static_cast<int>(element.second));
		}
		return retval;
	}

	template <typename EnumType>
	const std::set<std::string>& TPEnum<EnumType>::enumSymbols()
	{
		static auto enumSymbols = mapKeys(symbolVector_);
		return enumSymbols;
	}

	template <typename EnumType>
	const std::set<int>& TPEnum<EnumType>::enumRange()
	{
		static auto enumSymbols = mapValues(symbolVector_);
		return enumSymbols;
	}
	//==================================
	// Generic Functions to and from String
	//=========================================
	namespace Core
	{
		template <typename E>
		constexpr auto to_underlying(E e) noexcept
		{
			return static_cast<std::underlying_type_t<E>>(e);
		}


		template <typename EnumType>
		typename std::enable_if_t<std::is_enum<EnumType>::value, EnumType>
			fromString(std::string const& str)
		{
			return TPEnum<EnumType>::fromString(str);
		}

		template <typename Type>
		typename std::enable_if_t<std::is_class<Type>::value && !std::is_same<Type, std::string>::value, std::string>
			toString(Type const& x)
		{
			return x.toString();
		}

		//template <typename Type>
		////typename std::enable_if_t<std::is_class<Type>::value && !std::is_same<Type, std::vector<std::string>>::value, std::vector<std::string>>
		//std::vector<std::string>
		//	toAllStrings(Type const& x)
		//{
		//	return x.toAllStrings();
		//}

		// A convenience wrapper for EnumClass<EnumType>::toString that infers the type parameter for EnumClass
		template <class EnumType>
		inline
			typename std::enable_if_t<std::is_enum<EnumType>::value, std::string>
			toString(const EnumType e)
		{
			return TPEnum<EnumType>::toString(e);
		}

		template <class EnumType>
		inline
			typename std::enable_if_t<std::is_enum<EnumType>::value, std::vector<std::string>>
			toAllStrings(const EnumType e)
		{
			return TPEnum<EnumType>::toAllStrings(e);
		}

		template <class EnumType>
		inline
			typename std::enable_if_t<std::is_enum<EnumType>::value, int>	toIntegral(const EnumType e)
		{
			return to_underlying<EnumType>(e);
		}

	}
}

namespace std {
	template<typename EnumType>
	struct hash< ::TP::TPEnum< EnumType > >
	{
		using argument_type = ::TP::TPEnum<EnumType>;
		using result_type = size_t;
		result_type operator()(argument_type const& s) const
		{
			return s.hash();
		}
	};
}

#define TP_SERIALIZATION_ENUM(EnumType) \
    namespace cereal \
    { \
        template <class Archive> inline \
        std::string save_minimal(Archive const &, EnumType const & e) \
        { \
            return ::TP::TPEnum<EnumType>::toString(e); \
        } \
        template <class Archive> inline \
        void load_minimal(Archive const &, EnumType & e, std::string const & str) \
        { \
            e = ::TP::TPEnum<EnumType>::fromString(str); \
        } \
    }