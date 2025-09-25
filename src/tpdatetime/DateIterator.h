#pragma once
#include "Date.h"
#include "TPTools/Utils/TPOptional.h"
//#include <optional>
#include "TPTools/Utils/Operators.h"
#include "TPNikopolisUtils.h"

#include <iterator>

namespace TP::date_time
{
	/**
 * \brief Date iterators provide a standard mechanism for iteration through dates. Date iterators are are similar to
 *		  Bidirectional Iterators and can be used to populate collections with dates and other date generation tasks.
 *
 *		  Using CRTP to avoid vtable
 */
	template<typename DerivedIt>
	class DateIt
		: TP::operators::totally_ordered<DerivedIt, Date>,
		TP::operators::equality_comparable<DerivedIt>
	{
	public:
		// std compatible
		// ReSharper disable CppInconsistentNaming
		using iterator_category = std::bidirectional_iterator_tag;
		using value_type = Date;
		using pointer = const value_type*;
		using reference = const value_type&;
		// ReSharper restore CppInconsistentNaming

		constexpr reference operator*() const { return current_; }

		constexpr pointer operator->() const { return &current_; }


		constexpr DerivedIt& operator++()
		{
			auto& ptr = static_cast<DerivedIt&>(*this).next(current_);
			return ptr;
		}

		constexpr DerivedIt& operator--()
		{
			auto& ptr = static_cast<DerivedIt&>(*this).prev(current_);
			return ptr;
		}

		constexpr DerivedIt operator++(int)
		{
			DerivedIt tmp(*this);
			++*this;
			return tmp;
		}

		constexpr DerivedIt operator--(int)
		{
			DerivedIt tmp(*this);
			--*this;
			return tmp;
		}

		// those are borrowed from boost date_iter class and are useful to stop iterations in while loops
		[[nodiscard]] friend bool operator==(const DerivedIt& lhs, const Date& rhs) noexcept { return lhs.current_ == rhs; }
		[[nodiscard]] friend bool operator<(const DerivedIt& lhs, const Date& rhs) noexcept { return lhs.current_ < rhs; }

		[[nodiscard]] friend bool operator==(const DerivedIt& lhs, const DerivedIt& rhs) noexcept { return lhs.current_ == rhs.current_; }

	protected:
		friend DerivedIt;

		DateIt() = default;
		DateIt(const Date& date) : current_(date) {}
		Date current_;
	};

	/**
	 * \brief Iterate a number of days or weeks at a time
	 */
	class   DayIterator final : DateIt<DayIterator>
	{
	public:
		DayIterator() = default;

		// remember weeks is just 7 days so let chrono take care of the implicit conversion
		DayIterator(const Date& current, const Days& duration);

		DayIterator& next(Date& current);

		DayIterator& prev(Date& current);

	private:
		Days duration_{};
	};

	/**
	 * \brief Iterate a number of months and years at a time (this implementation closely follows boost and
	 *  avoids reversibility of operations pitfall https://www.boost.org/doc/libs/1_55_0/doc/html/date_time/gregorian.html#date_time.gregorian.date_period)
	 */
	class   MonthIterator final : DateIt<MonthIterator>
	{
	public:
		MonthIterator() = default;

		// remember years are just 12 months so let chrono take care of the implicit conversion
		MonthIterator(const Date& current, const Months& duration);

		MonthIterator& next(Date& current);

		MonthIterator& prev(Date& current);

	private:
		void adjustToDay(Date& current);

		static Optional<Day> day(const Date& date) noexcept;

		Months duration_{};
		Optional<Day> day_{};
	};
}
