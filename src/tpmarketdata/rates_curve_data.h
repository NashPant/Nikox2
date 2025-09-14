
#pragma once
//#include <instruments/rate_instruments.h> // #include <market/discount_curve.h>
#include <map>
#include <TPtools/Currency.h>
//#include <market_data/discount_factors.h>
#include <stdexcept>
#include "tpdatetime/datetime.h"
//#include <utils/using.h>
#include <vector>

namespace TP::market_data
{
	enum class CurveUseFlag
	{
		use, // use in the curve
		no_use, // do not use the curve
		not_holiday, // don't use on a holiday (e.g. Fx Forwards)
		overlap  // use in curve unless there is an overlap (e.g. Futures and // Swaps)
	};

	class MeetingDates {
	public:
		MeetingDates(std::vector<date_time::Date> meeting_dates, const int effective_days)
			: _meeting_dates(std::move(meeting_dates))
		{
			if (effective_days < 0)
				throw std::invalid_argument("Effective days must be non-negative");

			_effective_dates.reserve(_meeting_dates.size());
			for (const auto& meeting_date : _meeting_dates)
			{
				_effective_dates.push_back(
					meeting_date + std::chrono::days{ effective_days });
			}
		}
		MeetingDates(
			std::vector<std::pair<date_time::Date, date_time::Date>> 	meeting_and_effective_dates)
		{
			_meeting_dates.reserve(_meeting_dates.size());
			_effective_dates.reserve(_meeting_dates.size());
			for (const auto& dt_pair : meeting_and_effective_dates)
			{
				auto mtg_dt = dt_pair.first;
				auto eff_dt = dt_pair.second;
				if (mtg_dt > eff_dt)
					throw std::invalid_argument(
						//"Meeting date (" + utils::to_string(mtg_dt) + ") must be before effective date (" + utils::to_string(eff_dt) + ")");
						"Meeting date (" + mtg_dt.toString() + ") must be before effective date (" + eff_dt.toString() + ")");
				_meeting_dates.push_back(mtg_dt);
				_effective_dates.push_back(eff_dt);
			}
		}

		// accessors
		const std::vector<date_time::Date>& meeting_dates() const
		{
			return _meeting_dates;
		}
		const std::vector<date_time::Date>& effective_dates() const
		{
			return _effective_dates;
		}
	private:
		std::vector<date_time::Date> _meeting_dates;
		std::vector<date_time::Date> _effective_dates;
	};
	//class CurveInstruments {
	//public:
	//	CurveInstruments(
	//		std::vector<CPtr<instruments::BenchmarkInstrument>> instruments, std::vector<double> market_quotes, std::vector<CurveUseFlag> flags)
	//		: _instruments(std::move(instruments))
	//		_market_quotes(std::move(market_quotes))
	//		, _flags(std::move(flags))
	//		if (_instruments.empty() || _market_quotes.empty() || _flags.empty())
	//			throw std::invalid_argument("Empty input vectors");
	//	if (_instruments.size() != _market_quotes.size())
	//		throw std::invalid_argument(
	//			"Mismatched vector sizes (instruments and market quotes)");
	//	if (_instruments.size() != _flags.size())
	//		throw std::invalid_argument(
	//			"Mismatched vector sizes (instruments and flags)");
	//	// std::vector<double> weights
	//	// weights for instruments
	//	// accessors
	//	// optional: TBD if we want to use
	//	const std::vector<CPtr<instruments::BenchmarkInstrument>>&
	//		instruments() const
	//	{
	//		return _instruments;
	//	}
	//	const auto nb_instruments() const { return _instruments.size(); } const std::vector<double>& market_quotes() const { return _market_quotes; } const std::vector<CurveUseFlag>& flags() const { return _flags; }
	//	auto operator<=>(const CurveInstruments& other) const = default;
	//private:
	//};
	//std::vector<CPtr<instruments::BenchmarkInstrument>> _instruments; std::vector<double> _market_quotes; std::vector<CurveUseFlag> _flags;

			// namespace market_data
	}