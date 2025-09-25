#pragma once
#include "TPNikopolisUtils.h"
#include "Durations.h"
#include "YearMonthDay.h"

#include "TPTools/utils/Formatting.h"

#include <chrono>
#include <stdexcept>
#include <string>

namespace TP::Core
{
	class TPFrequency;
}

namespace TP::date_time
{
	// fwd declaration
	
	class FrequencyIndexed;

	class Frequency;
	using OptFrequency = TP::Optional<Frequency>;

	/**
	 * \brief A frequency is represented as the number of occurrences within a year
	 */
	class TPNIKOPOLIS_IMPEXP Frequency final
		: TP::operators::totally_ordered<Frequency>
	{
	public:
		/*
		 * @Brief. Default constructor with nothing here with 0 Frequency.
		 *		   TODO not sure if this is the desired behaviour
		 */
		Frequency()
		: rep_(0) {}

		explicit constexpr Frequency(const unsigned short value)
			: rep_(value) {}
		/**
		 * \brief constructs a frequency from string
		 * \param freqStr string corresponding to the relevant frequency Annually, Quarterly, Semi-Annually, Monthly...
		 * \param fmt the string formatting corresponding to the string, supported:
		 *			%b for bonds (A, SA, TERM),
		 *			%f for futures (S, Q, B, A or Z for BRL),
		 *			%v for verbose,
		 *			%r for regular (D, W, M, Q, Y)
		 *			%d for durations (1M, 6M...)
		 */
		explicit Frequency(const std::string_view& freqStr, const TP::Optional<std::string>& fmt = std::nullopt);

		/**
		 * \brief constructs a frequency from a given duration cycle. It will throw if the duration is composite (months and days)
		 * \param duration: this is the cycle of each repetition (e.g. 6M for semi-annual...)
		 */
		Frequency(const Duration& duration);
		/*
		  @brief. Construction from TPFrequency
		*/
		Frequency(const  TP::Core::TPFrequency& tpfreq);

		static OptFrequency tryMake(const Duration& duration);

		/**
		 * \brief constructs a frequency from a given duration cycle
		 * \param duration: this is the cycle of each repetition (e.g. 6M for semi-annual...)
		 */
		template<typename Period>
		constexpr Frequency(const std::chrono::duration<int, Period>& duration)
			: rep_(rep(duration))
		{
		}

		template<typename Period>
		static constexpr OptFrequency tryMake(const std::chrono::duration<int, Period>& duration)
		{
			return tryRep(duration).transform([](const auto& n)
			{
				return Frequency{ n };
			});
		}

		/**
		 * \brief converts frequency into equivalent duration
		 */
		[[nodiscard]] Duration toDuration() const;

		/**
		 * \brief gets the number of cycles per year (2 for semi annual, 0 for never)
		 */
		[[nodiscard]] constexpr unsigned count() const { return rep_; }

		friend constexpr bool operator==(const Frequency& lhs, const Frequency& rhs) noexcept { return lhs.rep_ == rhs.rep_; }
		friend constexpr bool operator<(const Frequency& lhs, const Frequency& rhs) noexcept { return lhs.rep_ < rhs.rep_; }

		constexpr FrequencyIndexed operator[](unsigned index) const noexcept;

	private:
		unsigned short rep_;

		template<typename Period>
		static constexpr TP::Optional<unsigned short> tryRep(const std::chrono::duration<int, Period>& duration)
		{
			if (duration.count() == 0) return std::numeric_limits<decltype(rep_)>::max(); // continuous freq

			const auto n = 1_Y / duration;
			// 1 is because weeks/days will divide to 364 for the year 
			return n == 0 || (std::chrono::duration_cast<Days>(1_Y - n * duration)).count() <= 1
				? TP::makeOptional(static_cast<unsigned short>(n))
				: std::nullopt;
		}

		template<typename Period>
		static constexpr unsigned short rep(const std::chrono::duration<int, Period>& duration)
		{
			const auto r = tryRep(duration);
			return r ? *r : throw std::invalid_argument(fmt::format("Cannot create frequency: 1Y is not divisible by {} periods", duration));
		}
	};

	inline namespace literals
	{
		// ReSharper disable CppInconsistentNaming
		constexpr Frequency Never{ Years::max() };
		constexpr Frequency Yearly{ 1_Y };
		constexpr Frequency SemiAnnually{ 6_M };
		constexpr Frequency TriAnnually{ 4_M };
		constexpr Frequency Quarterly{ 3_M };
		constexpr Frequency BiMonthly{ 2_M };
		constexpr Frequency Monthly{ 1_M };
		constexpr Frequency Fortnightly{ 2_W };
		constexpr Frequency Weekly{ 1_W };
		constexpr Frequency Daily{ 1_D };

		constexpr auto  SA = SemiAnnually;

		constexpr auto  Y = Yearly;
		constexpr auto  Q = Quarterly;
		constexpr auto  M = Monthly;
		constexpr auto  W = Weekly;
		constexpr auto  D = Daily;

		constexpr auto  A = Yearly;
		constexpr auto  B = SemiAnnually;
		constexpr auto  Z = Quarterly;
		constexpr auto  S = Monthly;

		constexpr auto ZeroCoupon = Never;
		// ReSharper restore CppInconsistentNaming
	}

	/**
	 * \brief This represents business rolling periods like Q1, M2 or Y1.
	 */
	class FrequencyIndexed
	{
		Frequency basis_;
		unsigned index_;

	public:
		constexpr FrequencyIndexed(const Frequency& basis, const unsigned index) noexcept
			: basis_(basis), index_(index)
		{}

		[[nodiscard]] constexpr Frequency basis() const { return basis_; }

		[[nodiscard]] constexpr unsigned index() const { return index_; }

		[[nodiscard]] constexpr bool ok() const { return basis_.count() <= 52 && index_ > 0; }
	};

	constexpr FrequencyIndexed Frequency::operator[](const unsigned index) const noexcept { return FrequencyIndexed{ *this, index }; }
}

namespace TP
{
	inline namespace formatting
	{
		//namespace date_time = TP::date_time;

		template<>
		struct Formatter<date_time::Frequency>
		{
			explicit Formatter(Optional<std::string> fmt = std::nullopt)
				: override_(std::move(fmt)) {}

			[[nodiscard]] Optional<date_time::Frequency> tryParse(const std::string_view& str) const;
			[[nodiscard]] std::string str(const date_time::Frequency& value) const;
			[[nodiscard]] std::string failToParse(const std::string_view& str) const;

		private:
			Optional<std::string> override_;
		};

		template<>
		struct Formatter<date_time::FrequencyIndexed>
		{
			explicit Formatter(Optional<std::string> fmt = std::nullopt)
				: override_(std::move(fmt)) {}

			[[nodiscard]] Optional<date_time::FrequencyIndexed> tryParse(const std::string_view& str) const;
			[[nodiscard]] std::string str(const date_time::FrequencyIndexed& value) const;
			[[nodiscard]] std::string failToParse(const std::string_view& str) const;

		private:
			Optional<std::string> override_;
		};
	}
}

namespace TP::date_time
{
	OSTREAM_FROM_TP(Frequency)
	OSTREAM_FROM_TP(FrequencyIndexed)
}

DECLARE_FMT_FORMATTER_FROM_TP(TP::date_time::Frequency)
DECLARE_FMT_FORMATTER_FROM_TP(TP::date_time::FrequencyIndexed)
