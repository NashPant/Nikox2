
#include "datetime.h"
//#include "arrays.h" 
//#include "calendar.h"
#include <chrono>
#include <iomanip>
namespace TP::date_time
{
	//Calendar::Calendar(CalendarDetails details, const CPtr<Holidays> holidays)
	//	: _details(details)
	//	_holidays(
	//		get_combined_holidays(get_weekend_holidays(), holidays)) {
	//};
	//Calendar::Calendar(
	//	CalendarDetails details,
	//	const CPtr<Holidays> holidays,
	//	const CPtr<Holidays> weekend_holiday)
	//	: _details(details)
	//	_holidays(get_combined_holidays(weekend_holiday, holidays)) {};
	//bool Calendar::is_business_day(const Date& date) const
	//	if (_holidays->contains(date))
	//		return false;
	//return true;
	// 
	//Date sysToday()
	//{
	//	return floor<std::chrono::days>(std::chrono::system_clock::now());
	//}

	Date get_date(int year, int month, int day)
	{
		return static_cast<Date>(std::chrono::year(year) / month / day);
	}

	DateTime get_datetime(int year, int month, int day, int hour, int minute, int second)
	{
		auto date = get_date(year, month, day);
		return DateTime(date.time_since_epoch()) + std::chrono::hours(hour) + std::chrono::minutes(minute) + std::chrono::seconds(second);
	}
	DateLocal get_date_local(int year, int month, int day)
	{
		return static_cast<DateLocal>(std::chrono::year(year) / month / day);
	}

	DateTimeLocal get_datetime_local(
		int year, int month, int day, int hour, int minute, int second)
	{
		auto date = get_date_local(year, month, day);
		return date + std::chrono::hours(hour) + std::chrono::minutes(minute) + std::chrono::seconds(second);
	}

	//DateTime datetime_combine(Date date, Time time)
	//{
	//	return std::chrono::time_point_cast<std::chrono::microseconds>(date) +
	//		time.to_duration();
	//}
	Date to_date(DateTime datetime)
	{
		//return Date{ std::chrono::floor<std::chrono::days>(datetime) };
		return Date::from_sys_days(std::chrono::floor<std::chrono::days>(datetime));
	}

	DateLocal to_date_local(DateTimeLocal datetime)
	{
		return std::chrono::floor<std::chrono::days>(datetime);
	}

	//DateTime to_datetime(Date date)
	//{
	//	return std::chrono::time_point_cast<std::chrono::microseconds>(date);
	//}

	//std::string to_string(const Date& d)
	//{
	//	auto ymd = std::chrono::year_month_day{ d };
	//	std::stringstream ss;
	//	ss << std::to_string(int(ymd.year())) << "-" << std::setfill('0')
	//		<< std::setw(2) << unsigned(ymd.month()) << "-" << std::setfill('0') << std::setw(2) << unsigned(ymd.day());
	//	return ss.str();
	//}
	std::string to_string(const DateTime& dt)
	{
		auto dt_and_time = datetime_split(dt);
		//auto ymd = std::chrono::year_month_day{ dt_and_time.first };
		auto ymd = dt_and_time.first.ymd();
		auto hms = std::chrono::hh_mm_ss{ dt_and_time.second };
		std::stringstream ss;
		ss << std::to_string(int(ymd.year())) << "-" << std::setfill('0')
			<< std::setw(2) << unsigned(ymd.month()) << "-" << std::setfill('0')
			<< std::setw(2) << unsigned(ymd.day()) << " " << std::setfill('0')
			<< std::setw(2) << hms.hours().count() << ":" << std::setfill('0')
			<< std::setw(2) << hms.minutes().count() << ":" << std::setfill('0')
			<< std::setw(2) << hms.seconds().count();
		return ss.str();
	}

	//Date date_from_string(const std::string& d)
	//{
	//	std::istringstream ss(d);
	//	int year, month, day;
	//	char dash1, dash2;
	//	ss >> year >> dash1 >> month >> dash2 >> day;
	//	if (ss.fail() || dash1 != '-' || dash2 != '-' || !ss.eof())
	//		throw std::invalid_argument("Invalid date format: " + d);
	//	return get_date(year, month, day);
	//}
	DateTime datetime_from_string(const std::string& dt)
	{
		std::istringstream ss(dt);
		int year, month, day, hour, minute, second;
		char dash1, dash2, colon1, colon2;
		ss >> year >> dash1 >> month >> dash2 >> day >> hour >> colon1 >> minute >> colon2 >> second;
		if (ss.fail() || dash1 != '-' || dash2 != '-' || colon1 != ':' || colon2 != ':' || !ss.eof())
			throw std::invalid_argument("Invalid datetime format: " + dt);
		return get_datetime(year, month, day, hour, minute, second);
	}

	int64_t to_int64_t(DateTime datetime)
	{
		return std::chrono::duration_cast<std::chrono::microseconds>(
			datetime.time_since_epoch()).count();
	}

	//int64_t to_int64_t(Date date)
	//{
	//	return std::chrono::duration_cast<std::chrono::days>(
	//		date.time_since_epoch()).count();
	//}

	template <> 
	DateTime from_int64_t(int64_t count)
	{
		DateTime datetime =
			DateTime(std::chrono::microseconds(count));
		return datetime;
	}
	//template<> Date from_int64_t(int64_t count)
	//{
	//	Date date = Date(std::chrono::days(count)); return date;
	//}
} // namespace TP::date_time
	