#include "Calendar.h"

#include "TPTools/StringUtils/TPStringUtils.h"
//#include "common/math/Interval.h"
#include "Durations.h"
#include "YearMonthDay.h"

#include <boost/icl/split_interval_set.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/tokenizer.hpp>
#include <ranges>

#include <chrono>
#include <numeric>
#include <fmt/format.h>  // Main header that includes join
// OR
#include <fmt/ranges.h>  // Specifically for range formatting (includes join)


using namespace TP::math;

namespace TP::date_time
{
	Calendar::Code operator+(const Calendar::Code& x, const Calendar::Code& y)
	{
		if (x == y) return x;

		typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
		static const boost::char_separator<char> sep("+");

		const tokenizer tx(x.underlying(), sep), ty(y.underlying(), sep);
		std::set<std::string> res(tx.begin(), tx.end());
		res.insert(ty.begin(), ty.end());

		//return  format("{}", fmt::join(res.begin(), res.end(), "+"));
		return format("{}", boost::algorithm::join(res, "+"));
	}

	CalendarConstPtr Calendar::construct(Code&& code, const std::set<Date>& holidays, const std::vector<Weekday>& weekendDays)
	{
		std::array<bool, 7> isWe{ false };
		for (auto w : weekendDays) isWe[static_cast<unsigned>(w.c_encoding())] = true;
		const auto cdr = new Calendar(std::forward<Code>(code), std::vector<Date>(holidays.begin(), holidays.end()), isWe);
		return CalendarConstPtr(cdr);
	}



	CalendarConstPtr Calendar::merge(const std::vector<CalendarConstPtr>& calendars)
	{
		std::string composition;
		std::set<Code> codes;
		std::set<Date> holidays;
		std::array<bool, 7> isWe{ false };
		for (const auto& cdr : calendars)
		{
			codes.insert(cdr->code());
			holidays.insert(cdr->holidays_.begin(), cdr->holidays_.end());
			for (auto i = 0; i < 7; ++i) isWe[i] = isWe[i] | cdr->weekend_[i];
			composition += cdr->code().underlying() + "+";
		}
		// remove the last +
		auto st = composition.substr(0, composition.size() - 1);
		Code code(st);  //= format("{}", fmt::join(codes.begin(), codes.end(), "+"));
		const auto cdr = new Calendar(std::move(code), std::vector<Date>(holidays.begin(), holidays.end()), isWe);
		return CalendarConstPtr(cdr);
	}



	std::shared_ptr<const Calendar>  Calendar::ptr_merge(const std::vector<std::shared_ptr<const Calendar> >& calendars)
	{
		std::string composition;
		std::set<Code> codes;
		std::set<Date> holidays;
		std::array<bool, 7> isWe{ false };
		for (const auto& cdr : calendars)
		{
			codes.insert(cdr->code());
			holidays.insert(cdr->holidays_.begin(), cdr->holidays_.end());
			for (auto i = 0; i < 7; ++i) isWe[i] = isWe[i] | cdr->weekend_[i];
			composition += cdr->code().underlying() + "+";
		}
		// remove the last +
		auto st = composition.substr(0, composition.size() - 1);
		Code code(st);  //= format("{}", fmt::join(codes.begin(), codes.end(), "+"));
		const auto cdr = new Calendar(std::move(code), std::vector<Date>(holidays.begin(), holidays.end()), isWe);
		return  std::shared_ptr<const Calendar>(cdr);

	}

	//CalendarConstPtr Calendar::fromNikoPtr(const TPSmartPointer<const Calendar>& cached)
	//{
	//	auto ptr = const_cast<Calendar*>(cached.release());
	//	return CalendarConstPtr(const_cast<const Calendar*>(cached.release()));
	//}

	bool Calendar::isWeekend(const Weekday& weekday) const
	{
		return weekend_[static_cast<unsigned>(weekday.c_encoding())];
	}

	bool Calendar::isHoliday(const Date& date) const noexcept
	{
		if (!contains(middle_, date))
		{
			return isWeekend(date);
		}

		const auto offset = holidayOffset(date);

		// todo: improve with some recurring holiday
		return offset == 0 || accHolidays_[offset] != accHolidays_[offset - 1];
	}

	BusinessDays Calendar::countBusinessDays(const Interval<Date>& interval) const
	{
		auto result = busDaysInBounds(interval & middle_);
		result += busDaysOutOfBounds(interval & left_);
		result += busDaysOutOfBounds(interval & right_);

		return BusinessDays{ result };
	}

	BusinessDays Calendar::countBusinessDays(const Date& start, const Date& end) const
	{
		return start <= end
			? countBusinessDays(Interval<Date>::right_open(start, end))
			: -countBusinessDays(Interval<Date>::left_open(end, start));
	}

	Date Calendar::lastBusinessDay(const YearMonth& ym) const
	{
		const auto eom = Date(ym / last); // we start from end of month and roll back
		return previousOrSame(eom);
	}

	Date Calendar::firstBusinessDay(const YearMonth& ym) const
	{
		const auto first = Date(ym / 1); // we start from end of month and roll back
		return nextOrSame(first);
	}

	bool Calendar::isEom(const Date& date) const
	{
		return isBusinessDay(date) && next(date).month() != date.month();
	}

	Date Calendar::next(Date date) const
	{
		return isHoliday(++date) ? next(date) : date;
	}

	Date Calendar::nextOrSame(Date date) const
	{
		return isHoliday(date) ? nextOrSame(++date) : date;
	}

	Date Calendar::previous(Date date) const
	{
		return isHoliday(--date) ? previous(date) : date;
	}

	Date Calendar::previousOrSame(Date date) const
	{
		return isHoliday(date) ? previousOrSame(--date) : date;
	}

	namespace
	{
		// fast shifts forward according to holidays only
		Days getShiftWeekendOnly(const BusinessDays& businessDays, const unsigned char weekendDays)
		{
			const auto weekdays = 7 - weekendDays;
			Days days{ businessDays.count() % weekdays };
			days += Weeks{ businessDays.count() / weekdays };
			return days;
		}
	}

	Date Calendar::shift(Date date, BusinessDays businessDays) const
	{
		const auto start = date;
		const auto weekendShift = getShiftWeekendOnly(businessDays, weekendLength_);
		date += weekendShift;

		if (businessDays > 0_B)
		{
			businessDays -= countBusinessDays(Interval<Date>::left_open(start, date));
			while (businessDays-- > 0_B)
			{
				date = next(date);
			}
			return previousOrSame(date);
		}
		// ReSharper disable once CppRedundantElseKeywordInsideCompoundStatement
		else if (businessDays < 0_B)
		{
			businessDays += countBusinessDays(Interval<Date>::right_open(date, start));
			while (businessDays++ < 0_B)
			{
				date = previous(date);
			}
			return nextOrSame(date);
		}

		return date;
	}

	std::string Calendar::toCode(const std::string& str)
	{
		return TP::stringutils::toUpper(str);
	}

	int Calendar::holidayOffset(const Date& d) const
	{
		return (d - middle_.lower()).count();
	}

	int Calendar::busDaysInBounds(const Interval<Date>& interval) const
	{
		if (is_empty(interval)) return 0;

		const auto days = cardinality(interval);
		const auto l = holidayOffset(boost::icl::last(interval)), pf = holidayOffset(--first(interval));
		const auto acc = accHolidays_[l] - (pf >= 0 ? accHolidays_[pf] : 0); // number of holidays in period
		return days.count() - acc;
	}

	int Calendar::busDaysOutOfBounds(const Interval<Date>& interval) const
	{
		if (is_empty(interval)) return 0;

		const auto days = cardinality(interval);
		const auto weeks = std::chrono::floor<Weeks>(days);
		auto weekends = weeks.count() * weekendLength_; // this gives the number of weekend days in full week cycles
		const auto endWd = last_next(interval).weekday();

		for (auto it = first(interval).weekday(); it != endWd; ++it)
		{
			weekends += isWeekend(it);
		}

		return days.count() - weekends;
	}

	namespace
	{
		std::vector<unsigned short> getAccHolidays(const std::vector<Date>& dates, const std::array<bool, 7>& weekends)
		{
			if (dates.empty()) return {};

			std::vector<unsigned short> res((dates.back() - dates.front()).count() + 1, 0);
			auto it = dates.front();
			for (auto date : dates)
			{
				++res[(date - it).count()];
			}

			std::partial_sum(res.begin(), res.end(), res.begin(),
				[&it, &weekends](auto a, auto b)
			{
				const auto isWeekend = weekends[static_cast<unsigned>((++it).weekday().c_encoding())];
				return a + std::min(b + isWeekend, 1); // if a holiday falls on the weekend we can deal with it nicely
			});

			return res;
		}
	}

	Calendar::Calendar(Code&& code, std::vector<Date>&& holidays, const std::array<bool, 7> weekend)
		: holidays_(std::forward<std::vector<Date>>(holidays)),
		accHolidays_(getAccHolidays(holidays_, weekend)),
		middle_(holidays_.empty() ? Interval<Date>() : Interval<Date>::closed(holidays_.front(), holidays_.back())),
		left_(Interval<Date>::open(Date::min(), holidays_.empty() ? Date::max() : holidays_.front())),
		right_(Interval<Date>::open(holidays_.empty() ? Date::max() : holidays_.back(), Date::max())),
		code_(std::forward<Code>(code)),
		weekend_(weekend),
		weekendLength_(std::accumulate(weekend_.begin(), weekend_.end(), 0))
	{
	}

	CalendarConstPtr Calendar::noHoliday()
	{
		return construct(toCode("none"), {}, {});
	}

	CalendarConstPtr Calendar::isoWeekday()
	{
		// todo change name
		return construct(toCode("EVERY_WEEKDAY_IS_BUSINESS_DAY"), {}, { Sat, Sun });
	}

	CalendarConstPtr Calendar::everyDayisBusinessDay()
	{
		return construct(toCode("EVERY_DAY_IS_BUSINESS_DAY"), {}, {});
	}

	CalendarConstPtr Calendar::FridaysAndsaturdayWeekend()
	{
		return construct(toCode("FRIDAY_SATURDAY_WEEKEND"), {}, {Fri, Sat});
	}

	CalendarConstPtr Calendar::fridayWeekend()
	{
		return construct(toCode("FRIDAY_WEEKEND"), {}, { Fri});
	}
}

