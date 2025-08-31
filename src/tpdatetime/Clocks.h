#pragma once
#pragma once

#include <optional>
#include <chrono>


namespace TP::date_time
{
	struct ExcelClock;

	/**
	 * \brief This is an Excel date serial
	 */
	using ExcelDate = std::chrono::time_point<ExcelClock, std::chrono::days>;

	/**
	 * Here we are only interested in date conversion including the 1900 leap issue see [1] for more details
	 * [1] https://support.microsoft.com/en-us/help/214326/excel-incorrectly-assumes-that-the-year-1900-is-a-leap-year
	 */
	struct ExcelClock
	{
		using duration = std::chrono::days;
		using rep = duration::rep;
		using period = duration::period;
		using time_point = std::chrono::time_point<ExcelClock>;
		static const bool is_steady = false;

		static time_point now() noexcept;

		// ReSharper disable CppInconsistentNaming
		static std::optional<std::chrono::sys_days> try_to_sys(const ExcelDate& tp) noexcept;

		static std::chrono::sys_days to_sys(const ExcelDate& tp);

		static std::optional<ExcelDate> try_from_sys(const std::chrono::sys_days& tp) noexcept;

		static ExcelDate from_sys(const std::chrono::sys_days& tp);
		// ReSharper restore CppInconsistentNaming

		static ExcelDate(min)();

		static ExcelDate(max)();
	};
}

