#include "DayCounterTBillsACT365.h"

using namespace TP::date_time::day_count;

double DayCounterTBillsACT365::calculateDcf(const Date& from, const Date& to, const OptDate& originalIssueDate, const OptDate& referenceEndDate) const
{
	const double basis = originalIssueDate
		.transform([](auto issue) { return issue.add(1_Y, rule::eom::do_not_preserve) - issue; })
		.transform([](auto d) { return d.count(); })
		.orElse([]() { THROW("TBillsACT365 : This day counter needs a reference date to calculate dcf"); })
		.valueOr(0);

	return (to - from).count() / basis;
}
