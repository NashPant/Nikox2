#pragma once
// !!!! NEED TO INCLUDE <pybindT1/stl.h> BEFORE <datetime.h>
// !!!!
#include <pybind11/stl.h> // !!! include this BEFORE <datetime.h> !!!
#include <chrono>
#include <datetime.h> // !!! include this AFTER <pybind11/stl.h> !!!
#include <pybind11/chrono.h> 
#include <pybind11/pybind11.h> 
#include <utils/datetime.h> 
#include <tptools/TPException.h> // clang-format on
namespace pybind11
{
	namespace detail
	{
		// This is for casting times on the system clock into datetime.date
		// instances std::chrono::time_point<std::chrono::system_clock,
		// std::chrono::days>;
		template <>
		class type_caster<
			std::chrono::time_point<std::chrono::system_clock, std::chrono::days>> {
		public:
			using type = std::chrono::time_point<std::chrono::system_clock, std::chrono::days>;
			bool load(handle src, bool)
			{
				using namespace std::chrono;
				// Lazy initialise the PyDateTime import
				if (!PyDateTimeAPI)
				{
					PyDateTime_IMPORT;
				}
				if (!src)
				{
					return false;
				}
				if (PyDate_Check(src.ptr()))
				{
					const int days = PyDateTime_GET_DAY(src.ptr());
					const int months = PyDateTime_GET_MONTH(src.ptr());
					const int years = PyDateTime_GET_YEAR(src.ptr());
					std::chrono::year_month_day ymd =
						std::chrono::year{ years } / months / days;
					value = std::chrono::sys_days{ ymd };
					return true;
				}
				else
				{
					return false;
				}
			}

			static handle cast(
				const std::chrono::time_point<std::chrono::system_clock, std::chrono::days>& src,
				return_value_policy /* policy */,
				handle /* parent */)
			{
				using namespace std::chrono;
				// Lazy initialise the PyDateTime import
				if (!PyDateTimeAPI)
				{
					PyDateTime_IMPORT;
				};
				const std::chrono::year_month_day ymd{ std::chrono::floor<std::chrono::days>(src) };

				return PyDate_FromDate(
					static_cast<int>(ymd.year()),
					static_cast<unsigned>(ymd.month()),
					static_cast<unsigned>(ymd.day()));
				PYBIND11_TYPE_CASTER(type, const_name("datetime.date"));
			}
		};


		template <>
		class type_caster<std::chrono::time_point<
			std::chrono::system_clock,
			std::chrono::microseconds>>
		{
		public:
			using type = std::chrono::time_point<std::chrono::system_clock, std::chrono::microseconds>;

			bool load(handle src, bool)
			{
				using namespace std::chrono;

				object tzinfo = src.attr("tzinfo");
				if (!tzinfo.is_none())
				{
					THROW("The cpp function you are calling is expecting a naive datetime.");
				}
				// Lazy initialise the PyDateTime import
				if (!PyDateTimeAPI)
				{
					PyDateTime_IMPORT;
				}

				if (!src)
				{
					return false;
				}
				const auto& dt = src.ptr();
				if (PyDateTime_Check(src.ptr()))
				{
					int year = PyDateTime_GET_YEAR(dt);
					int month = PyDateTime_GET_MONTH(dt);
					int day = PyDateTime_GET_DAY(dt);
					int hour = PyDateTime_DATE_GET_HOUR(dt);
					int min = PyDateTime_DATE_GET_MINUTE(dt);
					int sec = PyDateTime_DATE_GET_SECOND(dt);
					int usec = PyDateTime_DATE_GET_MICROSECOND(dt);
					value = utils::get_datetime(year, month, day, hour, min, sec);
					value += std::chrono::microseconds(usec);
					return true;
				}
				else
				{
					return false;
				}
			}
			static handle cast(
				const std::chrono::time_point<	std::chrono::system_clock,
				std::chrono::microseconds>& src,
				return_value_policy /* policy */,
				handle /* parent */)
			{
				using namespace std::chrono;
				// Lazy initialise the PyDateTime import
				if (!PyDateTimeAPI)
				{
					PyDateTime_IMPORT;
				}

				const auto micro_since_epoch = src.time_since_epoch().count();
				const double sec_since_epoch = micro_since_epoch / 1.e6;
				module_ datetime = module_::import("datetime");
				return datetime.attr("datetime")
					.attr("fromtimestamp") (sec_since_epoch, datetime.attr("UTC")).attr("replace")(arg("tzinfo") = none())
					.release();
			}
			PYBIND11_TYPE_CASTER(type, const_name("datetime.datetime"));
		};
	}
	// namespace detail
}// namespace pybind11