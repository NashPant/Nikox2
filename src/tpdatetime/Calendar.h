#pragma once

#include "TPMaths/Interval.h"
#include "TPTools/Utils/NotNull.h"
#include "TPTools/Utils/Operators.h"
#include "TPTools/Utils/Typedefs.h"
#include "Date.h"

#include <array>
#include <functional>
#include <set>
#include <string>
#include <vector>

#ifdef _DEBUG
#pragma warning( disable: 4996)
#endif

namespace TP::date_time
{
	FWD_DECLARE_NOTNULLSHARED(Calendar)
	/**
	* \brief base holiday calendar class
	todo: add holiday names so we can expose them subsequently
	*/
	class TPNIKOPOLIS_IMPEXP Calendar : TP::operators::equality_comparable<Calendar>
	{
	public:
		TYPEDEF_CUSTOM_CSTR(std::string, Code, toCode)

		// creates an aggregated calendar code (deals with ordering so NYB+LNB = LNB+NYB)
		friend Code operator+(const Code& x, const Code& y);
		/**
		 * \brief Constructs a calendar from the data provided, does not assume anything from the parameters
		 */
		static CalendarConstPtr construct(Code&& code, const std::set<Date>& holidays, 
						const std::vector<Weekday>& weekendDays = { Sat, Sun });


		template<typename T>
		static T constructT(Code&& code, const std::set<Date>& holidays,
			const std::vector<Weekday>& weekendDays = { Sat, Sun })
		{
			std::array<bool, 7> isWe{ false };
			for (auto w : weekendDays) isWe[static_cast<unsigned>(w.c_encoding())] = true;
			const auto cdr = new Calendar(std::forward<Code>(code), std::vector<Date>(holidays.begin(), holidays.end()), isWe);
			return T(cdr);
		}

		/*
		  @brief. Note this version is dangerous as existing pointer in TPSmartPointer will be released
		*/
		//static CalendarConstPtr fromNikoPtr(const TPSmartPointer<const Calendar>& cached);

		/**
		 * \brief Merges several calendars providing a calendar representing the union of holidays!
		 */
		static CalendarConstPtr merge(const std::vector<CalendarConstPtr>& calendars);


		static std::shared_ptr<const Calendar>  ptr_merge(const std::vector<std::shared_ptr<const Calendar> >& calendars);

		/**
		 * \brief returns the calendar code like 'NYB'
		 */
		[[nodiscard]] const Code& code() const { return code_; }
		[[nodiscard]] const std::string& getID() const { return code_.underlying(); }

		/**
		 * \brief Returns <tt>true</tt> if the weekday/date is a weekend in the market
		 */
		[[nodiscard]] bool isWeekend(const Date& date) const { return isWeekend(date.weekday()); }
		[[nodiscard]] bool isWeekend(const Weekday& weekday) const;

		/**
		 * \brief Returns <tt>true</tt> iff the date is a business day for the given market
		 */
		[[nodiscard]] bool isBusinessDay(const Date& date) const noexcept { return !isHoliday(date); }

		/**
		 * \brief Returns <tt>true</tt> iff the date is a holiday day for the given market
		 */
		[[nodiscard]] bool isHoliday(const Date& date) const noexcept;

		/**
		 * \brief Returns the number of business days in a given Date interval
		 */
		[[nodiscard]] BusinessDays countBusinessDays(const TP::math::Interval<Date>& interval) const;

		/**
		 * \brief Returns the number of business days in [start, end)
		 */
		[[nodiscard]] BusinessDays countBusinessDays(const Date& start, const Date& end) const;

		/**
		 * \brief Returns the last business day of the given month/year
		 */
		[[nodiscard]] Date lastBusinessDay(const YearMonth& ym) const;

		/**
		 * \brief Returns the first business day of the given month/year
		 */
		[[nodiscard]] Date firstBusinessDay(const YearMonth& ym) const;

		/**
		 * \brief Returns <tt>true</tt> if the date is the last business day of the month
		 */
		[[nodiscard]] bool isEom(const Date& date) const;

		/**
		 * \brief Finds the next business day, always returning a later date.
		 */
		[[nodiscard]] Date next(Date date) const;

		/**
		 * \brief Finds the next business day, returning the input date if it is a business day.
		 */
		[[nodiscard]] Date nextOrSame(Date date) const;

		/**
		 * \brief Finds the previous business day, always returning an earlier date.
		 */
		[[nodiscard]] Date previous(Date date) const;

		/**
		 * \brief Finds the previous business day, returning the input date if it is a business day.
		 */
		[[nodiscard]] Date previousOrSame(Date date) const;

		/**
		 * \brief Shifts the date by the specified number of business days.
		 *          if the amount is 0, the date is unadjusted
		 *          if the amount is positive the result date is shifted by the number of business days strictly after current date
		 *          if the amount is negative the result date is shifted by the number of business days strictly before the current date
		 */
		[[nodiscard]] Date shift(Date date, BusinessDays businessDays) const;

		friend bool operator==(const Calendar& x, const Calendar& y) { return x.code() == y.code(); }

		static CalendarConstPtr noHoliday();
		static CalendarConstPtr isoWeekday(); // This calendar assumes that the weekend includes SATURDAY_AND_SUNDAY
		static CalendarConstPtr  everyDayisBusinessDay();
		static CalendarConstPtr  FridaysAndsaturdayWeekend();  // Jewish
		static CalendarConstPtr  fridayWeekend();

		//[[deprecated("Use shift instead"), nodiscard]] 
		Date businessDateAfter(Date fromDate, const unsigned char count) const
		{
			if (count == 0)
				return nextOrSame(fromDate);

			return shift(fromDate, BusinessDays{ count });
		}

		//[[deprecated("Use shift instead"), nodiscard]] 
		Date businessDateBefore(Date fromDate, const unsigned char count) const
		{
			if (count == 0)
				return previousOrSame(fromDate);

			return shift(fromDate, -BusinessDays{ count });
		}

		const std::vector<Date>& holidays() const { return holidays_; }

	private:
		static std::string toCode(const std::string& str);
		[[nodiscard]] int holidayOffset(const Date& d) const;
		[[nodiscard]] int busDaysInBounds(const TP::math::Interval<Date>& interval) const;
		[[nodiscard]] int busDaysOutOfBounds(const TP::math::Interval<Date>& interval) const;

		Calendar(Code&& code, std::vector<Date>&& holidays, std::array<bool, 7> weekend);

		std::vector<Date> holidays_; // list of holidays sorted
		std::vector<unsigned short> accHolidays_; // accumulated holidays (why because we have less holidays than business days so we don't need as much space)
		TP::math::Interval<Date> middle_, left_, right_;

		const Code code_;
		std::array<bool, 7> weekend_; // true for weekends, false otherwise
		unsigned char weekendLength_; // number of weekend days
		// Default copy constructor
		Calendar(const Calendar& rhs) = default;
	};

}

/**
 * \brief hash function for
 */
namespace std
{
	template<>
	struct hash<TP::date_time::Calendar::Code>
	{
		size_t operator()(const TP::date_time::Calendar::Code& e) const noexcept
		{
			static const auto hash = std::hash<std::string>();
			return hash(e);
		}
	};
}
