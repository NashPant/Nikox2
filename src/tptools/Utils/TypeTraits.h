#pragma once

#include <type_traits>
#include <functional>

// https://devblogs.microsoft.com/cppblog/optimizing-the-layout-of-empty-base-classes-in-vs2015-update-2-3/
#ifndef EMPTY_BASE_OPTIMIZATION_FIX
#if _MSC_VER  < 1900
#define EMPTY_BASE_OPTIMIZATION_FIX __declspec( empty_bases )
#else
#define EMPTY_BASE_OPTIMIZATION_FIX
#endif
#endif

namespace TP
{
	// tells if 2 types are equivalent after decay
	template <typename T, typename U>
	struct DecayEquiv : std::is_same<typename std::decay<T>::type, U>::type {};

	// tells if a type is complete, i.e. it has a body
	template <typename T>
	struct IsCompleteHelper
	{
		// ReSharper disable CppFunctionIsNotImplemented
		template <typename U>
		static auto test(U*)->std::integral_constant<bool, sizeof(U) == sizeof(U)>;

		static auto test(...)->std::false_type;
		// ReSharper restore CppFunctionIsNotImplemented

		using type = decltype(test((T*)0));
	};

	//template <typename T>
	//struct IsComplete : IsCompleteHelper<T>::type {};

	template<class T, class=void>
	struct IsComplete : std::false_type
	{};
	template<class T>
	struct IsComplete<T, decltype(void(sizeof(T)))>: std::true_type
	{};


	template<class T>
	constexpr bool isCompleteV = IsComplete<T>::value;


	/**
	 * \brief With this utility you can write all several lambdas for all matching types in one place:

		std::visit
		(
		  Overload
		  (
			[](const int& i) { PRINT("int: " + i); },
			[](const std::string& s) { PRINT("it's a string: " + s); },
			[](const float& f) { PRINT("float" + f); }
		  ),
		  yourVariant<int, std::string, float>;
		);
	 */
	//template<class... Ts> struct EMPTY_BASE_OPTIMIZATION_FIX Overloaded : Ts... { using Ts::operator()...; };
	//template<class... Ts> Overloaded(Ts...)->Overloaded<Ts...>; // Note that this will not be needed anymore in C++20

	//namespace details
	//{
	//	template<class X, class Y, class Op>
	//	struct HasOperation
	//	{
	//		template<class U, class L, class R>
	//		static auto test(int) ->decltype(std::declval<U>()(std::declval<L>(), std::declval<R>()),
	//			void(), std::true_type());

	//		template<class U, class L, class R>
	//		static auto test(...) -> std::false_type;

	//		using Type = decltype(test<Op, X, Y>(0));
	//	};
	//}

	//template<class X, class Y, class Op> using HasOperationT = typename details::HasOperation<X, Y, Op>::Type;
	//template<class X, class Y = X> using HasEquality = HasOperationT<X, Y, std::equal_to<>>;
	//template<class X, class Y = X> using HasInequality = HasOperationT<X, Y, std::not_equal_to<>>;
	//template<class X, class Y = X> using HasLessThan = HasOperationT<X, Y, std::less<>>;
	//template<class X, class Y = X> using HasLessThanEqual = HasOperationT<X, Y, std::less_equal<>>;
	//template<class X, class Y = X> using HasGreaterThan = HasOperationT<X, Y, std::greater<>>;
	//template<class X, class Y = X> using HasGreaterThanEqual = HasOperationT<X, Y, std::greater_equal<>>;

	//template<class X, class Y, class Op> constexpr bool hasOperation = HasOperationT<X, Y, Op>::value;
	//template<class X, class Y = X> constexpr bool hasEquality = HasEquality<X, Y>::value;
	//template<class X, class Y = X> constexpr bool hasInequality = HasInequality<X, Y>::value;
	//template<class X, class Y = X> constexpr bool hasLessThan = HasLessThan<X, Y>::value;
	//template<class X, class Y = X> constexpr bool hasLessThanEqual = HasLessThanEqual<X, Y>::value;
	//template<class X, class Y = X> constexpr bool hasGreaterThan = HasGreaterThan<X, Y>::value;
	//template<class X, class Y = X> constexpr bool hasGreaterThanEqual = HasGreaterThanEqual<X, Y>::value;
}