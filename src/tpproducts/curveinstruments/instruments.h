
#pragma once
#include <variant>
#include <tptools/Currency.h>
#include <tpdatetime/Date.h>
//#include <market/discount_curve.h> 
//#include <market/inflation_curve.h> 
//#include <market_data/currency.h> 
//#include <utils/enums.h>

namespace TP
{
	namespace instruments
	{
		class Instrument {
		public:

			virtual ~Instrument() = default;
			// todo: virutal some_type serialize() = 0;
		};

		class SettledCash : public Instrument {
		public:
			SettledCash(TP::Currency ccy)
				: _data{ .ccy = std::move(ccy) }
			{
			}
			const TP::Currency& ccy() const { return _data.ccy; }
		private:

			struct Data
			{
				TP::Currency ccy;
				auto operator<=>(const Data& other) const = default;
			} _data;
		};


		class Cash: public Instrument {
		public:
			Cash(TP::Currency ccy, date_time::Date settlement)
				: _data{ .ccy = std::move(ccy), .settlement = settlement }
			{
			}
			const TP::Currency& ccy() const { return _data.ccy; } 
			date_time::Date settlement() const { return _data.settlement; }
		private:
			struct Data
			{
				TP::Currency ccy;
				date_time::Date settlement{};
				auto operator<=>(const Data& other) const = default;
			} _data;
		};

		// benchmark instruments: used for curve construction et al
		class BenchmarkInstrument : public Instrument {
		public:
			virtual ~BenchmarkInstrument() = default;
			
			// Comment: maybe it should take a "MarketData" object instead 
			virtual double value(const market::CurveMap& curves_map,
				const market::InflationCurveMap& infl_map = {}) const = 0;


			virtual utils::Date maturity_date() const = 0;
			// we need flexibility for curve fitting. normally it's maturity date 
			// but e.g. for inflation swaps, it would be "inflation index end date" 
			
			virtual utils::Date curve_fit_date() const { return maturity_date(); }
			virtual double market_quote() const = 0;
			virtual std::string to_string() const = 0;


			virtual market::CurveDatesSetMap
				product_dates_map(date_time::Date as_of_date) const = 0;
		};
	}
	// namespace instruments
}