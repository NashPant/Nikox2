#include "DateIterator.h"

namespace TP::date_time
{

	DayIterator::DayIterator(const Date& current, const Days& duration)
		: DateIt<DayIterator>(current), duration_(duration) { }

	DayIterator& DayIterator::next(Date& current)
	{
		current += duration_;
		return *this;
	}

	DayIterator& DayIterator::prev(Date& current)
	{
		current -= duration_;
		return *this;
	}

	MonthIterator::MonthIterator(const Date& current, const Months& duration)
		: DateIt<MonthIterator>(current), duration_(duration), day_(day(current)) { }

	MonthIterator& MonthIterator::next(Date& current)
	{
		current += duration_; // this will deal with end of month rule 
		adjustToDay(current);
		return *this;
	}

	MonthIterator& MonthIterator::prev(Date& current)
	{
		current -= duration_;
		adjustToDay(current);
		return *this;
	}

	void MonthIterator::adjustToDay(Date& current)
	{
		const auto offset = day_
			.transform([&current](const auto& d) { return std::max(current.day() - d, 0_D); })
			.valueOr(0_D);

		current -= offset;
	}

	TP::Optional<Day> MonthIterator::day(const Date& date) noexcept
	{
		const auto eom = date.eom();
		auto day = date.day();
		return date != eom && day >= 28d ? TP::makeOptional(day) : std::nullopt;
	}
}
