////////////////////////////////////////////////////////////////////////////
//
//  This program is the CONFIDENTIAL and PROPRIETARY property of'
//   Athanasios Pantas. Unauthorized use and reproduction'
//  of this program is strictly prohibited.
//
// Copyright (C) 2003  - Nikopolis
//  Copyright (C) 2003 - Athanasios Pantas.
//  ALL RIGHTS RESERVED.
/////////////////////////////////////////////////////////////////////////////

#ifndef TP_DAYCOUNTER_H_38_
#define TP_DAYCOUNTER_H_38_

#include <TPNikopolisUtils.h>
#include <string>
#include <boost/optional.hpp>
#include "TPTools/Time/TPDate.h"
#include "TPDateTime/Date.h"
#include "TPDateTime/Calendar.h"
#include "TPDateTime\Schedule\conventions\DateConvention.h"
#include "TPTools/Utils/TPEnum.h"
#include "TPDateTime/Durations.h"

namespace TP::date_time::day_count {

	using date_time::Date;
	using date_time::OptDate;
	using date_time::CalendarConstPtr;
	class DayCounter;
	using DayCounterConstPtr = std::shared_ptr<const DayCounter>;

	using date_time::CalendarConstPtr;
	using date_time::Duration;
	using date_time::DateConventionConstPtr;

	class TPNIKOPOLIS_IMPEXP DayCounter
	{
	public:
		TP_ENUM_CREATE(Code, ACT360, ACT365, ACTACT, ACTACTBOND, ISDA30360, ISMA30360, GERMAN30360, NL365, BUS252, FRACMONTH, CONSTANT, USWITACTACT, TBILLSACT365, CONT)
			/**
				* The 'Act/360' day count, which divides the actual number of days by 360.
				* <p>
				* The result is a simple division.
				* The numerator is the actual number of days in the requested period.
				* The denominator is always 360.
				* <p>
				* Also known as 'Actual/360' or 'French'.
				* Defined by the 2006 ISDA definitions 4.16e and ICMA rule 251.1(i) part 1.
				*/
			TP_ENUM_VALUE(ACT360, "Act360")
			TP_ENUM_VALUE(ACT360, "Act/360")
			TP_ENUM_VALUE(ACT360, "_ACT360")

			/**
				* The 'Act/365F' day count, which divides the actual number of days by 365 (fixed).
				*
				* The result is a simple division.
				* The numerator is the actual number of days in the requested period.
				* The denominator is always 365.
				*
				* Also known as 'Act/365', 'Actual/365 Fixed' or 'English'.
				* Defined by the 2006 ISDA definitions 4.16d.
				*/
			TP_ENUM_VALUE(ACT365, "Act365")
			TP_ENUM_VALUE(ACT365, "Act/365")
			TP_ENUM_VALUE(ACT365, "_Act365")

			/**
				* The 'Act/Act ISDA' day count, which divides the actual number of days in a
				* leap year by 366 and the actual number of days in a standard year by 365.
				* <p>
				* The result is calculated in two parts.
				* The actual number of days in the requested period that fall in a leap year is divided by 366.
				* The actual number of days in the requested period that fall in a standard year is divided by 365.
				* The result is the sum of the two.
				* The first day in the period is included, the last day is excluded.
				* <p>
				* Also known as 'Actual/Actual'.
				* Defined by the 2006 ISDA definitions 4.16b.
				*/
			TP_ENUM_VALUE(ACTACT, "ActAct")
			TP_ENUM_VALUE(ACTACT, "Act/Act")
			TP_ENUM_VALUE(ACTACT, "_ActAct")

			/**
				* The 'Act/Act ICMA' day count, which divides the actual number of days by
				* the actual number of days in the coupon period multiplied by the frequency.
				*
				* The result is calculated as follows.
				*
				* First, the underlying schedule period is obtained treating the first date as the start of the schedule period.
				*
				* Second, if the period is a stub, then nominal regular periods are created matching the
				* schedule frequency, working forwards or backwards from the known regular schedule date.
				* An end-of-month flag is used to handle month-ends.
				* If the period is not a stub then the schedule period is treated as a nominal period below.
				*
				* Third, the result is calculated as the sum of a calculation for each nominal period.
				* The actual days between the first and second date are allocated to the matching nominal period.
				* Each calculation is a division. The numerator is the actual number of days in
				* the nominal period, which could be zero in the case of a long stub.
				* The denominator is the length of the nominal period  multiplied by the frequency.
				* The first day in the period is included, the last day is excluded.
				*
				* Due to the way that the nominal periods are determined ignoring business day adjustments,
				* this day count is recommended for use by bonds, not swaps.
				*
				* Also known as 'Actual/Actual ICMA' or 'Actual/Actual (Bond)'.
				* Defined by the 2006 ISDA definitions 4.16c and ICMA rule 251.1(iii) and 251.3
				* as later clarified by ISDA 'EMU and market conventions' http://www.isda.org/c_and_a/pdf/mktc1198.pdf.
				*/
			TP_ENUM_VALUE(ACTACTBOND, "ActActBond")
			TP_ENUM_VALUE(ACTACTBOND, "Act/Act (Bond)")

			/**
				* \brief The '30/360 ISDA' day count, which treats input day-of-month 31 specially.
				*
				* The result is calculated as {@code (360 * deltaYear + 30 * deltaMonth + deltaDay) / 360}.
				* The deltaDay is calculated once day-of-month adjustments have occurred.
				* If the second day-of-month is 31 and the first day-of-month is 30 or 31, change the second day-of-month to 30.
				* If the first day-of-month is 31, change the first day-of-month to 30.
				*
				* Also known as '30/360 U.S. Municipal' or '30/360 Bond Basis'.
				* Defined by the 2006 ISDA definitions 4.16f.
				*/
			TP_ENUM_VALUE(ISDA30360, "30360")
			TP_ENUM_VALUE(ISDA30360, "30/360")
			TP_ENUM_VALUE(ISDA30360, "30/360 ISDA")
			TP_ENUM_VALUE(ISDA30360, "_30360")

			/**
				* The '30E/360' day count, which treats input day-of-month 31 specially.
				*
				* The result is calculated as {@code (360 * deltaYear + 30 * deltaMonth + deltaDay) / 360}.
				* The deltaDay is calculated once day-of-month adjustments have occurred.
				* If the first day-of-month is 31, it is changed to 30.
				* If the second day-of-month is 31, it is changed to 30.
				*
				* Also known as '30/360 ISMA', '30/360 European', '30S/360 Special German' or 'Eurobond'.
				* Defined by the 2006 ISDA definitions 4.16g and ICMA rule 251.1(ii) and 252.2.
				*/
			TP_ENUM_VALUE(ISMA30360, "30E360")
			TP_ENUM_VALUE(ISMA30360, "30E/360")
			TP_ENUM_VALUE(ISMA30360, "30/360 ISMA")
			TP_ENUM_VALUE(ISMA30360, "GER:30/360 NON-EOM")
			TP_ENUM_VALUE(ISMA30360, "ISMA-30/360")


			/**
				* The '30E/360 ISDA' day count, which treats input day-of-month 31 and end of February specially.
				*
				* The result is calculated as {@code (360 * deltaYear + 30 * deltaMonth + deltaDay) / 360}.
				* The deltaDay is calculated once day-of-month adjustments have occurred.
				* If the first day-of-month is 31, change the first day-of-month to 30.
				* If the second day-of-month is 31, change the second day-of-month to 30.
				* If the first date is the last day of February, change the first day-of-month to 30.
				* todo: fix If the second date is the last day of February and it is not the maturity date, change the second day-of-month to 30.
				*
				* Also known as '30E/360 German' or 'German'.
				* Defined by the 2006 ISDA definitions 4.16h.
				*/
			TP_ENUM_VALUE(GERMAN30360, "30360German")
			TP_ENUM_VALUE(GERMAN30360, "30E/360 ISDA")
			TP_ENUM_VALUE(GERMAN30360, "30E/360 GERMAN")

			/**
				* The 'NL/365' day count, which divides the actual number of days omitting leap days by 365.
				*
				* The result is a simple division.
				* The numerator is the actual number of days in the requested period minus the number of occurrences of February 29.
				* The denominator is always 365.
				* The first day in the period is excluded, the last day is included.
				*
				* Also known as 'Actual/365 No Leap'.
				*/
			TP_ENUM_VALUE(NL365, "NL365")
			TP_ENUM_VALUE(NL365, "NL/365")

			/**
				* The 'Bus/252' day count based on a specific calendar.
				*
				* The 'Bus/252' day count is unusual in that it relies on a specific holiday calendar.
				* The calendar is stored within the day count.
				*
				* This day count is typically used in Brazil.
				*/
			TP_ENUM_VALUE(BUS252, "Bus252")
			TP_ENUM_VALUE(BUS252, "_Bd252")
			TP_ENUM_VALUE(BUS252, "_Bus252")
			TP_ENUM_VALUE(BUS252, "Bus/252")

			/*
				* Used for converting tenor to double, comparing tenor lengths and interpolating between tenors
				* Result is the day count in terms of months and fractional months
				*/
			TP_ENUM_VALUE(FRACMONTH, "frac_month")
			TP_ENUM_VALUE(FRACMONTH, "fracmonth")

			/*
				* Used for US When-Issued bonds
			*/
			TP_ENUM_VALUE(USWITACTACT, "Act/Act (USWIT)")


			/*
			* Used for T-Bills
			*/
			TP_ENUM_VALUE(TBILLSACT365, "TBILLSACT365")
			TP_ENUM_VALUE(TBILLSACT365, "Act/365 TBill")
			TP_ENUM_VALUE(TBILLSACT365, "Act365 TBill")
			TP_ENUM_VALUE(TBILLSACT365, "Act/365 (TBill)")
			/*
			* Used for returning a constant day count regardless of start and end date
			* Result is the day count same fixed number as input
			*
			* This day count is typical used for single look product
			*/
			TP_ENUM_VALUE(CONSTANT, "Constant")
			TP_ENUM_VALUE(CONSTANT, "Const")
			/*
			  @brief. Translation from TPBasis really, just ACT/365.25
			*/
			TP_ENUM_VALUE(CONT, "continuous")
			TP_ENUM_VALUE(CONT, "cont")


			TP_ENUM_CREATE_END(Code)

			virtual ~DayCounter() = default;

		static DayCounterConstPtr actual360();
		static DayCounterConstPtr actual365();
		static DayCounterConstPtr actualActual();
		static DayCounterConstPtr continous();
		static DayCounterConstPtr actualActualBond(const Duration& tenor, const DateConventionConstPtr& dateConvention,
			bool isUSWIT);

		static DayCounterConstPtr thirty360();
		static DayCounterConstPtr icma30360();
		static DayCounterConstPtr german30360();
		static DayCounterConstPtr NL365();
		static DayCounterConstPtr business252(const CalendarConstPtr& calendar);
		static DayCounterConstPtr TBillsAct365();
		static DayCounterConstPtr FracMonth(const DateConventionConstPtr & dateConvention);
		static DayCounterConstPtr Const(const double dcf);
		/*
		  @brief. Create daycounter from old style basis
		*/
		static DayCounterConstPtr fromTPBasis(const TPBasis& tpBasis);

		static DayCounterConstPtr fromString(const std::string & input, const std::optional<CalendarConstPtr> & calendarInput = std::nullopt);
		static std::string calendarName(const std::string & input);
		
		virtual double calculateDcf(const Date& from, const Date& to,
		const OptDate& currentPeriodStartDate = std::nullopt, const OptDate& currentPeriodEndDate = std::nullopt) const = 0;
		/*
		  @brief. Alias of the above really with only from and to
		*/
		virtual double getDayCountFraction(const Date& from, const Date& to) const
		{
			return calculateDcf(from, to);
		}
		virtual double getDayCountFraction(const Date& from, const Date& to, const Date& refstartDate, const Date& refendDate) const
		{
			return calculateDcf(from, to, refstartDate, refendDate);
		}
		virtual std::string toString() const = 0;
		virtual DayCounter::Code type() const = 0;
			
	};

}

#endif
