
#pragma once
#include <TPtools/currency.h>
#include <string>
#include "tptools/serialisation/include.h"
//#include <utils/arrays.h>
#include <tptools/arrays.h>
#include <tpdatetime/datetime.h> 
#include "tpdatetime/Date.h"
#include <TPtools/tpexception.h>
#include <algorithm>
#include <vector>



namespace TP::market_data
{
	class RateIndex {
	public:
		RateIndex() = default;
		RateIndex(std::string name, Currency ccy)
			:_name(std::move(name)), _ccy(std::move(ccy))
		{
		}
		// accessors
		const std::string& name() const { return _name; }

		const Currency& ccy() const { return _ccy; }
		auto operator<=>(const RateIndex& other) const = default;
	private:
		std::string _name;
		Currency _ccy;
		PROPERTIES(RateIndex, _name, _ccy);
	};


	class DiscountFactors {
	public:
		DiscountFactors() = default;
		DiscountFactors(RateIndex rate_index,
				std::vector<date_time::Date> dates, std::vector<double> dfs):
			_rate_index(std::move(rate_index)),
			_dfs(std::move(dfs))
		{
			if (_dates.empty())
				THROW("DiscountFactors: dates cannot be empty");
			if (_dfs.empty())
				THROW("DiscountFactors: dfs cannot be empty");
			if (_dates.size() != _dfs.size())
				THROW("DiscountFactors: dates and dfs must have the same size");
			if (!tools::is_strictly_increasing(_dates))
				THROW("DiscountFactors: dates must be strictly increasing");
			if (!tools::is_finite(_dfs))
				THROW("DiscountFactors: dfs must be finite");
			if (*std::min_element(_dfs.begin(), _dfs.end()) <= 0.0)
				THROW("DiscountFactors: dfs must be positive");
		}

		// accessors
		const std::string& name() const { return _rate_index.name(); } 
		const Currency& ccy() const { return _rate_index.ccy(); }
		const std::vector<date_time::Date>& dates() const { return _dates; } 
		const std::vector<double>& dfs() const { return _dfs; }

	private:
		RateIndex _rate_index;
		std::vector<date_time::Date> _dates;
		std::vector<double> _dfs;

		PROPERTIES(DiscountFactors, _rate_index, _dates, _dfs);
	};

}
	