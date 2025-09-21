#pragma once

#include "Currency.h"

namespace TP
{
	struct CurrencyPair;
	using OptCurrencyPair =TP::Optional<CurrencyPair>;

	struct TPNIKOPOLIS_IMPEXP CurrencyPair  //: operators::totally_ordered< CurrencyPair>
	{
		const Currency left;
		const Currency right;
		CurrencyPair(){}
		/*
		  @brief. Construction with a string and a constructor with a domestic and foreign ccy
		*/
		explicit CurrencyPair(std::string_view sttr, std::string_view foramt = defaultFormat);
		static CurrencyPair createCurrencyPair(const Currency& domccy, const Currency& forccy);
		/*
		  @brief. Converts to string
		*/
		[[nodiscard]] std::string toString() const;
		/*
		  @brief. Return the ccy that does not match the input ccy. Nullopt if both don't match
		*/
		[[nodiscard]] OptCurrency noMatching(const Currency& in) const;
		/*
		  @brief. Returns true if input ccy exist in the pair
		*/
		[[nodiscard]] bool contains(const Currency& in) const;

	private:
		const static std::string_view defaultFormat;// = "%f%d";
		friend struct Formatter<CurrencyPair>;
		friend constexpr CurrencyPair operator / (const Currency& lhs, const Currency& rhs) noexcept;

		constexpr CurrencyPair(Currency lhs, Currency rhs) noexcept:
			left(std::move(lhs)), right(std::move(rhs))
		{}

		PROPERTIES(CurrencyPair, left, right);
	};

	inline namespace formatting
	{

		template<>
		struct TPNIKOPOLIS_IMPEXP Formatter<CurrencyPair>
		{
			explicit Formatter(std::string_view fmtOverride = CurrencyPair::defaultFormat);

			[[nodiscard]] OptCurrencyPair tryParse(const std::string_view& str) const;
			[[nodiscard]] std::string str(const CurrencyPair& value) const;
			[[nodiscard]] std::string failToParse(const std::string_view& str) const;

		private:
			std::string format_;
		};
	}


	constexpr CurrencyPair operator / (const Currency& lhs, const Currency& rhs) noexcept
	{
		return CurrencyPair(lhs, rhs);
	}
	constexpr bool operator==(const CurrencyPair& lhs, const CurrencyPair& rhs) noexcept
	{
		return lhs.left == rhs.left && lhs.right == rhs.right;
	}
	constexpr bool operator<(const CurrencyPair& lhs, const CurrencyPair& rhs) noexcept
	{
		return lhs.left < rhs.left ||(lhs.left == rhs.left &&  lhs.right <rhs.right );
	}
	/*
	 @brief. Some aliases
	*/
	inline const auto& base(const CurrencyPair& in) { return in.left; }
	inline const auto& quote(const CurrencyPair& in) { return in.right; }
	inline const auto& foreign(const CurrencyPair& in) { return in.left; }
	inline const auto& domestic(const CurrencyPair& in) { return in.right; }
	inline auto inverse(const CurrencyPair& in) { return in.right / in.left; }
	inline bool  isSingle(const CurrencyPair& in) { return in.right == in.left; }
}

OSTREAM_FROM_TP(TP::CurrencyPair)
DECLARE_FMT_FORMATTER_FROM_TP(TP::CurrencyPair)

namespace std
{
	template<>
	struct hash<TP::CurrencyPair>
	{
		size_t operator()(const TP::CurrencyPair& in) const noexcept;
	};
}