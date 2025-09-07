#include "YearMonthDay.h"
#include "Durations.h"
#include <fmt/format.h>
#include <fmt/chrono.h>

//#include "common/algorithm/string.h"
#include "TPTools/StringUtils/TPStringUtils.h"

#include <array>
#include <cctype>
#include <utility>

// ReSharper disable CppRedundantQualifier

using namespace TP::stringutils;
using namespace TP::date_time;
using namespace TP::formatting;

namespace
{
	template<typename T>
	TP::Optional<T> parser(const std::string_view& str, const std::string_view& fmt, T& out)
	{
		std::istringstream in{ std::string(str) };
		//in >> std::chrono::parse(fmt, out);
		//return out.ok() ? TP::makeOptional(std::move(out)) : std::nullopt;
		try
		{
			in >> std::chrono::parse(std::string(fmt), out);
			if(in.fail() || !in.eof())
				return std::nullopt;
			return TP::makeOptional(std::move(out));
		}
		catch(const std::exception&)
		{
			return std::nullopt;
		}
	}
}

// DAY --------------------------------

Formatter<Day>::Formatter(TP::Optional<std::string_view> fmtOverride)
	: override_(std::move(fmtOverride))
{
}

TP::Optional<Day> Formatter<Day>::tryParse(const std::string_view& str) const
{
	auto tp = 0d;
	const auto fmt = override_.valueOr("%d");
	return parser(str, fmt, tp);
}

std::string Formatter<Day>::str(const Day& value) const
{
	std::string_view fmt = override_.valueOr("%d");
	try
	{
		//return format(std::locale(), fmt, value);
		//return format(std::locale(), "%d", value);
		return std::vformat(std::locale(), fmt, std::make_format_args(value));
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(fmt::format("Failed to format day {} with format '{}': {}", static_cast<unsigned>(value), fmt, e.what()));
	}
	//return format(std::locale(), override_.valueOr("%d"), value);
}

std::string Formatter<Day>::failToParse(const std::string_view& str) const
{
	return ::failToParse<Day>(str, __FILE__, __LINE__);
}

namespace
{
	bool isDayNumber(const std::string_view& str)
	{
		auto it = str.begin();
		switch (*it)
		{
		case '0':
			return *++it > '0' && *it <= '9';
		case '1':
		case '2':
			return *++it >= '0' && *it <= '9';
		case '3':
			return *++it == '0' || *it == '1';
		default:
			return false;
		}
	}
}

bool Formatter<std::chrono::day>::isNumeric(const std::string_view& str)
{
	return str.length() == 2 && isDayNumber(str);
}

// MONTH ------------------------------

Formatter<Month>::Formatter(TP::Optional<std::string_view> fmtOverride)
	: override_(std::move(fmtOverride))
{
}

namespace
{
	std::string_view monthFmt(const std::string_view& str) noexcept
	{
		switch (str.length())
		{
		case 1:
			return "%C";
		case 2:
			return "%m";
		case 3:
			return "%b";
		default:
			return "%B";
		}
	}

	constexpr std::array<char, 13> bbgCodes
	{
		'E' /* this one is invalid but avoids shifting one */,
		'F', 'G', 'H','J','K','M','N','Q','U','V','X','Z'
	};

	constexpr int offset(const char c)
	{
		return c - 'F';
	}

	auto months()
	{
		std::array<Month, offset('Z') + 1> ms = { Month{0} };
		for (auto i = 1u; i < bbgCodes.size(); ++i)
		{
			ms[offset(bbgCodes[i])] = Month{ i };
		}
		return ms;
	}

	Month parseMonthBbgCode(const char code)
	{
		static const auto m = months();
		static const auto size = static_cast<int>(m.size());
		const auto i = offset(code);
		return  i < 0 || i > size ? Month() : m[i];
	}

	bool isSingleFmtStr(const std::string_view& fmt)
	{
		return fmt.length() == 2 && fmt[0] == '%';
	}

	bool isBbgFmt(const std::string_view& fmt)
	{
		return isSingleFmtStr(fmt) && (fmt[1] == 'C' || fmt[1] == 'c');
	}
}

TP::Optional<Month> Formatter<Month>::tryParse(const std::string_view& str) const
{
	std::string_view fmt = override_.valueOr([&str]() { return monthFmt(str); });
	if (isBbgFmt(fmt))
		return bloombergCode(str[0]);

	auto tp = Month{ 0 };
	return parser(fmt != "%m" ? toCamelCase(std::string(str)) : str, fmt, tp);
}

std::string Formatter<Month>::str(const Month& value) const
{
	std::string_view fmt = override_.valueOr("%b");
	if (isBbgFmt(fmt))
	{
		return std::string(1, bbgCodes[static_cast<unsigned>(value)]);
	}

	//return format(std::locale(), fmt, value);
	return std::vformat(std::locale(), fmt, std::make_format_args(value));
}

std::string Formatter<Month>::failToParse(const std::string_view& str) const
{
	return ::failToParse<Month>(str, __FILE__, __LINE__);
}

namespace
{
	bool isMonthNumber(const std::string_view& str, size_t i)
	{
		switch (str[i])
		{
		case '0':
			return str[++i] > '0' && str[i] <= '9';
		case '1':
			return str[++i] >= '0' && str[i] <= '2';
		default:
			return false;
		}
	}
}

bool Formatter<Month>::isNumeric(const std::string_view& str)
{
	return str.length() == 2 && isMonthNumber(str, 0);
}

TP::Optional<Month> Formatter<Month, void>::bloombergCode(char c)
{
	const auto tp = parseMonthBbgCode(static_cast<char>(std::toupper(c)));
	return tp.ok() ? makeOptional(std::move(tp)) : std::nullopt;
}

// YEAR ------------------------------

namespace
{
	std::string_view yearFmt(const std::string_view& str) noexcept
	{
		switch (str.length())
		{
		case 1:
			return "%c";
		case 2:
			return "%C";
		default:
			return "%Y";
		}
	}

	auto asOfOrToday(const TP::Optional<YearMonthDay>& asOf)
	{
		const auto refDate = asOf
			.valueOr([]()
		{
			// quite expensive so let's call a function
			return YearMonthDay{ floor<Days>(std::chrono::system_clock::now()) };
		});
		return refDate;
	}

	constexpr int pow10(const int n)
	{
		return n <= 0 ? 1 : 10 * pow10(n - 1);
	}

	template<int Basis>
	auto parseYearDigits(const TP::Optional<YearMonthDay>& asOf, const char* str)
	{
		constexpr auto basis = pow10(Basis);
		constexpr auto lower = Years{ 2 * pow10(Basis - 1) }; // if basis is 10 it's 2y before to 8y after
		constexpr auto upper = Years{ 8 * pow10(Basis - 1) };

		const auto asOfYear = asOfOrToday(asOf).year();
		auto y = Year{ static_cast<int>(asOfYear) / basis * basis /*2025 becomes 2020*/ + std::atoi(str) };
		if (y <= asOfYear - lower)
			y += Years{ basis };
		else if (y > asOfYear + upper)
			y -= Years{ basis };

		return y;
	}

	auto toStringYearDigits(const TP::Optional<YearMonthDay>& asOf, const Year y, const char format)
	{
		const auto asOfYear = asOfOrToday(asOf).year();

		switch (format)
		{
		case 'c':
			if (asOfYear - 2_Y <= y && y < asOfYear + 8_Y) return std::to_string(static_cast<int>(y) % 10);
		case 'C':
			if (asOfYear - 20_Y <= y && y < asOfYear + 80_Y) return std::to_string(static_cast<int>(y) % 100);
		default:
			return std::to_string(static_cast<int>(y));
		}
	}
}

Formatter<Year>::Formatter(TP::Optional<std::string_view> fmtOverride, TP::Optional<YearMonthDay> asOf)
	: override_(std::move(fmtOverride)), asOf_(std::move(asOf))
{
}

TP::Optional<Year> Formatter<Year>::tryParse(const std::string_view& str) const
{
	auto tp = Year{ std::numeric_limits<short>::min() };
	const auto fmt = override_.valueOr([&str]() { return yearFmt(str); });

	if (!isSingleFmtStr(fmt)) return tp;

	switch (fmt[1])
	{
	case 'c':
		return parseYearDigits<1>(asOf_, std::string(str).c_str());

	case 'C':
		return parseYearDigits<2>(asOf_, std::string(str).c_str());

	default:
		return parser(str, fmt, tp);
	}
}

std::string Formatter<Year>::str(const Year& value) const
{
	std::string_view fmt= override_.valueOr("%Y");
	if (isBbgFmt(fmt)) return toStringYearDigits(asOf_, value, fmt[1]);
	try
	{
		//return format(std::locale(), fmt, value);
		return std::vformat(std::locale(), fmt, std::make_format_args(value));
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(fmt::format("Failed to format year {} with format '{}': {}", static_cast<int>(value), fmt, e.what()));
	}
	//return format(std::locale(), fmt, value);
}

std::string Formatter<Year>::failToParse(const std::string_view& str) const
{
	return ::failToParse<Year>(str, __FILE__, __LINE__);
}

// Weekday ------------------------------

Formatter<std::chrono::weekday>::Formatter(TP::Optional<std::string_view> fmtOverride)
	: override_(std::move(fmtOverride))
{
}

namespace
{
	std::string_view wdFmt(const std::string_view& str) noexcept
	{
		return str.length() == 3 ? "%a" : "%A";
	}
}

TP::Optional<Weekday> Formatter<Weekday>::tryParse(const std::string_view& str) const
{
	auto tp = Weekday{ 7 };
	return parser(toCamelCase(std::string(str)), override_.valueOr([&str]() { return wdFmt(str); }), tp);
}

std::string Formatter<Weekday>::str(const Weekday& value) const
{
	//return format(std::locale(), override_.valueOr("%A"), value);
	return std::vformat(std::locale(), override_.valueOr("%A"), std::make_format_args(value));
}

std::string Formatter<Weekday>::failToParse(const std::string_view& str) const
{
	return ::failToParse<Weekday>(str, __FILE__, __LINE__);
}

// YearMonth ------------------------------
Formatter<YearMonth>::Formatter(TP::Optional<std::string_view> fmtOverride, TP::Optional<YearMonthDay> asOf)
	: override_(std::move(fmtOverride)), asOf_(std::move(asOf))
{
}

namespace
{
	std::string ymFmt(const std::string_view& str) noexcept
	{
		switch (str.length())
		{
		case 2:
			return "%c";
		case 3:
			return "%C";
		case 4:
			return "%m%y";
		case 5:
			return "%b%y";
		case 6:
		{
			if (std::ispunct(str[3]))
				return { '%', 'b', str[3], '%', 'y', '\0' };
			if (std::ispunct(str[2]))
				return { '%', 'y', str[2], '%', 'b', '\0' };
			return "%m%Y";
		}
		case 7:
		{
			if (std::ispunct(str[2]))
				return { '%', 'm', str[2], '%', 'Y', '\0' };
			if (std::ispunct(str[4]))
				return { '%', 'Y', str[4], '%', 'm', '\0' };
			return "%b%Y";
		}
		case 8:
		{
			if (std::ispunct(str[3]))
				return { '%', 'b' , str[3], '%', 'Y', '\0' };
			return { '%', 'Y' , str[4], '%', 'b', '\0' };
		}
		default:
			return "%B%Y";
		}
	}

}

TP::Optional<YearMonth> Formatter<YearMonth>::tryParse(const std::string_view& str) const
{
	auto tp = 0y / Month{ 0 };
	std::string_view fmt;
	if (override_.hasValue() )
		fmt = *override_; 
	else
		fmt = ymFmt(str);
	//const auto fmt = override_.valueOr([&str]() { return ymFmt(str); });
	if (!isSingleFmtStr(fmt))
	{
		return parser(toCamelCase(std::string(str)), fmt, tp);
	}

	switch (fmt[1])
	{
	case 'c':
		return str.length() == 2 ? parseYearDigits<1>(asOf_, &str[1]) / parseMonthBbgCode(str[0]) : tp;

	case 'C':
		return str.length() == 3 ? parseYearDigits<2>(asOf_, &str[1]) / parseMonthBbgCode(str[0]) : tp;

	default:
		return parser(str, fmt, tp);
	}

}

std::string Formatter<YearMonth>::str(const YearMonth& value) const
{
	std::string_view fmt = override_.valueOr("%b%Y");
	if (isBbgFmt(fmt))
	{
		std::string res;
		res.reserve(3);
		res += bbgCodes[static_cast<unsigned>(value.month())];
		res += toStringYearDigits(asOf_, value.year(), fmt[1]);
		return res;
	}

	//return format(std::locale(), fmt, value);
	return std::vformat(std::locale(), fmt, std::make_format_args(value));
}

std::string Formatter<YearMonth>::failToParse(const std::string_view& str) const
{
	return TP::failToParse<YearMonth>(str, __FILE__, __LINE__);
}

// YearMonthDay ------------------------------
Formatter<YearMonthDay>::Formatter(TP::Optional<std::string_view> fmtOverride)
	: override_(std::move(fmtOverride))
{
}

namespace
{
	// guesses the date format for lots of strings
	std::string_view ymdFmt(const std::string_view& str) noexcept
	{
		switch (str.size())
		{
		case 6:
		{
			return Formatter<Day>::isNumeric(str.substr(4, 2)) ? "%y%m%d" : "%d%m%y";
		}
		case 7:
		{
			return Formatter<Day>::isNumeric(str) ? "%d%b%y" : "%y%b%d";
		}
		case 8:
		{
			if (std::ispunct(str[4]) && std::ispunct(str[6]))
			{ // 2000/1/1 fmt
				const char reverse[] = { '%', 'Y', str[4], '%', 'm', str[6], '%', 'd', '\0' };
				return reverse;
			}
			if (std::ispunct(str[1]) && std::ispunct(str[3]))
			{ // 1/1/2000 fmt
				const char front[] = { '%', 'd', str[1], '%', 'm', str[3], '%', 'Y', '\0' };
				return front;
			}

			// otherwise must be 01012000 fmt or 20000101 fmt
			return Formatter<Day>::isNumeric(str.substr(6, 2)) && Formatter<Month>::isNumeric(str.substr(4, 2))
				? "%Y%m%d"
				: "%d%m%Y";
		}
		case 9:
		{
			if (std::ispunct(str[1]))
			{ //  1/10/2020 fmt
				const char front[] = { '%', 'd', str[1], '%', 'm', str[4], '%', 'Y', '\0' };
				return front;
			}
			if (std::ispunct(str[2]))
			{ //  10/1/2020 fmt
				const char front[] = { '%', 'd', str[2], '%', 'm', str[4], '%', 'Y', '\0' };
				return front;
			}
			if (std::ispunct(str[7]))
			{ //  2020/10/1 fmt
				const char reverse[] = { '%', 'Y', str[4], '%', 'm', str[7], '%', 'd', '\0' };
				return reverse;
			}
			if (std::ispunct(str[6]))
			{ //  2020/1/10 fmt
				const char reverse[] = { '%', 'Y', str[4], '%', 'm', str[6], '%', 'd', '\0' };
				return reverse;
			}

			return std::isalpha(str[2]) ? "%d%b%Y" : "%Y%b%d";
		}
		case 10:
		{
			const char front[] = { '%', 'd', str[2], '%', 'm', str[5], '%', 'Y', '\0' };
			const char reverse[] = { '%', 'Y', str[4], '%', 'm', str[7], '%', 'd', '\0' };
			return !std::isdigit(str[2]) ? front : reverse;
		}
		case 11:
		{
			const char front[] = { '%', 'd', str[2], '%', 'b', str[6], '%', 'Y', '\0' };
			const char reverse[] = { '%', 'Y', str[4], '%', 'b', str[8], '%', 'd', '\0' };
			return !std::isdigit(str[2]) ? front : reverse;
		}
		case 12:
		{
			const char front[] = { '%', 'd', str[2], str[3], '%', 'm', str[6], str[7], '%', 'Y', '\0' };
			const char reverse[] = { '%', 'Y', str[4], str[5], '%', 'm', str[8], str[9], '%', 'd', '\0' };
			return !std::isdigit(str[2]) ? front : reverse;
		}
		case 13:
		{
			const char front[] = { '%', 'd', str[2], str[3], '%', 'b', str[7], str[8], '%', 'Y', '\0' };
			const char reverse[] = { '%', 'Y', str[4], str[5], '%', 'b', str[9], str[10], '%', 'd', '\0' };
			return !std::isdigit(str[2]) ? front : reverse;
		}
		default:
			return "%Y%m%d";
		}
	}

}

TP::Optional<YearMonthDay> Formatter<YearMonthDay>::tryParse(const std::string_view& str) const
{
	auto tp = YearMonthDay{ 0y, Month{0}, 0d };
	return parser(toCamelCase(std::string(str)), override_.valueOr([&str]() { return ymdFmt(str); }), tp);
}

std::string Formatter<YearMonthDay>::str(const YearMonthDay& value) const
{
	//return format(std::locale(), override_.valueOr("%Y%m%d"), value);
	return std::vformat(std::locale(), override_.valueOr("%Y%m%d"), std::make_format_args(value));
}

std::string Formatter<YearMonthDay>::failToParse(const std::string_view& str) const
{
	return fmt::format("Could not parse {}. Ensure the correct date fmt (current:{}) is provided or this is a valid date", str, override_.valueOr("%Y%m%d"));
}

