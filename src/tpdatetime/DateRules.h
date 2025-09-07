#pragma once
#include "Durations.h"
#include "YearMonthDay.h"

#include <type_traits>
#include <utility>

namespace TP::date_time
{
	namespace rule
	{
		/**
		 * \brief this policy ensures that when x and y are the same the offset is moved by 1 cycle defined the duration
		 */
		template <typename DateFieldType>
		struct NextPolicy
		{
			using OffsetType = decltype(std::declval<const DateFieldType&>() - std::declval<const DateFieldType&>());

			explicit NextPolicy(OffsetType duration) : duration_(duration) {}

			constexpr OffsetType operator()(const DateFieldType& x, const DateFieldType& y) const noexcept
			{
				const auto n = y - x;
				return n.count() == 0 ? duration_ : n;
			}

		private:
			OffsetType duration_;
		};

		/**
		 * \brief this policy ensures that when x and y are the same (weekday, month...)
		 * the offset leaves the date unchanged (next sunday on a sunday would give us 0 days)
		 */
		template<typename DateFieldType>
		struct SamePolicy
		{
			constexpr auto operator()(const DateFieldType& x, const DateFieldType& y) const noexcept -> decltype(y - x)
			{
				return y - x;
			}
		};

		namespace weekday
		{
			const static auto same = SamePolicy<Weekday>();
			const static auto next = NextPolicy<Weekday>(1_W);

			template <typename T> constexpr bool isWeekdayRule()
			{
				return TP::DecayEquiv<T, NextPolicy<Weekday>>::value || TP::DecayEquiv<T, SamePolicy<Weekday>>::value;
			}

		}

		namespace month
		{
			const static auto same = SamePolicy<Month>();
			const static auto next = NextPolicy<Month>(1_Y);

			template <typename T> constexpr bool isMonthRule()
			{
				return TP::DecayEquiv<T, NextPolicy<Month>>::value || TP::DecayEquiv<T, SamePolicy<Month>>::value;
			}
		}

		namespace month_day
		{
			const static struct Same
			{
				constexpr auto operator()(const YearMonthDay& x, const YearMonthDay& y) const noexcept
				{
					return 0_Y;
				}
			}
			same;

			const static struct Next
			{
				constexpr auto operator()(const YearMonthDay& x, const YearMonthDay& y) const noexcept
				{
					return x == y ? 1_Y : 0_Y;
				}
			}
			next;

			template <typename T> constexpr bool isMonthDayRule()
			{
				return TP::DecayEquiv<T, Same>::value || TP::DecayEquiv<T, Next>::value;
			}
		}

		namespace eom
		{
			const static struct DoNotPreserve : std::false_type
			{
				auto operator()(const std::chrono::year_month_day& ymd, const Months& months) const noexcept
				{
					const auto tmp = ymd + months;
					return  tmp.ok() ? tmp : tmp.year() / tmp.month() / last;
				}

				auto operator()(const std::chrono::year_month_day& ymd, const Years& years) const noexcept
				{
					return ymd == February / 29 / ymd.year() // ensures that if we land on non leap year we still get a valid date
						? February / last / (ymd.year() + years)
						: ymd + years;
				}
				// ReSharper disable once CppInconsistentNaming
			} do_not_preserve;

			const static struct Preserve : std::true_type
			{
				auto operator()(const std::chrono::year_month_day& ymd, const Months& months) const noexcept
				{
					const auto ymdLast = ymd.year() / ymd.month() / last;
					return ymd == ymdLast ? ymdLast + months : do_not_preserve(ymd, months);
				}

				auto operator()(const std::chrono::year_month_day& ymd, const Years& years) const noexcept
				{
					return ymd == February / last / ymd.year()
						? February / last / (ymd.year() + years)
						: ymd + years;

				}
			} preserve;
		}
	}
}
