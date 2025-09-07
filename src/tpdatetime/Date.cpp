#include "Date.h"

//#include <date/tz.h>
#include <stdexcept>
#include <utility>
#include <vector>
#include <fmt/format.h>
#include <fmt/chrono.h>
//#include "TPTools/Time/TPDate.h"

using namespace std::chrono;

namespace TP::date_time
{
	const std::string Date::defaultFormat = "{:%Y%m%d}";
	Date Date::sysToday() noexcept
	{
		const auto today = floor<days>(std::chrono::system_clock::now());
		return Date(today);
	}
	/*
		namespace
		{
			const class LocalTz // todo: promote when we create Instant class
			{
			public:
				// ReSharper disable once CppMemberFunctionMayBeStatic
				[[nodiscard]] const auto& operator()() const
				{
					// At the time of writing, this code was commented out.
					// Before uncommenting it back in, replace all calls
					// to 'call_once' because otherwise they break the
					// mechanism for .NET bindings.
					std::call_once(flag_, call);
					return *tz_;
				}

				static void call()
				{
					#ifdef _WIN32
						set_install("//ad/dfs/ligo/IANA/tzdata");
						tz_ = reload_tzdb().current_zone();
					#else
						tz_ = get_tzdb().current_zone();
					#endif
				}

			private:
				static const time_zone* tz_;
				static inline std::once_flag flag_;

			// ReSharper disable once IdentifierTypo
			} localTz;

			const time_zone* LocalTz::tz_;
		}
	*/
	Date Date::localToday() noexcept
	{
		time_t t;
		time(&t);
		const auto tm = std::localtime(&t);
		return Date(*tm);

		// TODO: enable when we sort out the tz lib
		//const auto today = floor<days>(localTz().to_local(std::chrono::system_clock::now()));
		//const auto ymd = year_month_day{today};
		//return Date(ymd);
	}

	namespace
	{
		//optimized for dated between 01/01/2000 on 31/12/2060
		constexpr year_month_day startYmd = 1d / January / 2000y;
		constexpr year_month_day endYmd = (startYmd.year() + years{ 60 }) / December / last;

		constexpr int offset(const sys_days serial)
		{
			constexpr sys_days serialStart = startYmd;
			return (serial - serialStart).count();
		}

		auto getYearMonthDayCache()
		{
			sys_days serial(startYmd);
			constexpr auto count = (sys_days{ endYmd } -sys_days{ startYmd }).count() + 1;
			std::vector<year_month_day> res(count);
			for (auto& re : res)
			{
				re = serial;
				serial += days{ 1 };
			}

			return res;
		}

		constexpr int offset(const year_month ym)
		{
			const auto years = (ym.year() - startYmd.year()).count();
			const auto months = (ym.month() - startYmd.month()).count();

			return static_cast<int>(years * 12 + months);
		}

		auto getSerialCache()
		{
			auto ym = startYmd.year() / startYmd.month();
			constexpr auto count = (endYmd.year() / endYmd.month() - startYmd.year() / startYmd.month()).count() + 1;
			std::vector<sys_days> res(count);
			for (auto& re : res)
			{
				re = sys_days{ ym / 1d };
				ym += months{ 1 };
			}

			return res;
		}

		template<class Arg>
		constexpr std::invalid_argument invalid(Arg arg)
		{
			std::stringstream os;
			os << "Could not create date " << arg;
			return std::invalid_argument(os.str());
		}

	}

	Date::Date(const std::string& date, const std::string& fmt)
		: Date(TP::parse<YearMonthDay>(date, fmt))
	{
	}

	Date::Date(const tm& tm)
		: Date(Year{ tm.tm_year + 1900 } / Month{ static_cast<unsigned>(tm.tm_mon + 1) } / Day{ static_cast<unsigned>(tm.tm_mday) })
	{
	}

	Date::Date(const std::chrono::system_clock::time_point& time)
		: Date(std::chrono::floor<Days>(time))
	{
	}

	Date::Date(const Year year, const Month month, const Day day)
		: Date(year / month / day)
	{
	}
	Date::Date(Day day, Month month, Year year)
		: Date(year / month / day)
	{

	}
	//Date::Date(const TPDate& tpd):Date(Year(static_cast<int>(tpd.year()),Month(tpd.month()),Day(tpd.day()))
//	{
//	}
	//Date::Date(const TPDate& tpd)
	//{
	//	Year y{ static_cast<int>(tpd.year()) };
	//	Month m{ tpd.month() };
	//	Day d{ static_cast<unsigned>(tpd.day()) };
	//	serial_ = Date(y, m, d).ymd();
	//}

	Date::Date(const YearMonthDay& ymd)
		: serial_(toSerial(ymd.ok() ? ymd : throw invalid(ymd)))
	{
	}

	Date::Date(const YearMonthDayLast& ymd)
		: serial_(toSerial(ymd.ok() ? ymd : throw invalid(ymd)))

	{
	}

	Date::Date(const YearMonthWeekday& ymwi)
		: serial_(ymwi.ok() ? ymwi : throw invalid(ymwi))
	{
	}

	Date::Date(const YearMonthWeekdayLast& ymwl)
		: serial_(ymwl.ok() ? ymwl : throw invalid(ymwl))
	{
	}

	Date::Date(const ExcelDate& tp)
		: serial_(ExcelClock::to_sys(tp))
	{
	}

	OptDate Date::tryFromExcel(const ExcelDate& tp)
	{
		return ExcelClock::try_to_sys(tp)
			.transform([](auto sys)
		{
			return Date{ sys };
		});
	}

	YearMonthDay Date::ymd() const noexcept
	{
		return YearMonthDay{ toYmd() };
	}

	YearMonth Date::ym() const noexcept
	{
		const auto ymd = toYmd();
		return ymd.year() / ymd.month();
	}

	Year Date::year() const noexcept
	{
		return toYmd().year();
	}

	Month Date::month() const noexcept
	{
		return toYmd().month();
	}

	Day Date::day() const noexcept
	{
		return toYmd().day();
	}

	Weekday Date::weekday() const noexcept
	{
		return Weekday{ serial_ };
	}

	Date Date::eom() const noexcept
	{
		const auto ymd = toYmd();
		const auto ymLast = ymd.year() / ymd.month() / last;
		if (ymd == ymLast)
			return *this;

		// it's ok to avoid checking as the last day of a month is always valid
		return Date(toSerial(ymLast));
	}

	Date Date::eoq() const noexcept
	{
		const auto ymd = toYmd();
		const auto offset = static_cast<unsigned>(ymd.month()) % 3;
		const auto duration = Months{ offset == 0 ? offset : 3 - offset };
		const auto ym = ymd.year() / ymd.month() + duration;

		// it's ok to avoid checking as the date is always valid
		return  Date(toSerial(ym / last));
	}

	Date Date::eoy() const noexcept
	{
		// it's ok to avoid checking as the date is always valid
		return  Date(toSerial(year() / Dec / last));
	}

	std::tm Date::toTm() const noexcept
	{
		const auto ymd = this->ymd();
		std::tm tm{};
		tm.tm_year = static_cast<int>(ymd.year()) - 1900;
		tm.tm_mon = static_cast<int>(static_cast<unsigned>(ymd.month()) - 1);
		tm.tm_mday = static_cast<int>(static_cast<unsigned>(ymd.day()));
		tm.tm_wday = static_cast<int>(static_cast<unsigned>(weekday().c_encoding()));
		tm.tm_yday = (serial_ - sys_days{ ymd.year() / Jan / 1d }).count();
		tm.tm_isdst = -1; // negative because not enough info to set tm_isdst

		return tm;
	}

	std::string Date::toString(const std::string& fmt) const
	{
		//return format(std::locale(), fmt, serial_);
		try
		{
			return fmt::vformat(std::locale(), fmt, fmt::make_format_args(serial_));
			//return format(std::locale(), fmt, serial_);
		}
		catch (const fmt::format_error& e)
		{
			throw std::invalid_argument("Could not format for date:" + std::string(e.what()));
		}
	}

	ExcelDate Date::toExcel() const
	{
		return ExcelClock::from_sys(serial_);
	}

	Date& Date::operator+=(const Months& months) noexcept
	{
		serial_ = toSerial(rule::eom::preserve(toYmd(), months));
		return *this;
	}

	Date& Date::operator+=(const Years& years) noexcept
	{
		serial_ = toSerial(rule::eom::preserve(toYmd(), years));
		return *this;
	}

	year_month_day Date::toYmd() const
	{
		static const auto cache = getYearMonthDayCache();
		static const auto size = static_cast<int>(cache.size());

		const auto i = offset(serial_);
		return i >= 0 && i < size ? cache[i] : year_month_day{ serial_ };
	}

	sys_days Date::toSerial(const year_month_day& ymd)
	{
		static const auto cache = getSerialCache();
		static const auto size = static_cast<int>(cache.size());
		const auto i = offset(ymd.year() / ymd.month());
		return i >= 0 && i < size ? cache[i] + days{ static_cast<unsigned>(ymd.day()) - 1 } : static_cast<sys_days>(ymd);
	}

}

namespace TP::formatting
{
	Formatter<date_time::Date>::Formatter(Optional<std::string> fmtOverride)
		: formatter_(std::move(fmtOverride))
	{
	}

	Optional<date_time::Date> Formatter<date_time::Date>::tryParse(const std::string& str) const
	{
		return formatter_.tryParse(str);
	}

	std::string Formatter<date_time::Date>::str(const date_time::Date& value) const
	{
		return formatter_.str(value.ymd());
	}

	std::string Formatter<date_time::Date>::failToParse(const std::string& str) const
	{
		return formatter_.failToParse(str);
	}
}
