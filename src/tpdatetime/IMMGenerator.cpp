#include "IMMGenerator.h"

//#include "include/date/iso_week.h"
#include "TPDateTime/Calendar.h"
//#include "TPDateTime/financial/DateAdjustment.h"

namespace TP::date_time
{
	IMMDateGenerator::IMMDates IMMDateGenerator::nextIMMDatePair(const Date& asof)
	{
		Date nextdate = asof;
		immiterator_.next(nextdate);
		return IMMDates(dateadjuster_->adjust(nextdate), std::move(nextdate));
	}

	IMMDateGenerator::IMMDates IMMDateGenerator::nextIMMDatePair()
	{
		return nextIMMDatePair(*immiterator_);
	}

	Date IMMDateGenerator::nextIMMDate(const Date& asof)
	{
		return nextIMMDatePair(asof).effective_;
	}

	Date IMMDateGenerator::prevIMMDate(const Date& asof)
	{
		Date nextdate = asof;
		immiterator_.prev(nextdate);
		return  nextdate;
	}

	std::set<Date> IMMDateGenerator::Chain(const Date& asof,
		const size_t& NContracts,
		std::optional<unsigned> earlyRollOffset)
	{

		//auto modifiedAsOfDate = busdaylag_->calendar()->shift(asof, 
		//					BusinessDays(earlyRollOffset.value_or(0.0))); // 
		auto modifiedAsOfDate = calendar_->shift(asof,
			BusinessDays(earlyRollOffset.value_or(0.0)));
		Date nextdate = modifiedAsOfDate;
		std::set<Date> res;

		for (unsigned i = 0; i < std::fabs(NContracts); i++)
		{
			if (NContracts > 0)
				immiterator_.next(nextdate);
			else
				immiterator_.prev(nextdate);
			//auto expiry = busdaylag_->expiryDate(nextdate);
			auto expiry = dateadjuster_->adjust(nextdate);
			res.emplace(nextdate);
		}
		return res;
	}

	Date    IMMDateGenerator::expiryDate(const Date& asof) const
	{
		return dateadjuster_->adjust(asof);
	}

	Date    IMMDateGenerator::shiftDate(const Date& asof, const size_t& d) const
	{
		//return busdaylag_->calendar()->shift(asof, BusinessDays(d));
		return calendar_->shift(asof, BusinessDays(d));
	}

	Date IMMDateGenerator::nthContract(
		const Date& asOfDate,
		unsigned int nthContract,
		bool includeToday,
		std::optional<unsigned> earlyRollOffset)
	{
		Date modifiedAsOfDate;
		if (isIMMDate(asOfDate))
			modifiedAsOfDate = modifiedAsOfDate - Days(earlyRollOffset.value_or(0));

		auto chain = Chain(modifiedAsOfDate, nthContract, earlyRollOffset);
		return *chain.rbegin();
	}


	std::set<Date> IMMOptionsDateGenerator::Chain(const Date& asof,
		const size_t& NContracts,
		std::optional<unsigned> earlyRollOffset)
	{
		const auto modifiedAsOfDate = shiftDate(asof, earlyRollOffset.value_or(0.0)); //

		Date nextdate = modifiedAsOfDate;
		std::set<Date> res;
		for (unsigned i = 0; i < std::fabs(NContracts); i++)
		{
			if (nextdate >= cutoffdate_)
			{
				if (NContracts > 0)
					secondimmgenerator_.next(nextdate);
				else
					secondimmgenerator_.prev(nextdate);
			}
			else //
			{
				Date dateforsecondimm = nextdate;
				if (NContracts > 0)
				{
					secondimmgenerator_.next(dateforsecondimm);
					nextdate = nextIMMDate(nextdate);
					if (dateforsecondimm.month() == nextdate.month())
						nextdate = dateforsecondimm;
				}
				else
				{
					secondimmgenerator_.prev(dateforsecondimm);
					nextdate = prevIMMDate(nextdate);
					if (dateforsecondimm.month() == nextdate.month())
						nextdate = dateforsecondimm;

				}
			}
			auto expiry = expiryDate(nextdate);
			res.emplace(nextdate);
		}

		return res;

	}

	std::set<Date> IMMOptionsDateGenerator::Chain_serial(const Date& asof,
		const size_t& Ntotal,
		const size_t& N1,
		std::optional<unsigned> earlyRollOffset)
	{
		unsigned counter = 0;
		const auto modifiedAsOfDate = shiftDate(asof, earlyRollOffset.value_or(0.0)); //

		Date nextdate = modifiedAsOfDate;
		std::set<Date> res;
		for (unsigned i = 0; i < std::fabs(Ntotal); i++)
		{
			if (counter >= N1)
			{
				if (Ntotal > 0)
					secondimmgenerator_.next(nextdate);
				else
					secondimmgenerator_.prev(nextdate);
			}
			else //
			{
				Date dateforsecondimm = nextdate;
				if (Ntotal > 0)
				{
					secondimmgenerator_.next(dateforsecondimm);
					nextdate = nextIMMDate(nextdate);
					if (dateforsecondimm.month() == nextdate.month())
						nextdate = dateforsecondimm;
					else
						counter++;
				}
				else
				{
					secondimmgenerator_.prev(dateforsecondimm);
					nextdate = prevIMMDate(nextdate);
					if (dateforsecondimm.month() == nextdate.month())
						nextdate = dateforsecondimm;
					else
						counter++;

				}
			}
			auto expiry = expiryDate(nextdate);
			if (expiry > modifiedAsOfDate)
				res.emplace(nextdate);
		}

		return res;
	}

	IMMDateGenerator::IMMDates IMMOptionsDateGenerator::nextIMMDatePair(const Date& asof)
	{

		Date nextdate = asof;
		if (nextdate > cutoffdate_)
			secondimmgenerator_.next(nextdate);
		else
		{
			Date dateforsecondimm = nextdate;
			secondimmgenerator_.next(dateforsecondimm);
			immiterator_.next(nextdate);
			if (dateforsecondimm.month() == nextdate.month())
				nextdate = dateforsecondimm;
		}
		//return std::make_pair(expiryDate(nextdate), nextdate);
		return  IMMDates(std::move(expiryDate(nextdate)), std::move(nextdate));
	}



	std::set<Date> IMMFiOptionsDateGenerator::Chain_serial(const Date& asof,
		const size_t& Ntotal,
		const size_t& N1,
		std::optional<unsigned> earlyRollOffset,
		bool isMidurve)
	{
		unsigned counter = 0;
		const auto modifiedAsOfDate = shiftDate(asof, earlyRollOffset.value_or(0.0)); //

		Date nextdate = modifiedAsOfDate;
		Date expiry;
		std::set<Date> res;
		for (unsigned i = 0; i < std::fabs(Ntotal); i++)
		{
			if (counter >= N1)
			{
				if (Ntotal > 0)
				{
					nextdate = nextIMMDate(nextdate);
					expiry = expiryDate(nextdate);
					if (isMidurve)
						expiry = secondimmgenerator_.expiry(expiry);
				}
				else
				{
					nextdate = prevIMMDate(nextdate);
					expiry = expiryDate(nextdate);
					if (isMidurve)
						expiry = secondimmgenerator_.expiry(expiry);
				}
			}
			else //
			{
				Date dateforsecondimm = nextdate;
				if (Ntotal > 0)
				{
					secondimmgenerator_.next(dateforsecondimm);
					nextdate = nextIMMDate(nextdate);
					if (dateforsecondimm.month() != nextdate.month())
					{
						nextdate = dateforsecondimm;
						expiry = secondimmgenerator_.expiry();
						expiry=secondDateAdjuster_->adjust(expiry);
						counter++;
					}
					else
					{
						expiry = expiryDate(nextdate);
						if (isMidurve)
							expiry = secondimmgenerator_.expiry();
						
					}
				}
				else
				{
					secondimmgenerator_.prev(dateforsecondimm).expiry();
					nextdate = prevIMMDate(nextdate);
					if (dateforsecondimm.month() != nextdate.month())
					{
						nextdate = dateforsecondimm;
						expiry = secondimmgenerator_.expiry();
						expiry = secondDateAdjuster_->adjust(expiry);
						counter++;
					}
					else
					{
						expiry = expiryDate(nextdate);
						if (isMidurve)
							expiry = secondimmgenerator_.expiry();
						
					}

				}
			}
			if (expiry > modifiedAsOfDate)
				res.emplace(expiry);
			else
				if (counter >0)
					counter--; // move the counter for the serials back
		}
		return res;
	}
	/*  Number of expiries for EUREX contracts as defined in the docs
	 *  Up to 6 months: The three nearest successive calendar months, as well as the following quarterly month
		of the March, June, September and December cycle thereafter.
	 */

	std::set<Date> BondFutFiOptionsDateGenerator::Chain_serial(const Date& asof,
		const size_t& Ntotal,
		std::optional<unsigned> earlyRollOffset )
	{
		
		auto firstOfNextMonth = [](const Date& d )
		{
			Date n = d + Months(1);
			return  Date(n.year(), n.month(), 1d);
		};
		std::set<Date> res;
		// First check the date of the next month
		Date prevImm;
		Date nextMonth = firstOfNextMonth(asof);
		for (unsigned i = 0; i < Ntotal; i++)
		{
			// Check previous friday.
			auto prevFri = IMMHelper::prevWeekday(nextMonth, previousWeekday_);
			// Check also 2 buss day before 1st of the month
			auto TwoBusBefore = calendar_->shift(nextMonth ,BusinessDays(-2));
			if (TwoBusBefore <= prevFri) // have to take the previous Friday again
			{
				prevFri = IMMHelper::prevWeekday(prevFri, previousWeekday_);
			}
			auto to_insert = calendar_->previousOrSame(prevFri);
			if (to_insert > asof)
			{
				//  check if we're inserting the same value
				
				if(res.empty())
					res.emplace(calendar_->previousOrSame(prevFri));
				if (to_insert == *res.rbegin()) // we're stuck in the same month. we have to move on
				{
					i -= 1;
				}
				else
					res.emplace(calendar_->previousOrSame(prevFri));
			}
			else
				i = 0;

			if(res.size() >=3) // go to the next imm date and get the first month
			{
				IMMIterator imm(prevImm, 3_M, WeekdayIndexed(Wednesday, 3));
				imm.next(prevImm);
				// get the first of month
				nextMonth = Date(prevImm.year(), prevImm.month(), 1d);
			}
			else
			{
				nextMonth = firstOfNextMonth(nextMonth);
				prevImm = nextMonth;
			}
		}
		return res;
	}

	//std::set<Date> BondFutFiOptionsWeeklyDateGenerator::Chain_serial(const Date& asof,
	//	std::optional<unsigned> earlyRollOffset)
	//{
	//	Date rolled_date = calendar_->businessDateAfter(asof,earlyRollOffset.value_or(0));
	//	TP_REQUIRE(rolled_date < firtMonthlyDate_, "Expecting First Expiry date to be greater than rolled day");

	//	// We have only 4 expiries for the weeklies
	//	Date next = rolled_date;
	//	std::set<Date> res;
	//	unsigned i = 0;
	//	auto w1 = iso_week::year_weeknum_weekday{ firtMonthlyDate_.ymd() };
	//	auto w2 = iso_week::year_weeknum_weekday{ secondMonthlyDate_.ymd() };

	//	while(i < 4)
	//	{
	//		// next weekday
	//		next = IMMHelper::nextWeekday(next, previousWeekday_);
	//		auto expiry = calendar_->previousOrSame(next);
	//		// This is to make sure that we don't have an existing expiry that week
	//		//auto days = abs(std::chrono::duration_cast<Days>(expiry - firtMonthlyDate_)).count();
	//		auto w = iso_week::year_weeknum_weekday{ next.ymd() };
	//		if (( w == w1 ) || (w == w2)) // have to move to the next week
	//			continue;

	//		res.emplace(calendar_->previousOrSame(next));
	//		i += 1;
	//	}

	//	return res;
	//}

}