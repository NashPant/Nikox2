#include "instruments.h"

namespace TP
{
	namespace instruments
	{
		// TODO: TermRateFuture, CompoundingRateFuture, AverageRateFuture (??) 
		class RateFuture : public BenchmarkInstrument {
		public:
			// TODO: how to treat margining ?
			RateFuture(
				TP::Currency ccy,
				market_data::RateIndex rate_index,
				date_time::Date settlement,
				double strike)
				// "last trade" ??
				: _data{ .ccy = std::move(ccy),
				.rate_index = std::move(rate_index),
				.settlement = settlement,
				.strike = strike }
			{
			}
			const TP::Currency& ccy() const { return _data.ccy; }
			const market_data::RateIndex& rate_index() const
			{
				return _data.rate_index;
			}
			date_time::Date settlement() const { return _data.settlement; } 
			double strike() const { return _data.strike; }

			double value(const market::CurveMap& curves_map, const market::InflationCurveMap& infl_map = {}) const override;


			date_time::Date maturity_date() const override
			{
				return _data.settlement;
			}
				// probably wrong
			double market_quote() const { return _data.strike; }; // TODO

			std::string to_string() const override;

			market::CurveDatesSetMap
				product_dates_map(date_time::Date as_of_date) const override;


		private:
			struct Data
			{
				TP::Currency ccy;
				market_data::RateIndex rate_index;
				date_time::Date settlement{};
				double strike{};
				auto operator<=>(const Data& other) const = default;
			} _data;
		};
	}
}
// namespace instruments