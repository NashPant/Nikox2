#include "BusinessDay.h"

#include "TPTools/StringUtils/TPStringUtils.h"
#include "TPDateTime/Calendar.h"
#include "TPDateTime/financial/DateAdjustment.h"
#include <chrono>
#include <format>

using namespace TP::formatting;
using namespace TP::date_time;
using namespace TP::stringutils;

using TP::Optional;

template<typename T>
class RollAdjustment final : public DateAdjustment
{
public:
	explicit RollAdjustment(const T& md, const bool preserve)
		: md_(md), preserve_(preserve) {}

	[[nodiscard]] Date adjust(const Date& date) const override
	{
		return preserve_ ? date.next(md_, rule::month_day::same) : date.next(md_, rule::month_day::next);
	}
	[[nodiscard]] std::string getType() const override { return "RollAdjustment"; }
private:
	T md_;
	bool preserve_;
};

template<typename T>
auto makeRollAdjustment(const T& md, bool rollRule)
{
	return TP::utils::makeNotNullShared<RollAdjustment<T>>(md, rollRule);
}

DateAdjustmentConstPtr MonthBusinessDay::adjustment(const CalendarConstPtr& cdr, bool rollRule) const
{
	return DateAdjustment::compose({
			std::visit([rollRule](const auto& md) -> DateAdjustmentConstPtr
			{
				return makeRollAdjustment(md, rollRule);
			}, md_),
			DateAdjustment::modifiedFollowing(cdr)
		});
}

Formatter<MonthBusinessDay, void>::Formatter(TP::Optional<std::string> fmtOverride)
	: override_(std::move(fmtOverride)) {}

namespace
{
	template<typename T>
	TP::Optional<T> hinnantParser(const std::string& str, const std::string& fmt, T& out)
	{
		std::istringstream in{ str };
		in >> std::chrono::parse(fmt, out);
		return out.ok() ? TP::makeOptional(std::move(out)) : std::nullopt;
	}

	bool isEoMPrefix(const std::string_view& str)
	{
		return iStartsWith(str, "ME") || iStartsWith(str, "EO");
	}

	bool isBoMPrefix(const std::string_view& str)
	{
		return iStartsWith(str, "MB") || iStartsWith(str, "BO");
	}

	std::string mdFmt(const std::string_view& str)
	{
		if (str.length() < 3) return "";
		switch (str.size())
		{
		case 3:
			if (Formatter<Month>::bloombergCode(str[2]))
			{
				if (isEoMPrefix(str)) return "ME%c";
				if (isBoMPrefix(str)) return "MB%c";
			}
			return "";

		case 4:
			if (Formatter<Month>::isNumeric(str.substr(2, 2)))
			{
				if (isEoMPrefix(str)) return "ME%m";
				if (isBoMPrefix(str)) return "MB%m";
			}
			return "";

		case 5:
			if (std::ispunct(str[2]))
			{
				if (isEoMPrefix(str)) return { 'M', 'E', str[2], '%', 'm', '\0' };
				if (isBoMPrefix(str)) return { 'M', 'B', str[2], '%', 'm', '\0' };
			}
			if (isEoMPrefix(str)) return "ME%b";
			if (isBoMPrefix(str)) return "MB%b";
			if (Formatter<Day>::isNumeric(str)) return "%d%b";
			if (Formatter<Day>::isNumeric(str.substr(3, 2))) return "%b%d";
			return "";

		case 6:
			if (isEoMPrefix(str))
			{
				auto ispunct = std::ispunct(str[2]);
				if (ispunct && std::ispunct(str[5])) return { 'M', 'E', str[2], '%', 'm', str[5], '\0' };
				if (ispunct) return { 'M', 'E', str[2], '%', 'b', '\0' };
				return  "ME%B";
			}
			if (isBoMPrefix(str))
			{
				auto ispunct = std::ispunct(str[2]);
				if (ispunct && std::ispunct(str[5])) return { 'M', 'B', str[2], '%', 'm', str[5], '\0' };
				if (ispunct) return { 'M', 'B', str[2], '%', 'b', '\0' };
				return  "MB%B";
			}
			{
				auto ispunct = std::ispunct(str[2]);
				if (Formatter<Day>::isNumeric(str) && ispunct) return  { '%', 'd', str[2], '%', 'b', '\0' };
				if (Formatter<Day>::isNumeric(str.substr(3, 2)) && ispunct) return  { '%', 'b', str[2], '%', 'd', '\0' };
				return "";
			}

		default:
			return std::isalpha(str[0]) ? "%B%d" : "%d%B";
		}
	}
}

TP::Optional<MonthBusinessDay> Formatter<MonthBusinessDay>::tryParse(const std::string_view& str) const
{
	const auto fmt = override_.valueOr([&str]() { return mdFmt(str); });

	if (fmt.length() < 4) return std::nullopt;

	if (isBoMPrefix(fmt))
	{
		return Formatter<Month>(fmt.substr(2))
			.tryParse(std::string(str.substr(2)))
			.transform([](const auto m) { return Beginning.of(m); });
	}

	if (isEoMPrefix(fmt))
	{
		return Formatter<Month>(fmt.substr(2))
			.tryParse(std::string(str.substr(2)))
			.transform([](const auto m) { return End.of(m); });
	}

	auto tp = 0d / Month{ 0 };
	return hinnantParser(toCamelCase(std::string(str)), fmt, tp); // delegate to date
}

std::string Formatter<MonthBusinessDay>::str(const MonthBusinessDay& value) const
{
	return std::visit(TP::Overloaded
		{
			[this](const std::chrono::month_day& md)
			{
				if (md.day() != 1d) 
					//return date::format(std::locale(), override_.valueOr("%b%d"), md);
					return std::vformat(std::locale(), override_.valueOr("%b%d"), std::make_format_args(md));

				const auto fmt = override_.valueOr("MB%b");
				if (isBoMPrefix(fmt) && fmt.length() >= 4)
				{
					const auto formatter = Formatter<Month>(fmt.substr(2));
					return fmt::format("{}{}", fmt.substr(0,2), formatter.str(md.month()));
				}

				//return date::format(std::locale(), fmt, md);
				return std::vformat(std::locale(), fmt, std::make_format_args(md));
			},

			[this](const std::chrono::month_day_last& md)
			{
				const auto fmt = override_.valueOr("ME%b");
				if (fmt.length() >= 4)
				{
					const auto formatter = Formatter<Month>(fmt.substr(2));
					return fmt::format("{}{}", fmt.substr(0,2), formatter.str(md.month()));
				}

				const auto formatter = Formatter<Month>();
				//return date::format(std::locale(), "ME%b", md.month());
				//return std::vformat(std::locale(), "ME%b", std::make_format_args(md.month()));
				return std::format(std::locale(), "{:%b %d}", md);
				//std::ostringstream oss;
				//oss.imbue(std::locale());
				//oss << std::format("{:%b}", md.month());
				//return oss.str();
			}
		},
		value.md_);
}

std::string Formatter<MonthBusinessDay>::failToParse(const std::string_view& str) const
{
	return TP::failToParse<MonthBusinessDay>(str, __FILE__, __LINE__);
}

// ReSharper restore CppRedundantQualifier
std::size_t std::hash<MonthBusinessDay>::operator()(MonthBusinessDay const& s) const
{
	const auto hash = std::hash<unsigned>();
	const auto h1 = hash(static_cast<unsigned>(s.month()));

	return std::visit(TP::Overloaded
		{
			[h1, &hash](const std::chrono::month_day& md)
			{
				return h1 ^ hash(static_cast<unsigned>(md.day())) << 1;
			},
			[h1, &hash](std::chrono::month_day_last)
			{
				return h1 ^ hash(32) << 1;
			}
		}, s.md_);
}

