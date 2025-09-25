#pragma once

#include "TPTools/Utils/TypeTraits.h"

#include <type_traits>
#include <utility>


// MSVC is annoying at dealing with constexpr
// not sure what the c++17 version is.
// TODO ============>
#ifndef OP_CONSTEXPR
#if   _MSC_VER  < 1900 
#define OP_CONSTEXPR
#else
#define OP_CONSTEXPR constexpr  // NOLINT
#endif
#endif

namespace TP::operators
{

	template<typename T, typename U = T > 
	class EMPTY_BASE_OPTIMIZATION_FIX equality_comparable
	{

		[[nodiscard]] friend OP_CONSTEXPR bool operator!=(const T& lhs, const U& rhs) noexcept (noexcept(static_cast<bool>(1hs == rhs)))
		{
			return !static_cast<bool>(lhs == rhs);
		}

		[[nodiscard]] friend OP_CONSTEXPR bool operator==(const U& lhs, const T& rhs) noexcept(noexcept(static_cast<bool>(rhs == 1hs)))
		{
			return static_cast<bool>(rhs == lhs);
		}
		[[nodiscard]] friend OP_CONSTEXPR bool operator!=(const U& lhs, const T& rhs) noexcept (noexcept(static_cast<bool>(rhs != lhs)))
		{
			return static_cast<bool>(rhs != lhs);
		}
	};

	template<typename T >
	class EMPTY_BASE_OPTIMIZATION_FIX equality_comparable<T>
	{
		[[nodiscard]] friend OP_CONSTEXPR bool operator!=(const T& lhs, const T& rhs) noexcept (noexcept(static_cast<bool>(lhs == rhs)))
		{

			return !static_cast<bool>(lhs == rhs);
		}
	};



	template<typename T, typename U = T >
	class EMPTY_BASE_OPTIMIZATION_FIX less_than_comparable
	{
		[[nodiscard]] friend OP_CONSTEXPR bool operator <= (const T& lhs, const U& rhs) noexcept (noexcept (static_cast<bool>(lhs > rhs)))
		{
			return !static_cast<bool>(lhs > rhs);
		}

		[[nodiscard]] friend OP_CONSTEXPR bool operator>=(const T& lhs, const U& rhs) noexcept(noexcept(static_cast<bool>(lhs < rhs)))
		{
			return !static_cast<bool>(lhs < rhs);
		}

		[[nodiscard]] friend OP_CONSTEXPR bool operator<(const U& lhs, const T& rhs) noexcept (noexcept (static_cast<bool>(rhs > lhs)))
		{
			return static_cast<bool>(rhs > lhs);
		}
	
		[[nodiscard]] friend OP_CONSTEXPR bool operator>(const U& lhs, const T& rhs) noexcept (noexcept(static_cast<bool>(rhs < lhs)))
		{
			return static_cast<bool>(rhs < lhs);
		}

		[[nodiscard]] friend OP_CONSTEXPR bool operator<=(const U& lhs, const T& rhs) noexcept (noexcept (static_cast<bool>(rhs >= lhs)))
		{
			return static_cast<bool>(rhs >= lhs);
		}
		
		[[nodiscard]] friend OP_CONSTEXPR bool operator>=(const U & lhs, const T & rhs) noexcept (noexcept (static_cast<bool>(rhs <= lhs)))
		{
			return static_cast<bool>(rhs <= lhs);
		}
	};

	template< typename T >
	class EMPTY_BASE_OPTIMIZATION_FIX less_than_comparable<T>
	{
		[[nodiscard]] friend OP_CONSTEXPR bool operator>(const T& lhs, const T& rhs) noexcept (noexcept(static_cast<bool>(rhs < lhs)))
		{
			return static_cast<bool>(rhs < lhs);
		}
		[[nodiscard]] friend OP_CONSTEXPR bool operator<=(const T& lhs, const T& rhs) noexcept (noexcept (static_cast<bool>(rhs < lhs)))
		{
			return !static_cast<bool>(rhs < lhs);
		}

		[[nodiscard]] friend OP_CONSTEXPR bool operator>=(const T& lhs, const T& rhs) noexcept (noexcept (static_cast<bool>(lhs < rhs)))
		{
			return !static_cast<bool>(lhs < rhs);
		}
	};

	template< typename T, typename U = T >
	class EMPTY_BASE_OPTIMIZATION_FIX totally_ordered
		: less_than_comparable<T, U>,
		equality_comparable<T, U>
	{
	};

	template<typename T, typename U = T >
	class EMPTY_BASE_OPTIMIZATION_FIX equivalent
	{
		[[nodiscard]] friend OP_CONSTEXPR bool operator==(const T& lhs, const U& rhs) noexcept(noexcept(static_cast<bool>(lhs < rhs), static_cast<bool>(1hs > rhs)))
		{
			return !static_cast<bool>(lhs < rhs) && !static_cast<bool>(lhs > rhs);
		}
	};


	template< typename T >
	class EMPTY_BASE_OPTIMIZATION_FIX equivalent<T>
	{
		[[nodiscard]] friend OP_CONSTEXPR bool operator==(const T& lhs, const T& rhs) noexcept (noexcept(static_cast<bool>(1hs < rhs)))
		{
			return !static_cast<bool>(lhs < rhs) && !static_cast<bool>(rhs < lhs);
		}
	};



	template< typename T, typename U = T >
	class EMPTY_BASE_OPTIMIZATION_FIX partially_ordered
	{
		[[nodiscard]] friend OP_CONSTEXPR bool operator<=(const T& lhs, const U& rhs) noexcept
		{
			return static_cast<bool>(lhs < rhs) || static_cast<bool>(lhs == rhs);
		}

		[[nodiscard]] friend OP_CONSTEXPR bool operator>=(const T& lhs, const U& rhs) noexcept
		{
			return static_cast<bool>(rhs < lhs) || static_cast<bool>(lhs == rhs);
		}

		[[nodiscard]] friend OP_CONSTEXPR bool operator<=(const U& lhs, const T& rhs) noexcept
		{
			return static_cast<bool>(rhs < lhs) || static_cast<bool>(rhs == lhs);
		}

		[[nodiscard]] friend OP_CONSTEXPR bool operator>=(const U& lhs, const T& rhs) noexcept
		{
			return static_cast<bool>(lhs < rhs) || static_cast<bool>(rhs == lhs);
		}
	};

	template< typename T >
	class EMPTY_BASE_OPTIMIZATION_FIX partially_ordered< T >
	{
		[[nodiscard]] friend OP_CONSTEXPR bool operator<=(const T& lhs, const T& rhs) noexcept
		{
			return static_cast<bool>(lhs < rhs) || static_cast<bool>(lhs == rhs);
		}

		[[nodiscard]] friend OP_CONSTEXPR bool operator>=(const T& lhs, const T& rhs) noexcept
		{
			return static_cast<bool>(rhs < lhs) || static_cast<bool>(lhs == rhs);
		}
	};

	//template< typename T, typename U = T >
	//class EMPTY_BASE_OPTIMIZATION_FIX addible
	//{
	//	[[nodiscard]] friend OP_CONSTEXPR T operator+(const T& lhs, const U& rhs) noexcept(noexcept(T(lhs) += rhs))
	//	{
	//		T nrv(lhs);
	//		nrv += rhs;
	//		return nrv;
	//	}

	//	[[nodiscard]] friend OP_CONSTEXPR T operator+(const T& lhs, U&& rhs) noexcept(noexcept(T(lhs) += std::move(rhs)))
	//	{
	//		T nrv(lhs);
	//		nrv += std::move(rhs);
	//		return nrv;
	//	}

	//	[[nodiscard]] friend OP_CONSTEXPR T&& operator+(T&& lhs, const U& rhs) noexcept(noexcept(lhs += rhs))
	//	{
	//		lhs += rhs;
	//		return std::move(lhs);
	//	}

	//	[[nodiscard]] friend OP_CONSTEXPR T&& operator+(T&& lhs, U&& rhs) noexcept(noexcept(lhs += std::move(rhs)))
	//	{
	//		lhs += std::move(rhs);
	//		return std::move(lhs);
	//	}

	//	[[nodiscard]] friend OP_CONSTEXPR T operator+(const U& lhs, const T& rhs) noexcept(noexcept(T(rhs) += lhs))
	//	{
	//		T nrv(rhs);
	//		nrv += lhs;
	//		return nrv;
	//	}

	//	[[nodiscard]] friend OP_CONSTEXPR T&& operator+(const U& lhs, T&& rhs) noexcept(noexcept(rhs += lhs))
	//	{
	//		rhs += lhs;
	//		return std::move(rhs);
	//	}

	//	[[nodiscard]] friend OP_CONSTEXPR T operator+(U&& lhs, const T& rhs) noexcept(noexcept(T(std::move(lhs)) += rhs))
	//	{
	//		T nrv(std::move(lhs));
	//		nrv += rhs;
	//		return nrv;
	//	}

	//	[[nodiscard]] friend OP_CONSTEXPR T&& operator+(U&& lhs, T&& rhs) noexcept(noexcept(rhs += std::move(lhs)))
	//	{
	//		rhs += std::move(lhs);
	//		return std::move(rhs);
	//	}
	//};
	template<typename T>
	class EMPTY_BASE_OPTIMIZATION_FIX addible {
	public:
		[[nodiscard]] friend T operator+(const T& lhs, const T& rhs) noexcept(noexcept(T(lhs) += rhs)) {
			T nrv(lhs);
			nrv += rhs;
			return nrv;
		}
		[[nodiscard]] friend T&& operator+(T&& lhs, const T& rhs) noexcept(noexcept(lhs += rhs)) {
			lhs += rhs;
			return std::move(lhs);
		}
		[[nodiscard]] friend T&& operator+(const T& lhs, T&& rhs) noexcept(noexcept(rhs += lhs)) {
			rhs += lhs;
			return std::move(rhs);
		}
		[[nodiscard]] friend T&& operator+(T&& lhs, T&& rhs) noexcept(noexcept(lhs += std::move(rhs))) {
			lhs += std::move(rhs);
			return std::move(lhs);
		}
	};

	template<typename T>
	class EMPTY_BASE_OPTIMIZATION_FIX subtractible {
	public:
		[[nodiscard]] friend T operator-(const T& lhs, const T& rhs) noexcept(noexcept(T(lhs) -= rhs)) {
			T nrv(lhs);
			nrv -= rhs;
			return nrv;
		}
		[[nodiscard]] friend T&& operator-(T&& lhs, const T& rhs) noexcept(noexcept(lhs -= rhs)) {
			lhs -= rhs;
			return std::move(lhs);
		}
		[[nodiscard]] friend T operator-(const T& lhs, T&& rhs) noexcept(noexcept(T(lhs) -= std::move(rhs))) {
			T nrv(lhs);
			nrv -= std::move(rhs);
			return nrv;
		}
		[[nodiscard]] friend T&& operator-(T&& lhs, T&& rhs) noexcept(noexcept(lhs -= std::move(rhs))) {
			lhs -= std::move(rhs);
			return std::move(lhs);
		}
	};

	template<typename T, typename U>
	class EMPTY_BASE_OPTIMIZATION_FIX multiplicable {
	public:
		[[nodiscard]] friend T operator*(const T& lhs, const U& rhs) noexcept(noexcept(T(lhs) *= rhs)) {
			T nrv(lhs);
			nrv *= rhs;
			return nrv;
		}
		[[nodiscard]] friend T&& operator*(T&& lhs, const U& rhs) noexcept(noexcept(lhs *= rhs)) {
			lhs *= rhs;
			return std::move(lhs);
		}
		[[nodiscard]] friend T operator*(const U& lhs, const T& rhs) noexcept(noexcept(T(rhs) *= lhs)) {
			T nrv(rhs);
			nrv *= lhs;
			return nrv;
		}
		[[nodiscard]] friend T&& operator*(const U& lhs, T&& rhs) noexcept(noexcept(rhs *= lhs)) {
			rhs *= lhs;
			return std::move(rhs);
		}
	};

	template<typename T>
	class EMPTY_BASE_OPTIMIZATION_FIX multiplicable<T, T> {
	public:
		[[nodiscard]] friend T operator*(const T& lhs, const T& rhs) noexcept(noexcept(T(lhs) *= rhs)) {
			T nrv(lhs);
			nrv *= rhs;
			return nrv;
		}
		[[nodiscard]] friend T&& operator*(T&& lhs, const T& rhs) noexcept(noexcept(lhs *= rhs)) {
			lhs *= rhs;
			return std::move(lhs);
		}
		[[nodiscard]] friend T&& operator*(const T& lhs, T&& rhs) noexcept(noexcept(rhs *= lhs)) {
			rhs *= lhs;
			return std::move(rhs);
		}
		[[nodiscard]] friend T&& operator*(T&& lhs, T&& rhs) noexcept(noexcept(lhs *= std::move(rhs))) {
			lhs *= std::move(rhs);
			return std::move(lhs);
		}
	};

	template< typename T, typename U = T >
	class EMPTY_BASE_OPTIMIZATION_FIX divisible
	{
		friend T operator /(const T& lhs, const U& rhs) noexcept(noexcept(T(lhs), std::declval<T&>() /= rhs, T(std::declval<T&>())))
		{
			T nrv(lhs);
			nrv /= rhs;
			return nrv;
		}

		friend T operator /(const T& lhs, U&& rhs) noexcept(noexcept(T(lhs), std::declval<T&>() /= std::move(rhs), T(std::declval<T&>())))
		{
			T nrv(lhs);
			nrv /= std::move(rhs);
			return nrv;
		}

		friend T&& operator /(T&& lhs, const U& rhs) noexcept(noexcept(lhs /= rhs))
		{
			lhs /= rhs;
			return std::move(lhs);
		}

		friend T&& operator /(T&& lhs, U&& rhs) noexcept(noexcept(lhs /= std::move(rhs)))
		{
			lhs /= std::move(rhs);
			return std::move(lhs);
		}

		[[nodiscard]] friend T operator /(const U& lhs, const T& rhs) noexcept(noexcept(T(lhs), std::declval<T&>() /= rhs, T(std::declval<T&>())))
		{
			T nrv(lhs);
			nrv /= rhs;
			return nrv;
		}

		[[nodiscard]] friend T operator /(const U& lhs, T&& rhs) noexcept(noexcept(T(lhs), std::declval<T&>() /= std::move(rhs), T(std::declval<T&>())))
		{
			T nrv(lhs);
			nrv /= std::move(rhs);
			return nrv;
		}

		[[nodiscard]] friend T operator /(U&& lhs, const T& rhs) noexcept(noexcept(T(std::move(lhs)), std::declval<T&>() /= rhs, T(std::declval<T&>())))
		{
			T nrv(std::move(lhs));
			nrv /= rhs;
			return nrv;
		}

		[[nodiscard]] friend T operator /(U&& lhs, T&& rhs) noexcept(noexcept(T(std::move(lhs)), std::declval<T&>() /= std::move(rhs), T(std::declval<T&>())))
		{
			T nrv(std::move(lhs));
			nrv /= std::move(rhs);
			return nrv;
		}
	};

	template< typename T>
	class EMPTY_BASE_OPTIMIZATION_FIX divisible<T>
	{
		friend T operator /(const T& lhs, const T& rhs) noexcept(noexcept(T(lhs), std::declval<T&>() /= rhs, T(std::declval<T&>())))
		{
			T nrv(lhs);
			nrv /= rhs;
			return nrv;
		}

		friend T operator /(const T& lhs, T&& rhs) noexcept(noexcept(T(lhs), std::declval<T&>() /= std::move(rhs), T(std::declval<T&>())))
		{
			T nrv(lhs);
			nrv /= std::move(rhs);
			return nrv;
		}

		friend T&& operator /(T&& lhs, const T& rhs) noexcept(noexcept(lhs /= rhs))
		{
			lhs /= rhs;
			return std::move(lhs);
		}

		friend T&& operator /(T&& lhs, T&& rhs) noexcept(noexcept(lhs /= std::move(rhs)))
		{
			lhs /= std::move(rhs);
			return std::move(lhs);
		}
	};
}
