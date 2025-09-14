#include "MarketEnums.h"
#include "TPTools/TPException.h"
#include <map>

namespace
{
	using namespace TP::marketdata;
	const std::map<DELTA, double> deltaToDoubleMap = {
		   { DELTA::DELTA_1, 0.01 },
		   { DELTA::DELTA_5, 0.05 },
		   { DELTA::DELTA_10, 0.10 },
		   { DELTA::DELTA_25, 0.25 }
	};
}
namespace TP::marketdata
{
	//return delta in absolute units WITHOUT the sign
	double toDouble(DELTA e)
	{
		return deltaToDoubleMap.at(e);
	}

	DELTA toDelta(double d)
	{
		static auto doubleToEnum = reverseMap(deltaToDoubleMap);
		auto it = doubleToEnum.find(d);
		if (it != doubleToEnum.end())
			return it->second;
		
		THROW("Cannot convert double [" + std::to_string(d) + "] to a DELTA enum.");
	}

	inline double getQuoteInAbsoluteUnit(double value, TPUNIT unit)
	{
		switch (unit)
		{
		case TPUNIT::TPABSOLUTE:
			return value;
		case TPUNIT::PERCENTAGE_POINT:
			return value / 100.0;
		case TPUNIT::BASIS_POINT:
			return value / 10000.0;
		default:
			THROW("TPUNIT [" + TP::Core::toString<TPUNIT>(unit) + "] is not supported.");
		}
	}
	/*double getQuoteInAbsoluteUnit(const std::pair<double, TPUNIT>& value)
	{
		return getQuoteInAbsoluteUnit(value.first, value.second);
	}*/
	inline double getQuoteInPercentageUnit(double value, TPUNIT unit)
	{
		switch (unit)
		{
		case TPUNIT::TPABSOLUTE:
			return value * 100.0;
		case TPUNIT::PERCENTAGE_POINT:
			return value;
		case TPUNIT::BASIS_POINT:
			return value / 100.0;
		default:
			THROW("TPUNIT [" + TP::Core::toString<TPUNIT>(unit) + "] is not supported.");
		}
	}
}