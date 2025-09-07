
#include "Durations.h"
#include "tptools/stringutils/TPStringUtils.h" //#include "date_time/Tenor.h"
//#include <boost/algorithm/string.hpp>
using namespace TP::date_time;
using namespace TP::formatting;

Duration::Duration(const std::string& str)
	: Duration(TP::parse<Duration>(str))
{
}


Duration::operator Days() const
{
	if (months_ == 0_M) return days_;
	// TODO: restore message
	//throw std::logic_error(fmt::format("Cannot cast {} to days only", *this));
	throw std::logic_error("Cannot cast  to days only");
}


Duration::operator Months() const
{
	if (days_==0_D) return months_;
	throw std::logic_error("Cannot cast  to months only");
}

//throw std::logic_error(fmt::format("Cannot cast {} to months only", *this));
Duration Duration::operator-() const noexcept
{
	return Duration(-months_, -days_);
}


Duration& Duration::operator+=(const Duration& right) noexcept
{
	months_ += right.months_;
	days_ += right.days_;
	return *this;
}

Duration& Duration::operator-=(const Duration& right) noexcept
{
	months_ -= right.months_;
	days_ -= right.days_;
	return *this;
}


	
Duration& Duration::operator*=(const int quantity) noexcept
{
	months_ *= quantity;
	days_ *= quantity;
	return *this;
}
double Duration::tau(const Duration& rhs)
{
	Duration diff = *this - rhs;
	return diff.years().count() + diff.months().count() / 12. + diff.weeks().count() / 52. + diff.days().count() / 365.25;
}

// ReSharper disable CppMemberFunctionMayBeStatic
namespace
{
	TP::Optional<std::tuple<double, char, const char*>> strtod(const char* str)
	{
		char* endPtr;
		const auto i = std::strtod(str, &endPtr);
		if (endPtr == str // cannot parse
			|| i == HUGE_VAL   // from http://www.cplusplus.com/reference/cstdlib/strtod/
			|| i == -HUGE_VAL)
			return std::nullopt;

		return std::make_tuple(i, *endPtr++, endPtr);
	}

	struct Parser final
	{
		const char lowerCase{ '\0' };
		const char upperCase{ '\0' };

		[[nodiscard]] bool operator()(const char c) const
		{
			return c == lowerCase || c == upperCase;
		}
	};

	const Parser isDays{ 'd','D' };
	const Parser isBDays{ 'b','B' };
	const Parser isWeeks{ 'w','W' };
	const Parser isMonths{ 'm','M' };
	const Parser isYears{ 'y','Y' };
	const Parser empty{};
}


OptDays Formatter<Days>::tryParse(const std::string_view& str) const
{
	std::string strCopy(str);
	TP::stringutils::trim_copy_std(strCopy);
	return strtod(strCopy.c_str()).andThen
	([](const auto& t) -> OptDays
		{
			if (!empty(*std::get<const char*>(t)))
				return std::nullopt;

			if (isDays(std::get<char>(t)))
				return Days{ std::lround(std::get<double>(t)) };

			if (isWeeks(std::get<char>(t)))
			{
				double intPart;
				const double fracPart = modf(std::get<double>(t), &intPart);
				return Weeks{ std::lround(intPart) } + Days{ std::lround(fracPart * 7) };
			}

			return std::nullopt;
		});
}

std::string Formatter<Days>::str(const Days& value) const
{
	return fmt::format("{}D", value.count());
}

std::string Formatter<Days>::failToParse(const std::string_view& str) const
{
	return TP::failToParse<Days>(str, __FILE__, __LINE__);
}

OptBusinessDays Formatter<BusinessDays>::tryParse(const std::string_view& str) const
{
	std::string strCopy(str);
	TP::stringutils::trim_copy_std(strCopy);
	return strtod(strCopy.c_str()).andThen([=](const auto& t) -> OptBusinessDays
		{
			if (!isBDays(std::get<char>(t))) return std::nullopt;

			auto end = std::get<const char*>(t);
			if (empty(*end) || (isDays(*end) && empty(*(++end))))
			{
				return BusinessDays{ std::lround(std::get<double>(t)) };
			}

			return std::nullopt;
		});
}

std::string Formatter<BusinessDays>::str(const BusinessDays& value) const
{
	return fmt::format("{}B", value.count());
}

std::string Formatter<BusinessDays>::failToParse(const std::string_view& str) const
{
	return TP::failToParse<BusinessDays>(str, __FILE__, __LINE__);
}

OptWeeks Formatter<Weeks>::tryParse(const std::string_view& str) const
{
	std::string strCopy(str);
	TP::stringutils::trim_copy_std(strCopy);

	return strtod(strCopy.c_str()).andThen
	([](const auto& t) -> OptWeeks
		{
			if (!empty(*std::get<const char*>(t)))
				return std::nullopt;

			if (isWeeks(std::get<char>(t)))
				return Weeks{ std::lround(std::get<double>(t)) };

			if (isDays(std::get<char>(t)))
			{
				const auto d = Days{ std::lround(std::get<double>(t)) };
				const auto ws = std::chrono::floor<Weeks>(d);
				if (d == ws) return ws;
			}

			return std::nullopt;
		});
}

std::string Formatter<Weeks>::str(const Weeks& value) const
{
	return fmt::format("{}W", value.count());
}

std::string Formatter<Weeks>::failToParse(const std::string_view& str) const
{
	return TP::failToParse<Weeks>(str, __FILE__, __LINE__);
}

OptMonths Formatter<Months>::tryParse(const std::string_view& str) const
{
	std::string strCopy(str);
	TP::stringutils::trim_copy_std(strCopy);

	return strtod(strCopy.c_str()).andThen
	([](const auto& t) -> OptMonths
		{
			if (!empty(*std::get<const char*>(t)))
				return std::nullopt;

			if (isMonths(std::get<char>(t)))
				return Months{ std::lround(std::get<double>(t)) };

			if (isYears(std::get<char>(t)))
			{
				double intPart;
				const double fracPart = modf(std::get<double>(t), &intPart);
				return Years{ std::lround(intPart) } + Months{ std::lround(fracPart * 12) }; // 1.25Y will give 1Y3M...
			}

			return std::nullopt;
		});
}

std::string Formatter<Months>::str(const Months& value) const
{
	return fmt::format("{}M", value.count());
}

std::string Formatter<Months>::failToParse(const std::string_view& str) const
{
	return TP::failToParse<Months>(str, __FILE__, __LINE__);
}

OptYears Formatter<Years>::tryParse(const std::string_view& str) const
{
	std::string strCopy(str);
	TP::stringutils::trim_copy_std(strCopy);

	return strtod(strCopy.c_str()).andThen
	([](const auto& t) ->OptYears
		{
			if (!empty(*std::get<const char*>(t)))
				return std::nullopt;

			if (isYears(std::get<char>(t)))
				return Years{ std::lround(std::get<double>(t)) };

			if (isMonths(std::get<char>(t)))
			{
				const auto m = Months{ std::lround(std::get<double>(t)) };
				const auto ys = std::chrono::floor<Years>(m);
				if (m == ys) return ys;
			}

			return std::nullopt;
		});
}
std::string Formatter<Years>::str(const Years& value) const
{
	return fmt::format("{}Y", value.count());
}

std::string Formatter<Years>::failToParse(const std::string_view& str) const
{
	return TP::failToParse<Years>(str, __FILE__, __LINE__);
}

OptDuration Formatter<Duration>::tryParse(const std::string_view& str) const
{
	// this also removes spaces in between the tenors, very useful if we are taking about composite duration. I.e. '1Y 2W'
	std::string copy(str); // todo think if we need the copy maybe str is a rvalue for which we do not need to keep state
	copy.erase(std::remove_if(copy.begin(), copy.end(), isspace), copy.end());

	if (str.length() < 2) return std::nullopt;

	auto cstr = copy.c_str();
	auto error = false;
	Duration d;
	for (auto i = strtod(cstr); !error && i.hasValue(); i = strtod(cstr))
	{
		error = i.transform([&d, &cstr](const auto& t)
			{
				auto c = std::get<char>(t);
				const auto l = std::get<double>(t);
				if (isYears(c))
				{
					double intPart;
					const double fracPart = modf(l, &intPart);
					d += Years{ std::lround(intPart) };
					d += Months{ std::lround(fracPart * 12) }; // we round so 1.33Y gives 16_M
				}
				else if (isMonths(c))
					d += Months{ std::lround(l) };
				else if (isWeeks(c))
				{
					double intPart;
					const double fracPart = modf(l, &intPart);
					d += Weeks{ std::lround(intPart) };
					d += Days{ std::lround(fracPart * 7) }; // 7 days in a week
				}
				else if (isDays(c))
					d += Days{ std::lround(l) };
				else
					return true; // no recognized format so we error

				cstr = std::get<const char*>(t);
				return !(empty(*cstr) || std::isdigit(*cstr) || *cstr == '+' || *cstr == '-');
			})
			.valueOr(false);
	}

	if (*cstr != '\0') return std::nullopt;
	return d;
}

std::string Formatter<Duration>::str(const Duration& duration) const
{
	if (duration == 0_D) return "0D"; // empty duration always falls back on days

	const auto y = duration.years();
	const auto m = duration.months();
	const auto w = duration.weeks();
	const auto d = duration.days();

	std::ostringstream os;

	if (m != 0_M) { os << m + y; }
	else if (y != 0_Y) { os << y; }

	if (d != 0_D) { os << d + w; }
	else if (w != 0_W) { os << w; }

	return os.str();
}

std::string Formatter<Duration>::failToParse(const std::string_view& str) const
{
	return TP::failToParse<Duration>(str, __FILE__, __LINE__);
}
// ReSharper restore CppMemberFunctionMayBeStatic

std::size_t std::hash<Duration>::operator()(Duration const& s) const noexcept
{
	static const std::hash<Months> m;
	static const std::hash<Days> d;

	const auto h1 = m(s.months() + s.years());
	const auto h2 = d(s.days() + s.weeks());
	return h1 ^ (h2 << 1);
}


