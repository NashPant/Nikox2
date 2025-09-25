#pragma once

#include <TPNikopolisUtils.h>
#include "TPTools/Utils/TPOptional.h"
#include <chrono>
#include "TPTools/Utils/Formatting.h"
#include "TPTools/Utils/Operators.h"

namespace TP::date_time
{
	using Days = std::chrono::days;
	using BusinessDays = std::chrono::duration<int, std::ratio_divide<std::chrono::years::period, std::ratio<252>>>; // more or less 252 business days in 1y
	using Weeks = std::chrono::weeks;
	using Months = std::chrono::months;
	using Years = std::chrono::years;
	using Hours = std::chrono::hours;
	using Minutes = std::chrono::minutes;
	using Seconds = std::chrono::seconds;

	using OptDays = TP::Optional<Days>;
	using OptBusinessDays = TP::Optional<BusinessDays>;
	using OptWeeks = TP::Optional<Weeks>;
	using OptMonths = TP::Optional<Months>;
	using OptYears = TP::Optional<Years>;
	using OptHours = TP::Optional<Hours>;
	using OptMinutes = TP::Optional<Minutes>;

	inline namespace literals
	{
		constexpr auto operator "" _D(unsigned long long d) noexcept { return Days{ d }; }
		constexpr auto operator "" _B(unsigned long long b) noexcept { return BusinessDays{ b }; }
		constexpr auto operator "" _W(unsigned long long w) noexcept { return Weeks{ w }; }
		constexpr auto operator "" _M(unsigned long long m) noexcept { return Months{ m }; }
		constexpr auto operator "" _Y(unsigned long long y) noexcept { return Years{ y }; }
	}



	class TPNIKOPOLIS_IMPEXP Duration final
	 :TP::operators::equality_comparable<Duration>,
		TP::operators::equality_comparable<Duration, Years>,
		TP::operators::equality_comparable<Duration, Months>,
		TP::operators::equality_comparable<Duration, Weeks>,
		TP::operators::equality_comparable<Duration, Days>,
		TP::operators::addible<Duration>,
		TP::operators::subtractible<Duration>,
		TP::operators::multiplicable<Duration, int>
	{
	public:
		constexpr Duration() noexcept : Duration(0_M, 0_D) {}; // null duration 
		explicit Duration(const std::string& str);
		constexpr Duration(const Years& y) noexcept : months_(y) {}
		constexpr Duration(const Months& m)  noexcept : months_(m) {}
		constexpr Duration(const Weeks& w)   noexcept : days_(w) {}
		constexpr Duration(const Days& d)  noexcept : days_(d) {}


		template<typename T, std::enable_if_t<std::disjunction_v<
			std::is_same<T, Years>,
			std::is_same<T, Months>,
			std::is_same<T, Weeks>,
			std::is_same<T, Days>
			>, int> = 0>
		[[nodiscard]] constexpr bool is() const
		{
			if constexpr (std::is_same_v<T, Days>)				return months_== 0_M;
			else if constexpr (std::is_same_v<T, Weeks>)     return months_ == 0_M && days() == 0_D;
			else if constexpr (std::is_same_v<T, Months>)		return days_ == 0_D;
			else 												return days_ == 0_D && months() == 0_M;


		}
		/*
		* \brief Cast duration to days only it will concatenate weeks and days, will throw if there is month component in this duration
		*/
		explicit operator Days() const;
		/**
		* \brief Cast duration to months only it will concatenate years and months, will throw if there is day component in this duration */
		explicit operator Months() const;


		Duration(const Duration&) = default;
		Duration(Duration&&) = default;
		Duration& operator =(const Duration&) = default;
		Duration& operator = (Duration&&) = default;
		~Duration() = default;


		[[nodiscard]] constexpr Years years() const;
		[[nodiscard]] constexpr Months months() const;
		[[nodiscard]] constexpr Weeks weeks() const;
		[[nodiscard]] constexpr Days days() const;

		friend constexpr bool operator==(const Duration& lhs, const Duration& rhs) noexcept;
		friend constexpr bool operator==(const Duration& lhs, const Years& rhs) noexcept;
		friend constexpr bool operator== (const Duration& lhs, const Months& rhs) noexcept;
		friend constexpr bool operator==(const Duration& lhs, const Weeks& rhs) noexcept;
		friend constexpr bool operator==(const Duration& lhs, const Days& rhs) noexcept;


		Duration& operator+= (const Duration& right) noexcept;
		Duration& operator-=(const Duration& right) noexcept;
		Duration& operator *= (int quantity) noexcept;
		Duration operator-() const noexcept;


		auto isNull() const { return months_ == 0_M && days_ == 0_D; }
		/*
		Will return the year fraction between 2 durations
		*/
		double tau(const Duration& rhs);
		/*
		* @brief. To convert into a
		* */
	private:
		friend class Date;
		friend class Frequency;
		friend constexpr std::pair<Months, Days> normalized(const Duration& d);
		constexpr Duration(const Months& m, const Days& d) noexcept: months_(m), days_(d) {}


		// Note we only store months and days as the other terms are convertible to those.
		// It means this period is normalized by construction (comparison is fast and simple) 
		Months months_{};
		Days days_{};

	};

	using OptDuration  = TP::Optional<Duration>;
	constexpr Years Duration::years() const
	{
		return std::chrono::floor<std::chrono::years>(months_);
	}

	constexpr Months Duration::months() const
	{
		return months_ - years();
	}

	constexpr Weeks Duration::weeks() const
	{
		return std::chrono::floor<std::chrono::weeks>(days_);
	}
	constexpr Days Duration::days() const
	{
		return days_ - weeks();
	}
	constexpr std::pair<Months, Days> normalized(const Duration& d)
	{
		return std::make_pair(d.months_, d.days_);
	}
	// todo: remove as this is mostly wrong way of calculating dcfs
	constexpr double dcf(const Duration& d, double basis = 365.25)
	{
		const auto [ms, ds] = normalized(d);
		return static_cast<double> (ms.count()) / 12.0
			+ static_cast<double>(ds.count()) / basis;
	}

	constexpr bool operator==(const Duration& lhs, const Duration& rhs) noexcept
	{
		return lhs.months_ == rhs.months_ && lhs.days_ == rhs.days_;
	}
	constexpr bool operator== (const Duration& lhs, const Years& rhs) noexcept
	{
		return lhs.months_ == rhs && lhs.days_ == 0_D;
	}
	constexpr bool operator == (const Duration& lhs, const Months& rhs) noexcept
	{
		return lhs.months_ == rhs && lhs.days_ == 0_D;
	}
	constexpr bool operator==(const Duration& lhs, const Weeks& rhs) noexcept
	{
		return lhs.days_ == rhs && lhs.months_ == 0_M;
	}
	constexpr bool operator== (const Duration& lhs, const Days& rhs) noexcept
	{
		return lhs.days_ == rhs && lhs.months_ == 0_M;
	}

}

namespace TP
{
	inline namespace formatting
	{
		//namespace date_time = TP::date_time;

		template<>
		struct Formatter<date_time::Days>
		{
			[[nodiscard]] date_time::OptDays tryParse(const std::string_view& str) const;
			[[nodiscard]] std::string str(const date_time::Days& value) const;
			[[nodiscard]] std::string failToParse(const std::string_view& str) const;
		};

		template<>
		struct Formatter<date_time::BusinessDays>
		{
			[[nodiscard]] date_time::OptBusinessDays tryParse(const std::string_view& str) const;
			[[nodiscard]] std::string str(const date_time::BusinessDays& value) const;
			[[nodiscard]] std::string failToParse(const std::string_view& str) const;
		};

		template<>
		struct Formatter<date_time::Weeks>
		{
			[[nodiscard]] date_time::OptWeeks tryParse(const std::string_view& str) const;
			[[nodiscard]] std::string str(const date_time::Weeks& value) const;
			[[nodiscard]] std::string failToParse(const std::string_view& str) const;
		};

		template<>
		struct Formatter<date_time::Months>
		{
			[[nodiscard]] date_time::OptMonths tryParse(const std::string_view& str) const;
			[[nodiscard]] std::string str(const date_time::Months& value) const;
			[[nodiscard]] std::string failToParse(const std::string_view& str) const;
		};

		template<>
		struct Formatter<date_time::Years>
		{
			[[nodiscard]] date_time::OptYears tryParse(const std::string_view& str) const;
			[[nodiscard]] std::string str(const date_time::Years& value) const;
			[[nodiscard]] std::string failToParse(const std::string_view& str) const;
		};

		template<>
		struct Formatter<date_time::Duration>
		{
			[[nodiscard]] date_time::OptDuration tryParse(const std::string_view& str) const;
			[[nodiscard]] std::string str(const date_time::Duration& duration) const;
			[[nodiscard]] std::string failToParse(const std::string_view& str) const;
		};
	}
}

namespace TP::date_time
{
	OSTREAM_FROM_TP(Days)
		OSTREAM_FROM_TP(BusinessDays)
		OSTREAM_FROM_TP(Weeks)
		OSTREAM_FROM_TP(Months)
		OSTREAM_FROM_TP(Years)
		//OSTREAM_FROM_TP(Duration)

}

namespace std  // NOLINT
{
	template<typename Rep, typename Ratio>
	struct hash<std::chrono::duration<Rep, Ratio>>
	{
		std::size_t operator()(std::chrono::duration<Rep, Ratio> const& s) const noexcept
		{
			return std::hash<Rep>{}(s.count());
		}
	};

	using namespace TP::date_time;
	template<>
	struct hash<Duration>
	{
		std::size_t operator()(Duration const& s) const noexcept;
	};
}


DECLARE_FMT_FORMATTER_FROM_TP(TP::date_time::Days)
DECLARE_FMT_FORMATTER_FROM_TP(TP::date_time::BusinessDays)
DECLARE_FMT_FORMATTER_FROM_TP(TP::date_time::Weeks)
DECLARE_FMT_FORMATTER_FROM_TP(TP::date_time::Months)
DECLARE_FMT_FORMATTER_FROM_TP(TP::date_time::Years)
DECLARE_FMT_FORMATTER_FROM_TP(TP::date_time::Duration)