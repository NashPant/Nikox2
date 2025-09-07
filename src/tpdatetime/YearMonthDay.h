#pragma once

#include "TPNikopolisUtils.h"
#include "TPTools/Utils/Formatting.h"
#include "TPTools/Utils/TPOptional.h"
//#include <include/date/date.h>
#include <chrono>

#include <string>

namespace TP::date_time
{
	using Year = std::chrono::year;

	inline namespace literals
	{
		using std::chrono::operator ""y;
	}

	using Month = std::chrono::month;

	inline namespace literals
	{
		using std::chrono::January;
		using std::chrono::February;
		using std::chrono::March;
		using std::chrono::April;
		using std::chrono::May;
		using std::chrono::June;
		using std::chrono::July;
		using std::chrono::August;
		using std::chrono::September;
		using std::chrono::October;
		using std::chrono::November;
		using std::chrono::December;


		// ReSharper disable CppInconsistentNaming
		constexpr auto Jan = January;
		constexpr auto Feb = February;
		constexpr auto Mar = March;
		constexpr auto Apr = April;
		constexpr auto Jun = June;
		constexpr auto Jul = July;
		constexpr auto Aug = August;
		constexpr auto Sep = September;
		constexpr auto Oct = October;
		constexpr auto Nov = November;
		constexpr auto Dec = December;
		// ReSharper restore CppInconsistentNaming
	}

	using Day = std::chrono::day;

	inline namespace literals
	{
		constexpr auto last = std::chrono::last;

		using std::chrono::operator ""d;
	}

	using Weekday = std::chrono::weekday;

	inline namespace literals
	{
		using std::chrono::Monday;
		using std::chrono::Tuesday;
		using std::chrono::Wednesday;
		using std::chrono::Thursday;
		using std::chrono::Friday;
		using std::chrono::Saturday;
		using std::chrono::Sunday;

		// ReSharper disable CppInconsistentNaming
		//constexpr auto Sun = sun;
		//constexpr auto Mon = mon;
		//constexpr auto Tue = tue;
		//constexpr auto Wed = wed;
		//constexpr auto Thu = thu;
		//constexpr auto Fri = fri;
		//constexpr auto Sat = sat;
		// ReSharper restore CppInconsistentNaming
	}

	using YearMonthDay = std::chrono::year_month_day;
	using YearMonthDayLast = std::chrono::year_month_day_last;
	using YearMonthWeekday = std::chrono::year_month_weekday;
	using YearMonthWeekdayLast = std::chrono::year_month_weekday_last;

	using YearMonth = std::chrono::year_month;
	using MonthDay = std::chrono::month_day;
	using MonthDayLast = std::chrono::month_day_last;

	using WeekdayIndexed = std::chrono::weekday_indexed;
}

namespace TP
{
	inline namespace formatting
	{
		//namespace date_time = TP::date_time;

		template<>
		struct Formatter<date_time::Day>
		{
			explicit Formatter(Optional<std::string_view> fmtOverride = std::nullopt);

			[[nodiscard]] Optional<date_time::Day> tryParse(const std::string_view& str) const;
			[[nodiscard]] std::string str(const date_time::Day& value) const;
			[[nodiscard]] std::string failToParse(const std::string_view& str) const;

			// checks if str can be converted to a day of month in numeric format
			static bool isNumeric(const std::string_view& str);

		private:
			Optional<std::string_view> override_;
		};

		template<>
		struct Formatter<date_time::Month>
		{
			explicit Formatter(Optional<std::string_view> fmtOverride = std::nullopt);

			[[nodiscard]] Optional<date_time::Month> tryParse(const std::string_view& str) const;
			[[nodiscard]] std::string str(const date_time::Month& value) const;
			[[nodiscard]] std::string failToParse(const std::string_view& str) const;

			// checks if str can be converted to a month in numeric format
			static bool isNumeric(const std::string_view& str);

			static Optional<date_time::Month> bloombergCode(char c);

		private:
			Optional<std::string_view> override_;
		};

		template<>
		struct Formatter<date_time::Year>
		{
			explicit Formatter(Optional<std::string_view> fmtOverride = std::nullopt, Optional<date_time::YearMonthDay> asOf = std::nullopt);

			[[nodiscard]] Optional<date_time::Year> tryParse(const std::string_view& str) const;
			[[nodiscard]] std::string str(const date_time::Year& value) const;
			[[nodiscard]] std::string failToParse(const std::string_view& str) const;

		private:
			Optional<std::string_view> override_;
			Optional<date_time::YearMonthDay> asOf_;
		};

		template<>
		struct TPNIKOPOLIS_IMPEXP Formatter<date_time::Weekday>
		{
			explicit Formatter(Optional<std::string_view> fmtOverride = std::nullopt);

			[[nodiscard]] Optional<date_time::Weekday> tryParse(const std::string_view& str) const;
			[[nodiscard]] std::string str(const date_time::Weekday& value) const;
			[[nodiscard]] std::string failToParse(const std::string_view& str) const;

		private:
			Optional<std::string_view> override_;
		};

		template<>
		struct Formatter<date_time::YearMonth>
		{
			explicit Formatter(Optional<std::string_view> fmtOverride = std::nullopt, Optional<date_time::YearMonthDay> asOf = std::nullopt);

			[[nodiscard]] Optional<date_time::YearMonth> tryParse(const std::string_view& str) const;
			[[nodiscard]] std::string str(const date_time::YearMonth& value) const;
			[[nodiscard]] std::string failToParse(const std::string_view& str) const;

		private:
			Optional<std::string_view> override_;
			Optional<date_time::YearMonthDay> asOf_;
		};

		template<>
		struct Formatter<date_time::YearMonthDay>
		{
			explicit Formatter(Optional<std::string_view> fmtOverride = std::nullopt);

			[[nodiscard]] Optional<date_time::YearMonthDay> tryParse(const std::string_view& str) const;
			[[nodiscard]] std::string str(const date_time::YearMonthDay& value) const;
			[[nodiscard]] std::string failToParse(const std::string_view& str) const;

		private:
			Optional<std::string_view> override_;
		};
	}
}

DECLARE_FMT_FORMATTER_FROM_TP(TP::date_time::Day)
DECLARE_FMT_FORMATTER_FROM_TP(TP::date_time::Month)
DECLARE_FMT_FORMATTER_FROM_TP(TP::date_time::Year)
DECLARE_FMT_FORMATTER_FROM_TP(TP::date_time::Weekday)
DECLARE_FMT_FORMATTER_FROM_TP(TP::date_time::YearMonth)

