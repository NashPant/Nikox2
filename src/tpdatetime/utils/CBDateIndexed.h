#pragma once
#include "TPDateTime/Tenor.h"
#include "TPDateTime/Date.h"
//#include "TPTools/Utils/TPOptional.h"
#include <optional>
#include <string>

namespace TP::date_time
{
	/*
	 *	@brief. ShortCut for sequential Central Bank Meeting Dates shothand notation/
	 *			so it will be MTG1, MTG2, etc,
	 *			It needs a calendar of Central Bank meeting dates and an asofDate and then it will work out the sequence(trivial)
	 */

	class TPNIKOPOLIS_IMPEXP CBMDateIndexed final
	{
	public:
		static CBMDateIndexed create(const std::string& code);

		CBMDateIndexed(const unsigned index) noexcept
			:index_(index)
		{}
		CBMDateIndexed(unsigned index, unsigned maxI) noexcept
			:index_(index),	maxIndex_(maxI)
		{}
		CBMDateIndexed(const CBMDateIndexed& rhs) = default;
		CBMDateIndexed(CBMDateIndexed&& rhs) = default;

		const CBMDateIndexed& operator = (const CBMDateIndexed& rhs)
		{
			if(this != &rhs)
			{
				base_ = rhs.base_;
				index_ = rhs.index_;
			}
			return *this;
		}

		[[nodiscard]] const std::string& baseStr() const { return base_; }
		[[nodiscard]] unsigned index() const { return index_; }
		[[nodiscard]] bool ok() const { return maxIndex_.has_value() ? index_ > 0 && index_ < maxIndex_.value() : index_ >0; }

	private:
		std::string base_ = "MTD";
		unsigned index_;
		std::optional<unsigned> maxIndex_;
	};

	/*
	 * @brief. Same as above but with month year ending, ie.. MTDM20, MTDZ20, etc..
	 */
	class TPNIKOPOLIS_IMPEXP CBMDateMonthIndexed final
	{
	public:
		explicit CBMDateMonthIndexed(YearMonth&& m): yearMonth_(std::move(m)){}

		static CBMDateMonthIndexed create(const std::string& codestr);

		CBMDateMonthIndexed(const CBMDateMonthIndexed& rhs) = default;
		CBMDateMonthIndexed(CBMDateMonthIndexed&& rhs) = default;

		const CBMDateMonthIndexed& operator = (const CBMDateMonthIndexed& rhs)
		{
			if (this != &rhs)
			{
				base_ = rhs.base_;
				yearMonth_ = rhs.yearMonth_;
			}
			return *this;
		}

		[[nodiscard]] const std::string& baseStr() const { return base_; }
		[[nodiscard]] const YearMonth& ym() const { return yearMonth_; }
	private:
		std::string base_ = "MTD";
		YearMonth	yearMonth_;
	};

}

namespace TP
{
	inline namespace formatting
	{
		//Date Indexed

		template<>
		struct TPNIKOPOLIS_IMPEXP Formatter<date_time::CBMDateIndexed>
		{
			explicit Formatter(Optional<std::string> fmtOverride = std::nullopt);

			[[nodiscard]] Optional<date_time::CBMDateIndexed> tryParse(const std::string& str) const;
			[[nodiscard]] std::string str(const date_time::CBMDateIndexed& value) const;
			[[nodiscard]] std::string failToParse(const std::string& str) const;

		private:
			Optional<std::string> override_;
		};

		//YearMonth Date Indexed

		template<>
		struct TPNIKOPOLIS_IMPEXP Formatter<date_time::CBMDateMonthIndexed>
		{
			explicit Formatter(Optional<std::string> fmtOverride = std::nullopt);

			[[nodiscard]] Optional<date_time::CBMDateMonthIndexed> tryParse(const std::string& str) const;
			[[nodiscard]] std::string str(const date_time::CBMDateMonthIndexed& value) const;
			[[nodiscard]] std::string failToParse(const std::string& str) const;

		private:
			Optional<std::string> override_;
		};

	}

}

