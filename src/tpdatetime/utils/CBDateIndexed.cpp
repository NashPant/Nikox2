#include "CBDateIndexed.h"

#include <boost/algorithm/string/trim.hpp>
#include "TPTools/StringUtils/TPStringUtils.h"
#include "fmt/format.h"
#include "TPDateTime/Frequency.h"
using namespace TP::formatting;
using namespace TP::date_time;
namespace TP::date_time
{

	//CBMDateMonthIndexed CBMDateMonthIndexed::create(const std::string& codestr)
	//{
	//	auto res = tryParse<CBMDateMonthIndexed>(codestr);
	//	if (res.has_value())
	//		return res.value();
	//	else
	//		THROW(fmt::format("Cannot parse {} into a appropriate CB Month Indexed Class", codestr));
	//	//return res.valueOrThrow<TPException>(fmt::format("Cannot parse {} into a appropriate CB INdexed Class", codestr));
	//}
}




Formatter<CBMDateIndexed>::Formatter(TP::Optional<std::string> fmtOverride)
	: override_(std::move(fmtOverride))
{
}

TP::Optional<CBMDateIndexed> Formatter<CBMDateIndexed>::tryParse(const std::string& str) const
{
	auto strCopy = str;
	boost::trim(strCopy);
	const auto cid = TP::stringutils::toUpper(strCopy);
	const auto b = TPStringUtils::contains(cid, "MTD");
	if (!b)
		return std::nullopt;
	const auto mtg = TPStringUtils::removeDigits(cid);
	unsigned n;
	try
	{
		n = static_cast<unsigned>(std::stoi(TPStringUtils::split(cid, mtg, false)));
	}
	catch(...)
	{
		return std::nullopt;
	}
	return mtg =="MTD" ? makeOptional(date_time::CBMDateIndexed{ n }) : std::nullopt;
}

std::string Formatter<CBMDateIndexed>::str(const CBMDateIndexed& value) const
{
	return fmt::format("{}{}", value.baseStr(), value.index());
}

std::string Formatter<CBMDateIndexed>::failToParse(const std::string& str) const
{

	//return ::failToParse<TomNext>(str, __FILE__, __LINE__);
	return ::failToParse<CBMDateIndexed>(str, __FILE__, __LINE__);
}

Formatter<CBMDateMonthIndexed>::Formatter(TP::Optional<std::string> fmtOverride)
	: override_(std::move(fmtOverride))
{
}

TP::Optional<CBMDateMonthIndexed> Formatter<CBMDateMonthIndexed>::tryParse(const std::string& str) const
{
	auto strCopy = str;
	boost::trim(strCopy);
	const auto cid = TP::stringutils::toUpper(strCopy);
	const auto b = TPStringUtils::contains(cid, "MTD");
	if (!b)
		return std::nullopt;
	const auto yStr = TPStringUtils::split(cid, "MTD", false);
	const auto isMonthYear = Formatter<YearMonth>{}.tryParse(yStr);

	return isMonthYear.hasValue() ? makeOptional(CBMDateMonthIndexed{ YearMonth{isMonthYear.value()} }) : std::nullopt;
}

std::string Formatter<CBMDateMonthIndexed>::str(const CBMDateMonthIndexed& value) const
{
	return fmt::format("{}{}", value.baseStr(), toString(value.ym(), "%C"));
}

std::string Formatter<CBMDateMonthIndexed>::failToParse(const std::string& str) const
{

	//return ::failToParse<TomNext>(str, __FILE__, __LINE__);
	return ::failToParse<CBMDateMonthIndexed>(str, __FILE__, __LINE__);
}