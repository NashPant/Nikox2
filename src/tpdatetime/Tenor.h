#pragma once

#pragma once

#include "TPDateTime/Durations.h"
#include "TPDateTime/YearMonthDay.h"
#include "TPDateTime/BusinessDay.h"

#include "TPTools/Utils/Formatting.h"

#include <variant>
#include <ostream>

namespace TP::date_time
{
	/**
	 * \brief tag for special tenors (tenors which do not have a generic representation, i.e. 1D, 1W...)
	 */
	 // ReSharper disable CppInconsistentNaming
	struct TomNext;
	struct SpotNext;
	constexpr struct Overnight
		: TP::operators::totally_ordered<Overnight>,
		TP::operators::totally_ordered<Overnight, TomNext>,
		TP::operators::totally_ordered<Overnight, SpotNext>
	{
		friend constexpr bool operator==(Overnight const & lhs, Overnight const & rhs)  noexcept { return true; }
		friend constexpr bool operator<(Overnight const & lhs, Overnight const & rhs) noexcept { return false; }
		friend constexpr bool operator==(Overnight const & lhs, TomNext const & rhs)  noexcept { return false; }
		friend constexpr bool operator<(Overnight const & lhs, TomNext const & rhs) noexcept { return true; }
		friend constexpr bool operator==(Overnight const & lhs, SpotNext const & rhs)  noexcept { return false; }
		friend constexpr bool operator<(Overnight const & lhs, SpotNext const & rhs) noexcept { return true; }
	} ON; // cannot be compared to Spot or SpotNext as it depends on convention

	constexpr struct TomNext
		: TP::operators::totally_ordered<TomNext>
	{
		friend constexpr bool operator==(TomNext const & lhs, TomNext const & rhs)  noexcept { return true; }
		friend constexpr bool operator<(TomNext const & lhs, TomNext const & rhs) noexcept { return false; }
	} TN; // cannot be compared to Spot or SpotNext as it depends on convention

	constexpr struct Spot
		: TP::operators::totally_ordered<Spot>,
		TP::operators::totally_ordered<Spot, SpotNext>
	{
		friend constexpr bool operator==(Spot const & lhs, Spot const & rhs)  noexcept { return true; }
		friend constexpr bool operator<(Spot const & lhs, Spot const & rhs) noexcept { return false; }
		friend constexpr bool operator==(Spot const & lhs, SpotNext const & rhs)  noexcept { return false; }
		friend constexpr bool operator<(Spot const & lhs, SpotNext const & rhs) noexcept { return true; }
	} SP;

	constexpr struct SpotNext
		: TP::operators::totally_ordered<SpotNext>
	{
		friend constexpr bool operator==(SpotNext const & lhs, SpotNext const & rhs)  noexcept { return true; }
		friend constexpr bool operator<(SpotNext const & lhs, SpotNext const & rhs) noexcept { return false; }
	} SN;
	// ReSharper restore CppInconsistentNaming

	class Tenor;
	using OptTenor = TP::Optional<Tenor>;

	/**
	 * A tenor indicating how long it will take for a financial instrument to reach maturity.
	 * <p>
	 * A tenor is allowed to be any non-negative non-zero period of days, weeks, month or years.
	 * <p>
	 * Each tenor is based on a {@link Duration}. The months and years of the period are normalized,
	 * thus a tenor of 12 months is the same as the one of 1 year.
	 */
	class TPNIKOPOLIS_IMPEXP Tenor final
		: TP::operators::totally_ordered<Tenor>, // it does not make much sense to order tenors without an asof (i.e. how can you compare 30D and 1M?), this is just wrong
		TP::operators::equality_comparable<Tenor, Duration>,
		TP::operators::equality_comparable<Tenor, Years>,
		TP::operators::equality_comparable<Tenor, Months>,
		TP::operators::equality_comparable<Tenor, Weeks>,
		TP::operators::equality_comparable<Tenor, Days>,
		TP::operators::equality_comparable<Tenor, Overnight>,
		TP::operators::equality_comparable<Tenor, TomNext>,
		TP::operators::equality_comparable<Tenor, Spot>,
		TP::operators::equality_comparable<Tenor, SpotNext>,
		TP::operators::equality_comparable<Tenor, MonthBusinessDay>
	{
		using ValueType = std::variant<Overnight, TomNext, Spot, SpotNext, MonthBusinessDay, Duration>;
	public:
		explicit  Tenor(const std::string& str);
		constexpr Tenor()                   noexcept : Tenor(0_D) {} // null tenor (0_D)
		constexpr Tenor(const Duration& d)  noexcept : value_(d) {}
		constexpr Tenor(const Years& y)     noexcept : value_(Duration{ y }) {}
		constexpr Tenor(const Months& m)    noexcept : value_(Duration{ m }) {}
		constexpr Tenor(const Weeks& w)     noexcept : value_(Duration{ w }) {}
		constexpr Tenor(const Days& d)      noexcept : value_(Duration{ d }) {}
		constexpr Tenor(const Overnight& _) noexcept : value_(ON) {}
		constexpr Tenor(const TomNext& _)   noexcept : value_(TN) {}
		constexpr Tenor(const Spot& _)      noexcept : value_(SP) {}
		constexpr Tenor(const SpotNext& _)  noexcept : value_(SN) {}
		constexpr Tenor(const MonthBusinessDay& md) noexcept : value_(md) {}

		Tenor& operator=(const Duration& other) noexcept;
		Tenor& operator=(const Years& other) noexcept;
		Tenor& operator=(const Months& other) noexcept;
		Tenor& operator=(const Weeks& other) noexcept;
		Tenor& operator=(const Days& other) noexcept;
		Tenor& operator=(const Overnight& _) noexcept;
		Tenor& operator=(const TomNext& _) noexcept;
		Tenor& operator=(const SpotNext& _) noexcept;
		Tenor& operator=(const MonthBusinessDay& other) noexcept;

		/**
		 * \brief Allows to call the underlying variant visitor using input callable object
		 */
		template<class Callable>
		[[nodiscard]] constexpr auto visit(Callable&& callable) const { return std::visit(std::forward<Callable>(callable), value_); }

		explicit operator Duration() const;
		explicit operator Months() const;
		explicit operator Days() const;

		friend bool operator==(const Tenor& lhs, const Tenor& rhs) noexcept
		{
			return lhs.value_ == rhs.value_;
		}

		friend bool operator==(const Tenor& lhs, const Duration& rhs) noexcept
		{
			if (const auto duration = std::get_if<Duration>(&lhs.value_)) return *duration == rhs;
			return false;
		}

		template<typename T, std::enable_if_t<std::disjunction_v<
			std::is_same<T, Years>,
			std::is_same<T, Months>,
			std::is_same<T, Weeks>,
			std::is_same<T, Days>
		>, int> = 0>
			friend bool operator==(const Tenor& lhs, const T& rhs)
		{
			if (const auto duration = std::get_if<Duration>(&lhs.value_)) return *duration == Duration{ rhs };
			return false;
		}

		template<typename T, std::enable_if_t<std::disjunction_v<
			std::is_same<T, Overnight>,
			std::is_same<T, TomNext>,
			std::is_same<T, Spot>,
			std::is_same<T, SpotNext>
		>, int> = 0>
			friend bool operator==(const Tenor& lhs, const T& rhs) { return std::holds_alternative<T>(lhs.value_); }

		friend bool operator==(const Tenor& lhs, const MonthBusinessDay& rhs);

		// todo remove all this
		static Tenor(max)();

		[[nodiscard]] unsigned yearCount() const;
		[[nodiscard]] unsigned monthCount() const;

		[[nodiscard]] bool is1DTenor() const;
		[[nodiscard]] bool isByMonth() const;
		bool isByWeek() const;
		[[nodiscard]] bool isByDay() const;
		bool isNull() const;

		// end review

		/**
		 * \brief returns the string representation of the current date
		 */
		[[nodiscard]] std::string toString() const;

	private:
		friend struct std::hash<Tenor>;

		ValueType value_;
	};

	// todo remove as it does not make sense
	bool operator<(const Tenor& lhs, const Tenor& rhs);

	inline Tenor operator+(const Tenor& tenor1, const Tenor& tenor2) { return static_cast<Duration>(tenor1) + static_cast<Duration>(tenor2); }

	inline std::string operator+(const std::string& string, const Tenor& tenor) { return string + tenor.toString(); }

	// TODO get rid this horrible class (very inefficient for no reason)
	struct StrTenorCompare
	{
		bool operator()(const std::string& t1, const std::string& t2) const {
			return Tenor(t1) < Tenor(t2);
		}
	};
}

namespace TP
{
	inline namespace formatting
	{
		//namespace date_time = TP::date_time;

		template<>
		struct Formatter<date_time::Overnight>
		{
			[[nodiscard]] Optional<date_time::Overnight> tryParse(const std::string_view& str) const;
			[[nodiscard]] std::string str(const date_time::Overnight& value) const;
			[[nodiscard]] std::string failToParse(const std::string_view& str) const;
		};

		template<>
		struct Formatter<date_time::TomNext>
		{
			[[nodiscard]] Optional<date_time::TomNext> tryParse(const std::string_view& str) const;
			[[nodiscard]] std::string str(const date_time::TomNext& value) const;
			[[nodiscard]] std::string failToParse(const std::string_view& str) const;
		};

		template<>
		struct Formatter<date_time::Spot>
		{
			[[nodiscard]] Optional<date_time::Spot> tryParse(const std::string_view& str) const;
			[[nodiscard]] std::string str(const date_time::Spot& value) const;
			[[nodiscard]] std::string failToParse(const std::string_view& str) const;
		};

		template<>
		struct Formatter<date_time::SpotNext>
		{
			[[nodiscard]] Optional<date_time::SpotNext> tryParse(const std::string_view& str) const;
			[[nodiscard]] std::string str(const date_time::SpotNext& value) const;
			[[nodiscard]] std::string failToParse(const std::string_view& str) const;
		};

		template<>
		struct Formatter<date_time::Tenor>
		{
			[[nodiscard]] date_time::OptTenor tryParse(const std::string_view& str) const;
			[[nodiscard]] std::string str(const date_time::Tenor& value) const;
			[[nodiscard]] std::string failToParse(const std::string_view& str) const;
		};
	}
}

namespace TP::date_time
{
	OSTREAM_FROM_TP(Overnight)
		OSTREAM_FROM_TP(TomNext)
		OSTREAM_FROM_TP(Spot)
		OSTREAM_FROM_TP(SpotNext)
		OSTREAM_FROM_TP(Tenor)
}

DECLARE_FMT_FORMATTER_FROM_TP(TP::date_time::Overnight)
DECLARE_FMT_FORMATTER_FROM_TP(TP::date_time::TomNext)
DECLARE_FMT_FORMATTER_FROM_TP(TP::date_time::Spot)
DECLARE_FMT_FORMATTER_FROM_TP(TP::date_time::SpotNext)
DECLARE_FMT_FORMATTER_FROM_TP(TP::date_time::Tenor)

namespace std //NOLINT
{
	using namespace TP::date_time;

	template<>
	struct hash<Overnight>
	{
		size_t operator()(const Overnight& tenor) const noexcept;
	};

	template<>
	struct hash<TomNext>
	{
		size_t operator()(const TomNext& tenor) const noexcept;
	};

	template<>
	struct hash<Spot>
	{
		size_t operator()(const Spot& tenor) const noexcept;
	};

	template<>
	struct hash<SpotNext>
	{
		size_t operator()(const SpotNext& tenor) const noexcept;
	};

	template<>
	struct hash<Tenor>
	{
		size_t operator()(const Tenor& tenor) const;
	};
}

