#include "Clocks.h"

// CPP
#include "Clocks.h"
#include "Durations.h"

#include <fmt/chrono.h>

#include <stdexcept>

namespace TP::date_time
{
	using namespace std::chrono;
	// ReSharper disable CppInconsistentNaming

	namespace
	{
		constexpr sys_days min_days = 1d / January / 1900y;
		constexpr sys_days max_days = 1d / January / 10000y; // careful this will map to 31/12/9999 (due to the leap year issue)
		constexpr sys_days epoch = 0d / January / 1900y; // this is the epoch
		constexpr sys_days leap = 29d / February / 1900y; // this date should not exist but excel thinks 1900 is leap

	}

	ExcelDate ExcelClock::min()
	{
		const static auto tp = ExcelDate(min_days - epoch);
		return tp;
	}

	ExcelDate ExcelClock::max()
	{
		const static auto tp = ExcelDate(max_days - epoch);
		return tp;
	}

	ExcelClock::time_point ExcelClock::now() noexcept
	{
		using namespace std::chrono;
		const auto sysDays = sys_days{ floor<days>(system_clock::now()) };
		return ExcelDate(sysDays - epoch + (sysDays >= leap ? 1_D : 0_D));
	}

	TP::Optional<sys_days> ExcelClock::try_to_sys(const ExcelDate& tp) noexcept
	{
		if (tp < min() || tp > max()) return std::nullopt;

		const auto date = tp.time_since_epoch() + epoch;
		return date - (date < leap ? 0_D : 1_D);
	}

	sys_days ExcelClock::to_sys(const ExcelDate& tp) 
	{
		auto date = tp.time_since_epoch() + epoch;
		if (tp < min())
		{
			throw std::out_of_range(fmt::format("Invalid Excel date {}. Smaller than minimum supported Excel date {}",
				date.time_since_epoch(), min_days.time_since_epoch()));

		}

		if (date < leap)
			return date;

		date -= 1_D;
		if (date < max_days)
			return date;

		throw std::out_of_range(fmt::format("Invalid Excel date {}. Greater than maximum supported Excel date {}",
			date.time_since_epoch(), (max_days - 1_D).time_since_epoch()));
	}

	TP::Optional<ExcelDate> ExcelClock::try_from_sys(const sys_days& tp) noexcept
	{
		if (min_days > tp || max_days <= tp)
			return std::nullopt;

		return ExcelDate(tp - epoch + (tp < leap ? 0_D : 1_D));
	}

	ExcelDate ExcelClock::from_sys(const sys_days& tp)
	{
		if (tp >= max_days)
		{
			throw std::out_of_range(fmt::format("Could not create Excel date for {}. Greater than maximum supported Excel date {}",
				tp.time_since_epoch(), (max_days - 1_D).time_since_epoch()));
		}

		if (tp >= leap)
			return ExcelDate(tp - epoch + 1_D);

		if (tp >= min_days)
			return ExcelDate(tp - epoch);

		throw std::out_of_range(fmt::format("Could not create Excel date for {}. Smaller than minimum supported Excel date {}",
			tp.time_since_epoch(), min_days.time_since_epoch()));
	}

	// ReSharper restore CppInconsistentNaming
}

