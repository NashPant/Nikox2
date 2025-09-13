#pragma	once

#include "TPTools/Utils/hash_replace.h"
#include "Date.h"
#include <chrono> 
#include <string> 
#include <variant>
namespace TP::date_time
{
	//using Date =std::chrono::time_point<std::chrono::system_clock, std::chrono::days>;
	using DateTime = std::chrono::time_point<std::chrono::system_clock, std::chrono::microseconds>;

	using TimeDelta = decltype(DateTime() - DateTime());

	using Time = std::chrono::hh_mm_ss<std::chrono::microseconds>;

	using DateLocal = std::chrono::time_point<std::chrono::local_t, std::chrono::days>;

	using DateTimeLocal = std::chrono::time_point<std::chrono::local_t, std::chrono::microseconds>;
	// gcc-11 does not offer tz support.
#ifdef _MSC_VER
	using ZonedDate = std::chrono::zoned_time<std::chrono::days>;
	using ZonedTime = std::chrono::zoned_time<std::chrono::microseconds>;
#endif // _MSC_VER
	//Date sysToday();
	Date get_date(int year, int month, int day);
	DateTime  get_datetime(int year, int month, int day, int hour, int minute, int second);
	//DateTime datetime_combine(utils::Date, utils::Time);
	//DateTime to_datetime(utils::Date date);
	DateLocal get_date_local(int year, int month, int day);
	DateTimeLocal get_datetime_local(int year, int month, int day, int hour, int minute, int second);

	Date to_date(DateTime datetime);

	DateLocal to_date_local(DateTimeLocal datetime);

	int64_t to_int64_t(DateTime datetime);
	//int64_t to_int64_t(utils::Date date);

	inline std::chrono::month m_from_date(const Date& date)
	{
		//return std::chrono::year_month_day{ date }.month();
		return  date.month();
		
	}
	inline std::chrono::year y_from_date(const Date& date)
	{
		//return std::chrono::year_month_day{ date }.year();
		return  date.year();
	}

	inline std::chrono::year_month ym_from_date(const Date& date)
	{
		//auto ymf = std::chrono::year_month_day{ date.ymd()};
		auto ymf =  date.ymd();
		return ymf.year() / ymf.month();
	}

	inline Date eom(const Date& date)
	{
		//const auto ymd = std::chrono::year_month_day{ date };
		const auto ymd = date.ymd();
		return ymd.year() / ymd.month() / std::chrono::last;
	}

	template <typename T> T from_int64_t(int64_t count);

	constexpr auto date_to_date_local(const Date& d)
	{
		return DateLocal{ d.time_since_epoch() };
	}

	constexpr auto date_local_to_date(DateLocal d)
	{
		return Date{ d.time_since_epoch() };
	}

	inline std::pair<Date, std::chrono::microseconds>
		datetime_split(const DateTime& dt)
	{
		using namespace std::chrono;
		
		// floor DateTime to whole days
		const auto days_tp = floor<days>(dt);

		// wrap that into your Date class
		Date d = Date::from_sys_days(days_tp);

		// remainder = dt - floored days
		const auto us = duration_cast<microseconds>(dt - days_tp);

		return { d, us };
	}

	inline double year_frac(TimeDelta time_delta)
	{
		const double micro_seconds = static_cast<double>(time_delta.count());
		return micro_seconds / 1000 / 1000 / 3600. / 24 / 365.;
	}
	inline double year_frac(DateTime dt1, DateTime dt2)
	{
		const auto time_delta = dt2 - dt1;
		return year_frac(time_delta);
	}
	inline auto time_delta(double year_frac)
	{
		const auto rep =
			static_cast<TimeDelta::rep>(year_frac * 365 * 24 * 3600 * 1000 * 1000);
		return TimeDelta(rep);
	}
	//			std::string to_string(const Date & d);
	std::string to_string(const DateTime& dt);
	//		utils::Date date_from_string(const std::string & d_str);
	DateTime datetime_from_string(const std::string& dt_str);
}

	// Hash functions
namespace std
{
	//template <class Rep, class Period> struct hash<chrono::duration<Rep, Period>>
	//{
	//	size_t operator()(chrono::duration<Rep, Period> duration) const
	//	{
	//		return TP::hash(duration.count());
	//	}
	//};

	template <class Clock, class Duration>
	struct hash<chrono::time_point<Clock, Duration>>
	{
		size_t operator()(chrono::time_point<Clock, Duration> date) const
		{
			return TP::hash(date.time_since_epoch());
		}
	};
}
					// namespace std