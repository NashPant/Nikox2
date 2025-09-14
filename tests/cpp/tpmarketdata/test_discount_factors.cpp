#include <gtest/gtest.h>
#include "TPDateTime/Date.h"
#include "tpmarketdata/marketcurves/discount_factors.h"

using namespace TP;
TEST(DiscountFactor, MarketDataContainer)
{
	using namespace TP::date_time;
	const market_data::RateIndex rate_index("EURIBOR", Currency("EUR"));
	auto d = Date(2000y, Jan, 31d);
	const std::vector<date_time::Date> dates =
	{
	date_time::Date(2021y, Jan, 1d),
	date_time::Date(2021y, Mar, 1d),
	date_time::Date(2021y, May, 1d),
	date_time::Date(2021y, Jul, 1d),
	date_time::Date(2021y, Sep, 1d),
	date_time::Date(2021y, Feb, 1d),
	date_time::Date(2021y, Apr, 1d),
	date_time::Date(2021y, Jun, 1d),
	date_time::Date(2021y, Aug, 1d),
	date_time::Date(2021y, Oct, 1d),
	date_time::Date(2021y, Nov, 1d), date_time::Date(2021y, Dec, 1d)
	};

	const std::vector<double> dfs = {
	1.0,
	0.999, 0.998, 0.997, 0.996, 0.995,
	0.994, 0.993, 0.992, 0.991, 0.990, 0.989
	};
	market_data::DiscountFactors discount_mkt(rate_index, dates, dfs);
	EXPECT_EQ(discount_mkt.name(), "EURIBOR");
	//EXPECT_EQ(discount_mkt.ccy().code(), "EUR");
	EXPECT_EQ(discount_mkt.dates().size(), 12);
	EXPECT_EQ(discount_mkt.dfs().size(), 12);
	EXPECT_EQ(discount_mkt.dates()[0], date_time::Date(2021y, Jan, 1d));
	EXPECT_EQ(discount_mkt.dfs()[0], 1.0);
}