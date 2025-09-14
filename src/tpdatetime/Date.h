#pragma once

#include <TPNikopolisUtils.h>
#include "Durations.h"
#include "YearMonthDay.h"
#include "Clocks.h"
#include "DateRules.h"
#include "TPTools/Utils/Operators.h"
#include "TPTools/Utils/Formatting.h"
#include "TPTools/Utils/TypeTraits.h"

//#include <date/date.h>  // do not expose any of these types directly (we want to be able to change once date becomes standard)
//#include <include/date/date.h>

#include <chrono>
#include <ctime>
#include <string>
#include <functional>
#include <ostream>
#include <type_traits>


class TPDate;

namespace TP::date_time
{
	class Date;
	using OptDate = TP::Optional<Date>;

	/**
	 * @brief structure that holds the date in serial form and has all functionality to handle date based logic
	 */
	class TPNIKOPOLIS_IMPEXP  Date final
	  : TP::operators::totally_ordered<Date>
	{
	public:
		// ReSharper disable CppInconsistentNaming
		typedef Days difference_type; // needed for boost icl
		// ReSharper restore CppInconsistentNaming

		/**
		 * \brief provides today's date based on system clock
		 */
		static Date sysToday() noexcept;

		static constexpr Date from_sys_days(const std::chrono::sys_days& tp) noexcept {
			return Date(tp);
		}

		/**
		 * \brief provides today's date based on local clock
		 */
		 // todo remove. This function should not exist we should have a local date and convert to system date accurately (using time conversion to system clock) but Date is not doing a good job at the moment
		static Date localToday() noexcept;

		/**
		 * \brief creates a date from string. Can throw if date is not in correct format or invalid
		 * \param date the date string e.g. "12/01/2000"
		 * \param fmt the format representation of the date string, e.g. "%m/%d/%Y" for mm/dd/yyyy
		 */
		explicit Date(const std::string& date, const std::string& fmt = defaultFormat);

		/// \brief Construct from a duration since epoch (e.g. days).
		constexpr explicit Date(std::chrono::sys_days::duration d) noexcept
			: serial_{ std::chrono::sys_days{d} } {
		}
		/**
		 * \brief Will throw for invalid dates (e.g. day = 32, month 13)
		 */
		Date(Year year, Month month, Day day);
		Date(Day day, Month month, Year year);
		//Date(const TPDate& tpd);
		/**
		 * \brief Allows to build dates from the nice Hinnant semantics 2000_y / January / 1_d
		 * this will throw for invalid year, month or day
		 */
		Date(const YearMonthDay& ymd);

		/**
		 * \brief Allows to build dates from the nice Hinnant semantics 2000_y / January / last (last day of January 2000)
		 * this will throw for invalid year or month
		 */
		Date(const YearMonthDayLast& ymd);

		/**
		 * \brief Allows to build dates from the nice Hinnant semantics 2000_y / January / Sunday[3] (3rd sunday of Jan 2000)
		 * this will throw if invalid. E.g. tue[5] / sep / 2019_y
		 */
		Date(const YearMonthWeekday& ymwi);

		/**
		 * \brief Allows to build dates from the nice Hinnant semantics 2000_y / January / Sunday[last] (last sunday of Jan 2000)
		 * will throw for invalid month
		 */
		Date(const YearMonthWeekdayLast& ymwl);

		// ReSharper restore CppNonExplicitConvertingConstructor

		/**
		 * \brief creates a date from a tm. Will throw if date is invalid
		 */
		explicit Date(const tm& tm);

		/**
		 * \brief creates a date from a time point.
		 */
		explicit Date(const std::chrono::system_clock::time_point& time);

		/// \brief Returns the duration since the epoch (1970-01-01) in days,
		///      consistent with std::chrono::time_point::time_since_epoch().
		[[nodiscard]] constexpr std::chrono::days time_since_epoch() const noexcept {
			return serial_.time_since_epoch();
		}

		/**
		 * \brief Creates a date from an Excel serial (including the 1900 issue)
		 */
		explicit Date(const ExcelDate& tp);

		static OptDate tryFromExcel(const ExcelDate& tp);

		Date() : serial_(0_D) {}

		Date(const Date&) = default;

		Date(Date&&) = default;

		Date& operator =(const Date&) = default;

		Date& operator =(Date&&) = default;

		~Date() = default;

		/*
		 @ @brief. Trivial function really. Not sure if needed but keeping some consistency with boost date
				   Issue here is that 0_D is a valid date.
		*/
		bool isNull() const { return serial_ == std::chrono::sys_days(0_D); }

		[[nodiscard]] constexpr std::chrono::sys_days to_sys_days() const noexcept {
			return serial_;
		}

		[[nodiscard]] YearMonthDay ymd() const noexcept;
		[[nodiscard]] YearMonth ym() const noexcept;
		[[nodiscard]] Year year() const noexcept;
		[[nodiscard]] Month month() const noexcept;
		[[nodiscard]] Day day() const noexcept;
		[[nodiscard]] Weekday weekday() const noexcept;
		[[nodiscard]] Date eom() const noexcept; // end of current month date
		[[nodiscard]] Date eoq() const noexcept; // end of current quarter (always after date)
		[[nodiscard]] Date eoy() const noexcept; // end of current year

		static constexpr Date(min)() noexcept { return Date(std::chrono::sys_days{ Year::min() / Jan / 1 }); }
		static constexpr Date(max)() noexcept { return Date(std::chrono::sys_days{ Year::max() / Dec / last }); }

		/**
		 * \brief creates a tm from date.
		 */
		[[nodiscard]] std::tm toTm() const noexcept;

		/**
		 * \brief returns the string representation of the current date
		 * \param fmt: the format representation of the date string, e.g. "%m/%d/%Y" for mm/dd/yyyy
		 */
		[[nodiscard]] std::string toString(const std::string& fmt = defaultFormat) const;

		/**
		 * \brief converts current date to Excel equivalent serial (number of days since excel epoch)
		 */
		[[nodiscard]] ExcelDate toExcel() const;

		[[nodiscard]] friend  auto operator==(const Date& lhs, const Date& rhs) noexcept { return lhs.serial_ == rhs.serial_; }
		[[nodiscard]] friend  auto operator<(const Date& lhs, const Date& rhs) noexcept { return lhs.serial_ < rhs.serial_; }
		[[nodiscard]] friend  auto operator-(const Date& lhs, const Date& rhs) noexcept { return lhs.serial_ - rhs.serial_; }

		 Date& operator+=(const Days& days) noexcept { serial_ += days; return *this; }
		 Date& operator-=(const Days& days) noexcept { serial_ -= days; return *this; }
		 Date& operator+=(const Weeks& weeks) noexcept { serial_ += weeks; return *this; }
		 Date& operator-=(const Weeks& weeks) noexcept { serial_ -= weeks; return *this; }

		/**
		 * \brief Adds a number of months to the current date returning valid dates preserving end of month.
		 * \note  This end of month rule means that when this date is the last day of the month, adding or subtracting any number of months will give a
		 *			result that is also the last day of the month. For any other date adding and subtracting months will give the same day as current date if valid
		 *			 or last day of the month
		 *
		 * \return	this date after adding the relevant months
		 */
		Date& operator+=(const Months& months) noexcept;
		Date& operator-=(const Months& months) noexcept { return *this += -months; }

		/**
		 * \brief  Adds a number of years to the current date returning valid dates preserving end of month (consistent with months logic).
		 * \note  This end of month rule means that when this date is the last day of February, adding or subtracting any number of years will give a
		 *			result that is also the last day of February (accounting for leap years). For any other date adding and subtracting years will give a date
		 *			with the same day and month as current date
		 * \return	this date after adding the relevant years
		 */
		Date& operator+=(const Years& years) noexcept;
		Date& operator-=(const Years& years) noexcept { return *this += -years; }

		/**
		 * \brief  Adds specified duration to the current date returning valid dates preserving end of month (consistent with months logic).
		 * \note  This end of month rule means that when this date is the last day of February, adding or subtracting any number of years will give a
		 *			result that is also the last day of February (accounting for leap years). For any other date adding and subtracting years will give a date
		 *			with the same day and month as current date
		 * \return	this date after adding the relevant years
		 */
		Date& operator+=(const Duration& period) noexcept { return (*this += period.months_) += period.days_; }
		Date& operator-=(const Duration& period) noexcept { return *this += -period; }

		 Date operator+(const Days& days)		const noexcept { return Date(*this) += days; }//constexpr
		 Date operator+(const Weeks& weeks)	const noexcept { return Date(*this) += weeks; }//constexpr
		Date operator+(const Months& months)	const noexcept { return Date(*this) += months; }
		Date operator+(const Years& years)	const noexcept { return Date(*this) += years; }
		Date operator+(const Duration& d)		const noexcept { return Date(*this) += d; }

		 Date operator-(const Days& days)		const noexcept { return Date(*this) -= days; } //constexpr
		 Date operator-(const Weeks& weeks)	const noexcept { return Date(*this) -= weeks; }//constexpr
		Date operator-(const Months& months)	const noexcept { return Date(*this) -= months; }
		Date operator-(const Years& years)	const noexcept { return Date(*this) -= years; }
		Date operator-(const Duration& d)		const noexcept { return Date(*this) -= d; }

		/**
		 * \brief gets the next date that will have a weekday equal to the input.
		 * E.g. if date is Wednesday 4 Sep 2019 then next Saturday is 7 Sep 2019
		 */
		template <typename Policy = std::remove_cv<decltype(rule::weekday::next)>::type>
		[[nodiscard]] Date next(const Weekday& wd, const Policy& policy = rule::weekday::next) const noexcept;

		/**
		 * \brief gets the previous date that will have a weekday equal to the input.
		 * E.g. if date is 4 Sep 2019 then previous Saturday is 31 Aug 2019
		 */
		template <typename Policy = std::remove_cv<decltype(rule::weekday::next)>::type>
		[[nodiscard]] Date previous(const Weekday& wd, const Policy& policy = rule::weekday::next) const noexcept;

		/**
		 * \brief gets the next date that will have a month equal to the input (subsequent or same year and same day).
		 * E.g. if date is 4 Sep 2019 then next January is 4 Jan 2020
		 */
		template <typename Policy = std::remove_cv<decltype(rule::month::next)>::type>
		[[nodiscard]] Date next(const Month& month, const Policy& policy = rule::month::next) const noexcept;

		/**
		 * \brief gets the previous date that will have a month equal to the input (previous or same year and same day).
		 * E.g. if date is 4 Sep 2019 then previous October is 4 Oct 2018
		 */
		template <typename Policy = std::remove_cv<decltype(rule::month::next)>::type>
		[[nodiscard]] Date previous(const Month& month, const Policy& policy = rule::month::next) const noexcept;

		/**
		 * \brief gets the next date that will have a month and day equal to the input (subsequent or same year).
		 * E.g. if date is 4 Sep 2019 then next January 10th is 10 Jan 2020
		 */
		template <typename T, typename Policy = std::remove_cv<decltype(rule::month_day::next)>::type,
			std::enable_if_t<std::disjunction_v<std::is_same<T, MonthDay>, std::is_same<T, MonthDayLast>>, int> = 0>
			[[nodiscard]] Date next(const T& md, const Policy& policy = rule::month_day::next) const noexcept;

		/**
		 * \brief gets the previous date that will have a month and day equal to the input (previous or same year).
		 * E.g. if date is 4 Sep 2019 then previous September 5th is 5 Sep 2018
		 */
		template <typename T, typename Policy = std::remove_cv<decltype(rule::month_day::next)>::type,
			std::enable_if_t<std::disjunction_v<std::is_same<T, MonthDay>, std::is_same<T, MonthDayLast>>, int> = 0>
			[[nodiscard]] Date previous(const T& md, const Policy& policy = rule::month_day::next) const noexcept;

		/**
		 * \brief Adds a duration to the current date returning valid dates and using provided end of month rule.
		 *
		 * \return	this date after adding the relevant years
		 */
		template <typename Policy = decltype(rule::eom::preserve)>
		[[nodiscard]] Date add(const Duration& period, const Policy& policy = rule::eom::preserve) const noexcept;

		/**
		 * \brief Adds a number of months/years to the current date returning valid dates and using provided end of month rule.
		 *
		 * \return	this date after adding the relevant months/years
		 */
		template <typename T, typename Policy = decltype(rule::eom::preserve)>
		[[nodiscard]] Date add(const T& duration, const Policy& policy = rule::eom::preserve) const noexcept;

		/**
		 * \brief Same as previous add methods but with runtime rule support
		 */
		template <typename T>
		[[nodiscard]] Date add(const T& duration, bool eomRule) const noexcept;

		[[nodiscard]] Date add(const Duration& duration, bool eomRule) const noexcept;

		Date& operator++()
		{
			serial_ += 1_D;
			return *this;
		}

		Date operator++(int)
		{
			const Date d{ serial_ };
			++*this;
			return d;
		}

		Date& operator--()
		{
			serial_ -= 1_D;
			return *this;
		}

		Date operator--(int)
		{
			const Date d{ serial_ };
			--*this;
			return d;
		}


	private:
		std::chrono::sys_days serial_;

		constexpr explicit Date(const std::chrono::sys_days& tp) : serial_(tp) {}

		friend struct std::hash<Date>;
		const static std::string defaultFormat;// = "%Y%m%d";

		[[nodiscard]] std::chrono::year_month_day toYmd() const;
		static std::chrono::sys_days toSerial(const std::chrono::year_month_day& ymd);
	};

	template <typename Policy>
	Date Date::next(const Weekday& wd, const Policy& policy) const noexcept
	{
		static_assert(rule::weekday::isWeekdayRule<Policy>(), "Unsupported policy type class");
		return *this + policy(this->weekday(), wd);
	}

	template <typename Policy>
	Date Date::previous(const Weekday& wd, const Policy& policy) const noexcept
	{
		static_assert(rule::weekday::isWeekdayRule<Policy>(), "Unsupported policy type class");
		return *this - policy(wd, this->weekday());
	}

	template <typename Policy>
	Date Date::next(const Month& month, const Policy& policy) const noexcept
	{
		static_assert(rule::month::isMonthRule<Policy>(), "Unsupported policy type class");
		return *this + policy(this->month(), month);
	}

	template <typename Policy>
	Date Date::previous(const Month& month, const Policy& policy) const noexcept
	{
		static_assert(rule::month::isMonthRule<Policy>(), "Unsupported policy type class");
		return *this - policy(month, this->month());
	}

	template <typename T, typename Policy,
		std::enable_if_t<std::disjunction_v<std::is_same<T, MonthDay>, std::is_same<T, MonthDayLast>>, int>
	>
		Date Date::next(const T& md, const Policy& policy) const noexcept
	{
		static_assert(rule::month_day::isMonthDayRule<Policy>(), "Unsupported policy type class");

		const auto ymd = this->ymd();
		const YearMonthDay x = ymd.year() / md;
		return x < ymd ? x + 1_Y : (x + policy(x, ymd));
	}

	template <typename T, typename Policy,
		std::enable_if_t<std::disjunction_v<std::is_same<T, MonthDay>, std::is_same<T, MonthDayLast>>, int>
	>
		Date Date::previous(const T& md, const Policy& policy) const noexcept
	{
		static_assert(rule::month_day::isMonthDayRule<Policy>(), "Unsupported policy type class");

		const auto ymd = this->ymd();
		const YearMonthDay x = ymd.year() / md;
		return x > ymd ? x - 1_Y : (x - policy(x, ymd));
	}

	template <typename T, typename Policy>
	Date Date::add(const T& duration, const Policy& policy) const noexcept
	{
		static_assert(TP::DecayEquiv<T, Months>::value || TP::DecayEquiv<T, Years>::value,
			"Only addition with Years or Months is supported");
		return Date(toSerial(policy(toYmd(), duration)));
	}

	template <typename Policy>
	Date Date::add(const Duration& period, const Policy& policy) const noexcept
	{
		auto d = this->add(period.months_, policy);
		return d += period.days_;
	}

	template <typename T>
	Date Date::add(const T& duration, const bool eomRule) const noexcept
	{
		using namespace rule::eom;
		static_assert(TP::DecayEquiv<T, Months>::value || TP::DecayEquiv<T, Years>::value || TP::DecayEquiv<T, Duration>::value,
			"Only addition with Years, Months or Durations is supported");
		return eomRule == preserve ? add(duration, preserve) : add(duration, do_not_preserve);
	}

	inline Date Date::add(const Duration& duration, const bool eomRule) const noexcept
	{
		using namespace rule::eom;
		return eomRule == preserve ? add(duration, preserve) : add(duration, do_not_preserve);
	}

	inline    Date operator+(const Days& days, Date d) { return d += days; }//constexpr
	inline    Date operator+(const Weeks& weeks, Date d) { return d += weeks; }//constexpr
	inline    Date operator+(const Months& months, Date d) { return d += months; }
	inline    Date operator+(const Years& years, Date d) { return d += years; }
	inline    Date operator+(const Duration& dur, Date d) { return d += dur; }
}

namespace TP
{
	inline namespace formatting
	{
		//namespace date_time = TP::date_time;

		template<>
		struct TPNIKOPOLIS_IMPEXP Formatter<date_time::Date>
		{
			explicit Formatter(Optional<std::string> fmtOverride = std::nullopt);

			[[nodiscard]] Optional<date_time::Date> tryParse(const std::string& str) const;
			[[nodiscard]] std::string str(const date_time::Date& value) const;
			[[nodiscard]] std::string failToParse(const std::string& str) const;

		private:
			Formatter<date_time::YearMonthDay> formatter_;
		};
	}

}

namespace TP::date_time
{
	OSTREAM_FROM_TP(Date)
}

DECLARE_FMT_FORMATTER_FROM_TP(TP::date_time::Date)

namespace std  // NOLINT
{
	//using namespace TP::date_time;

	template<>
	struct hash<Date>
	{
		size_t operator()(const Date& date) const noexcept
		{
			const static std::hash<int> hash{};
			return hash(date.serial_.time_since_epoch().count());
		}
	};
}
