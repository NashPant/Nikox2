#pragma once
#include "YearMonthDay.h"

#include "TPTools/Utils/NotNull.h"

#include <variant>

namespace TP::date_time
{
	// fwd declarations
	class Date;
	FWD_DECLARE_NOTNULLSHARED(Calendar)
	FWD_DECLARE_NOTNULLSHARED(DateAdjustment)

		/**
		 * \brief This class implements the concept of a business day and moth (i.e. end of APR, beginning of JUN...).
		 * This is a very useful type for turns and is used within our tenor
		 */
	//class MonthBusinessDay;

	class MonthBusinessDay final:
		 TP::operators::equality_comparable<MonthBusinessDay>,
		TP::operators::equality_comparable<MonthDay>,
		TP::operators::equality_comparable<MonthDayLast>
	{
		friend struct TP::Formatter<MonthBusinessDay>;
		friend struct std::hash<MonthBusinessDay>;

		std::variant<MonthDay, MonthDayLast> md_;
	public:
		MonthBusinessDay() = default;

		// ReSharper disable CppNonExplicitConvertingConstructor
		constexpr MonthBusinessDay(const MonthDay& md) noexcept : md_(md) {}
		constexpr MonthBusinessDay(const MonthDayLast& mdl) noexcept : md_(mdl) {}
		// ReSharper restore CppNonExplicitConvertingConstructor

		[[nodiscard]] constexpr Month month() const
		{
			return std::visit([](const auto& v) { return v.month(); }, md_);
		}

		[[nodiscard]] constexpr bool ok() const
		{
			return std::visit([](const auto& v) { return v.ok(); }, md_);
		}

		/**
		 * \brief Gets the date adjustment for this term
		 * \param cdr the calendar used to get the correct business date
		 * \param rollRule if true means we won't role to next year period if date is same as input
		 */
		[[nodiscard]] DateAdjustmentConstPtr adjustment(const CalendarConstPtr& cdr, bool rollRule = true) const;

		friend constexpr bool operator==(MonthBusinessDay const& lhs, MonthBusinessDay const& rhs) { return lhs.md_ == rhs.md_; }

		template<typename T, std::enable_if_t<std::disjunction_v<
			std::is_same<T, MonthDay>,
			std::is_same<T, MonthDayLast>>
			>>
			friend constexpr bool operator==(MonthBusinessDay const& lhs, T const& rhs)
		{
			if (const auto md = std::get_if<T>(lhs.md_)) return *md == rhs;
			return false;
		}
	};

	using OptMonthBusinessDay = TP::Optional<MonthBusinessDay>;


	inline namespace literals
	{
		// ReSharper disable CppInconsistentNaming
		// ReSharper disable CppMemberFunctionMayBeStatic
		constexpr struct BeginSpec final
		{
			explicit BeginSpec() = default;

			[[nodiscard]] constexpr MonthBusinessDay of(const Month& m) const { return m / 1d; }

		} Beginning;

		constexpr struct EndSpec final
		{
			explicit EndSpec() = default;

			[[nodiscard]] constexpr MonthBusinessDay of(const Month& m) const { return m / last; }
		} End;
		// ReSharper restore CppMemberFunctionMayBeStatic
		// ReSharper restore CppInconsistentNaming

	}
}

namespace TP
{
	inline namespace formatting
	{
		//namespace date_time = TP::date_time;

		template<>
		struct Formatter<date_time::MonthBusinessDay>
		{
			explicit Formatter(Optional<std::string> fmtOverride = std::nullopt);

			[[nodiscard]] Optional<date_time::MonthBusinessDay> tryParse(const std::string_view& str) const;
			[[nodiscard]] std::string str(const date_time::MonthBusinessDay& value) const;
			[[nodiscard]] std::string failToParse(const std::string_view& str) const;

		private:
			Optional<std::string> override_;
		};
	}
}

namespace TP::date_time
{
	OSTREAM_FROM_TP(MonthBusinessDay)
}

DECLARE_FMT_FORMATTER_FROM_TP(TP::date_time::MonthBusinessDay)

namespace std  // NOLINT
{
	using namespace TP::date_time;
	template<>
	struct hash<MonthBusinessDay>
	{
		std::size_t operator()(MonthBusinessDay const& s) const;
	};
}
