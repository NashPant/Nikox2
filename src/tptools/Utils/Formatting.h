#pragma once

//#include "Optional.h"
#include <optional>
// ReSharper disable CppUnusedIncludeDirective
//#include "common/TypeTraits.h" // used in macros
// ReSharper restore CppUnusedIncludeDirective
#include "tptools/Utils/TypeTraits.h"
#include <fmt/format.h>

#include <type_traits>
#include <stdexcept>
// ReSharper disable CppUnusedIncludeDirective
#include <ostream> // used in macro
// ReSharper restore CppUnusedIncludeDirective
#include <string>

namespace TP
{
	/**
	 * Non intrusive common formatting API for .
	 *
	 * Allows to consistently implement parse/generate strings to/from objects.
	 *
	 */
	inline namespace formatting
	{
		/* Formatter template interface, please implement specialization for your type if you want to use it*/
		template<typename T, typename Enable = void> struct Formatter;

		/* To implement a formatter for a custom type please follow below API:
		 *
		 * struct A { int a; };
		 *
		 * template<> struct Formatter<A>
		 * {
		 *  [[nodiscard]] ligo::Optional<T> tryParse(const std::string_view& str) const {...}
		 *  [[nodiscard]] std::string str(const A& value) const {...}
		 *  [[nodiscard]] std::string failToParse(const std::string_view& str) const {...}
		 * };
		 *
		 * To use the fmt framework one will also need to have a default constructor
		 */

		template<typename T>
		auto failToParse(const std::string_view& str, const char* file, const int line)
		{
			return fmt::format("Could not parse {} as {}. {}({})", str, typeid(T).name(), file, line);
		}
	}

	/**
	 * @brief parsing functionality
	 */
	template<typename T, typename StrT>
	std::optional<T> tryParse(const StrT& str, const Formatter<T>& formatter)  
	{
		return formatter.tryParse(str);
	}

	template<typename T, typename StrT, typename... Types, typename = std::enable_if_t<std::is_constructible_v<Formatter<T>, Types...>>>
	std::optional<T> tryParse(const StrT& str, Types&&... args)
	{
		const auto fmt = Formatter<T>(std::forward<Types>(args)...);
		return tryParse(str, fmt);
	}

	template<typename T, typename StrT>
	T parse(const StrT& str, const Formatter<T>& formatter)
	{
	/*	auto res = formatter.tryParse(str);
		if(res.has_value())
			return res.value();
		
		std::string msg = formatter.failToParse(str);
		throw std::invalid_argument(msg);*/

		return formatter
			.tryParse(str)
			.orElse([str, &formatter]()
		{
			std::string msg = formatter.failToParse(str);
			throw std::invalid_argument(msg);
		})
			.value();
	}

	template<typename T, typename StrT, typename... Types, typename = std::enable_if_t<std::is_constructible_v<Formatter<T>, Types...>>>
	T parse(const StrT& str, Types&&... args)
	{
		const auto fmt = Formatter<T>(std::forward<Types>(args)...);
		return parse(str, fmt);
	}

	template<typename T>
	auto toString(const T& value, const Formatter<T>& formatter)
	{
		return formatter.str(value);
	}

	template<typename T, typename... Types, typename = std::enable_if_t<std::is_constructible_v<Formatter<T>, Types...>>>
	auto toString(const T& value, Types&&... args)
	{
		const auto fmt = Formatter<T>(std::forward<Types>(args)...);
		return toString(value, fmt);
	}
}

/**
 * \brief ostream functionality (macro because it has to live in the same namespace as the declared class. Koenig lookup!)
 */
#define OSTREAM_FROM_TP(T)                                                                        \
template<typename Char, typename Traits>                                                            \
std::basic_ostream<Char, Traits>& operator <<(std::basic_ostream<Char, Traits>& os, const T& value) \
{                                                                                                   \
        static const TP::Formatter<T> formatter;                                                  \
		os << formatter.str(value);                                                                 \
	return os;                                                                                      \
}

FMT_BEGIN_NAMESPACE
template <typename T, typename Enable = void>
struct TP_formatter
	: formatter<std::string>
{
	template <typename FormatContext>
	auto format(const T& value, FormatContext &ctx) const -> decltype(ctx.out())
	{
		static const TP::Formatter<T> TPFormatter;
		return fmt::formatter<std::string>::format(TPFormatter.str(value), ctx);
	}
};
FMT_END_NAMESPACE

#define DECLARE_FMT_FORMATTER_FROM_TP(T) \
FMT_BEGIN_NAMESPACE                        \
    template <> struct formatter<T> : fmt::TP_formatter<T, std::enable_if_t<TP::isCompleteV<TP::Formatter<T>>>>{}; \
FMT_END_NAMESPACE
