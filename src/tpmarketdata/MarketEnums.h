#pragma once

//#include "TPNikopolisUtils.h"
#include "TPTools/Utils/Enum_Class.h"

namespace TP::marketdata
{
	enum class TPUNIT
	{
		TPABSOLUTE,
		PERCENTAGE_POINT,
		BASIS_POINT
	};

	enum class QUOTE_TYPE
	{
		DEFAULT,
		CLEAN_PRICE,
		DIRTY_PRICE,
		DISCOUNT,
		YIELD,
		MARKET_YIELD,
		Z_SPREAD
	};

	enum class DELTA
	{
		DELTA_1,
		DELTA_5,
		DELTA_10,
		DELTA_25
	};

	enum class SHOCK_TYPE
	{
		ABSOLUTE_SHOCK,//aka additive i.e. shocked value = original value + shock.size
		RELATIVE_SHOCK //                  shocked value = original value * (1 + shock.size)
	};

	enum class TP_FuturesDatesCalendar
	{
		TP_IMM,
		TP_AUD,
		TP_NZD,
		TP_CAD,
		TP_INVALIDFUTURESCALENDAR
	};


	inline TP_FuturesDatesCalendar ToFuturesDatesCalendar(const std::string& str)
	{

		TP_FuturesDatesCalendar ret = TP_FuturesDatesCalendar::TP_INVALIDFUTURESCALENDAR;

		if (str == "IMM") ret = TP_FuturesDatesCalendar::TP_IMM;
		else if (str == "AUD") ret = TP_FuturesDatesCalendar::TP_AUD;
		else if (str == "NZD") ret = TP_FuturesDatesCalendar::TP_NZD;
		else if (str == "CAD") ret = TP_FuturesDatesCalendar::TP_CAD;
		else
		{
			THROW("ToFuturesDatesCalendar: Invalid Futures Dates Calendar")
		}

		return ret;
	}

	double toDouble(DELTA e);//return delta in absolute units WITHOUT the sign
	DELTA toDelta(double d);

	double getQuoteInAbsoluteUnit(double value, TPUNIT unit);
	//TPNIKOPOLIS_IMPEXP double getQuoteInAbsoluteUnit(const std::pair<double, TPUNIT>&);
	double getQuoteInPercentageUnit(double value, TPUNIT unit);
}

namespace TP
{
	using namespace TP::marketdata;
	template <>
	const TPEnum<TPUNIT>::SymbolVector TPEnum<TPUNIT>::symbolVector_ = {
		{"ABSOLUTE", TPUNIT::TPABSOLUTE},
		{"ABS", TPUNIT::TPABSOLUTE},
		{"PERCENTAGE_POINT", TPUNIT::PERCENTAGE_POINT},
		{"PERCENTAGEPOINT", TPUNIT::PERCENTAGE_POINT },
		{"PERC", TPUNIT::PERCENTAGE_POINT },
		{"BASIS_POINT", TPUNIT::BASIS_POINT},
		{"BP", TPUNIT::BASIS_POINT}
	};

	// Quote Type
	template <>
	const TPEnum<QUOTE_TYPE>::SymbolVector TPEnum<QUOTE_TYPE>::symbolVector_ = {
		{ "DEFAULT", QUOTE_TYPE::DEFAULT },
		{ "CLEAN_PRICE", QUOTE_TYPE::CLEAN_PRICE },
		{ "DIRTY_PRICE", QUOTE_TYPE::DIRTY_PRICE },
		{ "DISCOUNT", QUOTE_TYPE::DISCOUNT },
		{ "YIELD", QUOTE_TYPE::YIELD },
		{ "MARKET_YIELD", QUOTE_TYPE::MARKET_YIELD },
		{ "Z_SPREAD", QUOTE_TYPE::Z_SPREAD }
	};

	// Delta
	template <>
	const TPEnum<DELTA>::SymbolVector TPEnum<DELTA>::symbolVector_ = {
		{"DELTA_1", DELTA::DELTA_1},
		{"DELTA_5", DELTA::DELTA_5},
		{"DELTA_10", DELTA::DELTA_10},
		{"DELTA_25", DELTA::DELTA_25}
	};
	// Shock Type
	template <>
	const TPEnum<SHOCK_TYPE>::SymbolVector TPEnum<SHOCK_TYPE>::symbolVector_ = {
		{"TPABSOLUTE", SHOCK_TYPE::ABSOLUTE_SHOCK},
		{"ABS", SHOCK_TYPE::ABSOLUTE_SHOCK},
		{"RELATIVE", SHOCK_TYPE::RELATIVE_SHOCK},
		{"REL", SHOCK_TYPE::RELATIVE_SHOCK}
	};
	// Shock Type
	template <>
	const TPEnum<TP_FuturesDatesCalendar>::SymbolVector TPEnum<TP_FuturesDatesCalendar>::symbolVector_ = {
		{"TP_IMM", TP_FuturesDatesCalendar::TP_IMM},
		{"IMM", TP_FuturesDatesCalendar::TP_IMM},
		{"TP_AUD", TP_FuturesDatesCalendar::TP_AUD},
		{"AUD", TP_FuturesDatesCalendar::TP_AUD},
		{"TP_NZD", TP_FuturesDatesCalendar::TP_NZD},
		{"NZD", TP_FuturesDatesCalendar::TP_NZD},
		{"TP_CAD", TP_FuturesDatesCalendar::TP_CAD},
		{"CAD", TP_FuturesDatesCalendar::TP_CAD},
		{"TP_INVALIDFUTURESCALENDAR", TP_FuturesDatesCalendar::TP_INVALIDFUTURESCALENDAR}
	};

}