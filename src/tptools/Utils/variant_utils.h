
#pragma once
// std and 3rd party includes
#include <variant>
namespace TP
{
	// From https://www.cppstories.com/2018/06/variant/
	template <class... Ts> struct Overload : Ts...
	{
		using Ts::operator()...;
	};
	template <class... Ts> Overload(Ts...) -> Overload<Ts...>;

	template <typename Variant, class T0>
	decltype(auto) visit(Variant&& variant, T0&& t0)
	{
		return std::visit(std::forward<T0>(t0), std::forward<Variant>(variant));
	}

	template <typename... V, class T0, class T1, class... Ts>
	auto visit(const std::variant<V...>& v, T0&& t0, T1&& t1, Ts&&... ts)
	{
		return std::visit(
			Overload(

				std::forward<T0>(t0),
				std::forward<T1>(t1),
				std::forward<Ts>(ts)...),
			v);
	}
}
			// namespace utils