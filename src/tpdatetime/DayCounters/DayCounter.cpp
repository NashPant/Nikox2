#include "DayCounter.h"

#include "DayCounterActual360.h"
#include "DayCounterActual365.h"
#include "DayCounterNL365.h"
#include "DayCounterActAct.h"
#include "DayCounter30360.h"
#include "DayCounter30E360.h"
#include "DayCounter30360German.h"
#include "DayCounterActActBond.h"
#include "DayCounterBus252.h"
#include "DayCounterTBillsACT365.h"
#include "DayCounterConstant.h"
#include "TPDateTime/CalendarStore.h"
#include "TPDateTime/DayCounters/DayCounterStore.h"
#include "TPDateTime\Schedule\conventions\StandardDateConvention.h"

//#include "TPTools\Utils\EnumClassImpl.h"
#include "TPTools\StringUtils\TPStringUtils.h"
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/split.hpp>


/*
namespace TP
{
	template class TPNIKOPOLIS_IMPEXP Enum<date_time::DayCounter::Code>;

	template <>
	//TPNIKOPOLIS_IMPEXP
	const Enum<date_time::DayCounter::Code>::SymbolVector Enum<date_time::DayCounter::Code>::symbolVector_ = {
		{ "ACT360", date_time::DayCounter::Code::ACT360 },
		{ "ACT365", date_time::DayCounter::Code::ACT365 },
		{ "ACTACT", date_time::DayCounter::Code::ACTACT },
		{ "ACTACTBOND", date_time::DayCounter::Code::ACTACTBOND },
		{ "ISDA30360", date_time::DayCounter::Code::ISDA30360 },
		{ "ISMA30360", date_time::DayCounter::Code::ISMA30360 },
		{ "GERMAN30360", date_time::DayCounter::Code::GERMAN30360 },
		{ "NL365", date_time::DayCounter::Code::NL365 },
		{ "BUS252", date_time::DayCounter::Code::BUS252 },

	};


}
*/

using namespace TP::date_time::day_count;
using TP::object_store::CalendarStore;
using TP::object_store::DayCounterStore;



DayCounterConstPtr DayCounter::fromTPBasis(const TPBasis& tpBasis)
{
	switch (tpBasis)
	{
	case TP_30_360: return thirty360();
	case TP_30E_360: return icma30360();
	case TP_ACT_360: return actual360();
	case TP_ACT_365:return actual365();
	case TP_ACT_ACT: return actualActual();
	case TP_CONT: return continous();
	default: THROW("Cannot translate from TPBasis to DayCoonter");
	}
}
DayCounterConstPtr DayCounter::actual360()
{
	static const auto DayCounterPtr = std::make_shared<DayCounterAct360>();
	return DayCounterPtr;
}

DayCounterConstPtr DayCounter::thirty360()
{
	static const auto DayCounterPtr = std::make_shared<DayCounter30360>();
	return DayCounterPtr;
}

DayCounterConstPtr DayCounter::actual365()
{
	static const auto DayCounterPtr = std::make_shared<DayCounterAct365>();
	return DayCounterPtr;
}

DayCounterConstPtr DayCounter::actualActual()
{
	static const auto DayCounterPtr = std::make_shared<DayCounterActAct>();
	return DayCounterPtr;
}

DayCounterConstPtr DayCounter::continous()
{
	static const auto DayCounterPtr = std::make_shared<DayCounterCont>();
	return DayCounterPtr;
}

DayCounterConstPtr DayCounter::icma30360() // Do not use store, but convention holder
{
	const auto dayCounterId = "30E360";
	auto dayCounter = DayCounterStore::getDayCounter(dayCounterId);
	if (!dayCounter)
	{
		dayCounter = std::make_shared<const DayCounter30E360>();
		DayCounterStore::addDayCounter(dayCounterId, dayCounter);
	}

	return dayCounter;
}

DayCounterConstPtr DayCounter::german30360()
{
	static const auto DayCounterPtr = std::make_shared<DayCounter30360German>();
	return DayCounterPtr;
}

DayCounterConstPtr DayCounter::NL365()
{
	static const auto DayCounterPtr = std::make_shared<DayCounterNL365>();
	return DayCounterPtr;
}

DayCounterConstPtr DayCounter::business252(const CalendarConstPtr& calendar)
{
	const auto dayCounterId = fmt::format("BUS252:{}", calendar->code().underlying());

	auto dayCounter = DayCounterStore::getDayCounter(dayCounterId);
	if (!dayCounter)
	{
		dayCounter = std::make_shared<const DayCounterBus252>(calendar);
		DayCounterStore::addDayCounter(dayCounterId, dayCounter);
	}

	return dayCounter;
}


DayCounterConstPtr DayCounter::actualActualBond(const Duration& tenor, const DateConventionConstPtr& dateConvention,
	bool isUSWIT)
{
	
	const auto standardDateConvention = std::dynamic_pointer_cast<const StandardDateConvention>(dateConvention);
	if (!standardDateConvention)
	{
		THROW("DayCounter ACTACTBOND is defined only for StandardDateConvention");
	}

	const auto rollConvention = static_cast<int>(standardDateConvention->rollConvention());

	const auto dayCounterId = fmt::format("ACTBONDBOND:{}:{}:{}", tenor, dateConvention->calendar()->code().underlying(), rollConvention);
	auto dayCounter = DayCounterStore::getDayCounter(dayCounterId);
	if (!dayCounter)
	{
	//	if (isUSWIT)
	//	{
	//		dayCounter = std::make_shared<const DayCounterUSWITActActBond>(tenor, dateConvention);
	//	}
	//	else
	//	{
			dayCounter = std::make_shared<const DayCounterActActBond>(tenor, dateConvention);
	//	}

		DayCounterStore::addDayCounter(dayCounterId, dayCounter);
	}

	return dayCounter;
	static const auto DayCounterPtr = std::make_shared<DayCounterNL365>();
	return DayCounterPtr;
}


DayCounterConstPtr DayCounter::TBillsAct365()
{
	static const auto DayCounterPtr = std::make_shared<DayCounterTBillsACT365>();
	return DayCounterPtr;
}

/*
DayCounterConstPtr DayCounter::FracMonth(const DateConventionConstPtr & dateConvention)
{
	const auto DayCounterPtr = std::make_shared<DayCounterFracMonth>(dateConvention);
	return DayCounterPtr;
}
*/

DayCounterConstPtr DayCounter::Const(const double dcf)
{
	const auto DayCounterPtr = std::make_shared<DayCounterConstant>(dcf);
	return DayCounterPtr;
}

DayCounterConstPtr DayCounter::fromString(const std::string& input, const std::optional<CalendarConstPtr>& calendarInput)
{
	const auto str = TP::stringutils::toUpper(input);
	if (str == "ACT360" || str == "ACT/360" || str == "ACT/360 NON-EOM")
	{
		return actual360();
	}

	if (str == "30360" || str == "30/360")
	{
		return thirty360();
	}

	if (str == "30E360" || str == "ISMA-30/360" || str == "GER:30/360 NON-EOM")
	{
		return icma30360();
	}

	if (str == "GERMAN:30/360")
	{
		return german30360();
	}

	if (str == "ACT365" || str == "ACT/365" || str == "_ACT365")
	{
		return actual365();
	}

	if (str == "ACTACT" || str == "ACT/ACT")
	{
		return actualActual();
	}

	if (str == "NL/365")
	{
		return NL365();
	}

	if (boost::istarts_with(str, "BUS252") || boost::istarts_with(str, "BUS/252") || boost::istarts_with(str, "_BUS252") || boost::istarts_with(str, "_BD252"))
	{
		std::vector<std::string> tokens;
		split(tokens, str, boost::is_any_of("-"));
		if (tokens.size() != 2)
		{
			THROW("To create BUS252 DayCounter, the string [" + str + "] must specify (exactly) one calendar (such as BUS252-NYB)");
		}

		const auto calendar = calendarInput.has_value() ? calendarInput.value() : CalendarStore::getCalendar(tokens[1]);
		return business252(calendar);
	}
	
	if (boost::istarts_with(str, "ACTACTBOND"))
	{
		std::vector<std::string> tokens;
		split(tokens, str, boost::is_any_of("-"));
		if (tokens.size() != 4)
		{
			THROW("To create ACTACTBOND DayCounter, the string [" + str + "] must specify a tenor, " +
				"calendar, accrual convention and  (such as ACTACTBOND-6M-NYB-NONE)");
		}

		const auto tenor = Duration(tokens[1]);
		const auto calendar = calendarInput.has_value() ? calendarInput.value() : CalendarStore::getCalendar(tokens[2]);
		const auto dateConvention = std::make_shared<const StandardDateConvention>(calendar, tokens[3], 0);
		return actualActualBond(tenor, dateConvention, false);
	}
	
	if (str == "US:WIT ACT/ACT")
	{
		const auto calendar = calendarInput.has_value() ? calendarInput.value() : CalendarStore::getCalendar(std::string("NYB"));
		const auto dateConvention = std::make_shared<const StandardDateConvention>(calendar, BusinessDayConvention("NONE"), 0);
		return actualActualBond(6_M, dateConvention, true);
	}
	
	if (str == "TBILLSACT365")
	{
		return TBillsAct365();
	}

	if (boost::istarts_with(str, "FRACMONTH"))
	{
		THROW("DayCounter::fromString - cannot create " + str + " daycounter from string");
	}

	if (boost::istarts_with(str, "CONSTANT"))
	{
		THROW("DayCounter::fromString - cannot create " + str + " daycounter from string");
	}

	THROW("unknown DayCounter [" + str + "]");
}

std::string DayCounter::calendarName(const std::string& input)
{
	const auto str = TP::stringutils::toUpper(input);
	if (boost::istarts_with(str, "BUS252"))
	{
		std::vector<std::string> tokens;
		split(tokens, str, boost::is_any_of("-"));
		if (tokens.size() != 2)
		{
			THROW("To create BUS252 DayCounter, the string [" + str + "] must specify (exactly) one calendar (such as BUS252-NYB)");
		}

		return tokens[1];
	}
	if (boost::istarts_with(str, "ACTACTBOND"))
	{
		std::vector<std::string> tokens;
		split(tokens, str, boost::is_any_of("-"));
		if (tokens.size() != 4)
		{
			THROW("To create ACTACTBOND DayCounter, the string [" + str + "] must specify a tenor, " +
				"calendar, accrual convention and  (such as ACTACTBOND-6M-NYB-NONE)");
		}

		return tokens[2];
	}
	if (str == "US:WIT ACT/ACT")
	{
		return "NYB";
	}
	return "";

}

