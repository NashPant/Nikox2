#pragma once

#include "TPDateTime/Date.h"
#include "TPDateTime/Calendar.h"
#include  "TPNikopolisUtils.h"
#include "TPDateTime/financial/BusinessDayConvention.h"
//#include 

namespace TP::date_time
{
	// fwd declarations
	class  Tenor;
	//class  BusinessDayConvention;
	struct CurrencyLag;
	struct BusinessDaysLag;

	FWD_DECLARE_SHAREDPTR(CalendarProvider)
	/*
	 * Basic API for adjusting dates according to the rules of the implementation.
	 */
	FWD_DECLARE_NOTNULLSHARED(DateAdjustment)
	class TPNIKOPOLIS_IMPEXP DateAdjustment  // NOLINT
	{
	public:
		virtual ~DateAdjustment() = default;

		[[nodiscard]] virtual Date adjust(const Date& date) const = 0;
		[[nodiscard]] virtual std::string getType() const = 0;

		/**
		* An instance that performs no adjustment.
		*/
		static DateAdjustmentConstPtr none() noexcept;

		/**
		* An instance that performs the max adjusted date of a series of adjusters
		*/
		static DateAdjustmentConstPtr(max)(std::vector<DateAdjustmentConstPtr>&& adjusters) noexcept;

		/**
		* An instance that performs the composition of adjusted dates from a series of adjusters
		*/
		static DateAdjustmentConstPtr compose(std::vector<DateAdjustmentConstPtr>&& adjusters) noexcept;

		/**
		* An instance that adjusts to next or same business day.
		*/
		static DateAdjustmentConstPtr following(const CalendarConstPtr& calendar) noexcept;
		static DateAdjustmentConstPtr following(const CalendarProviderConstPtr& referenceData, const Calendar::Code& calendar) noexcept;

		/**
		* An instance that adjusts to previous or same business day.
		*/
		static DateAdjustmentConstPtr preceding(const CalendarConstPtr& calendar) noexcept;
		static DateAdjustmentConstPtr preceding(const CalendarProviderConstPtr& referenceData, const Calendar::Code& calendar) noexcept;

		/**
		* An instance that adjusts to next or same business day unless over a month end.
		*/
		static DateAdjustmentConstPtr modifiedFollowing(const CalendarConstPtr& calendar) noexcept;
		static DateAdjustmentConstPtr modifiedFollowing(const CalendarProviderConstPtr& referenceData, const Calendar::Code& calendar) noexcept;

		/**
		* An instance that adjusts to previous or same business day unless over a month end.
		*/
		static DateAdjustmentConstPtr modifiedPreceding(const CalendarConstPtr& calendar) noexcept;
		static DateAdjustmentConstPtr modifiedPreceding(const CalendarProviderConstPtr& referenceData, const Calendar::Code& calendar) noexcept;

		/**
		* An instance that adjusts the date based on the business day convention.
		*/
		static DateAdjustmentConstPtr fromConvention(const BusinessDayConvention& convention, const CalendarConstPtr& calendar);
		static DateAdjustmentConstPtr fromConvention(const BusinessDayConvention& convention, const CalendarProviderConstPtr& referenceData, const Calendar::Code& calendar);

		/**
		 * An instance that can adjust a date by a specific number of business days.
		 *
		 * <remarks>
		 *    When adjusting a date, the specified number of business days is added. This is equivalent to repeatedly finding the next business day.
		 *    No business day adjustment is applied to it so if lag is 0B this adjustment will have no effect
		 * </remarks>
		 */
		static DateAdjustmentConstPtr businessDaysLag(const BusinessDays& lag, const CalendarConstPtr& calendar) noexcept;
		static DateAdjustmentConstPtr businessDaysLag(const CalendarProviderConstPtr& referenceData, const BusinessDays& lag, const Calendar::Code& calendar) noexcept;
		static DateAdjustmentConstPtr businessDaysLag(const CalendarProviderConstPtr& referenceData, const BusinessDaysLag& bDays) noexcept;

		/**
		 * An instance that can adjust a date by a specific number of calendar days/months.
		 */
		static DateAdjustmentConstPtr calendarDaysLag(const Days& days) noexcept;
		static DateAdjustmentConstPtr calendarMonthsLag(const Months& months, bool eomRule = true) noexcept;
		/**
		 * An instance that adjusts to next frida, month, quarter and year
		 */
		static DateAdjustmentConstPtr eow() noexcept;
		static DateAdjustmentConstPtr eom() noexcept;
		static DateAdjustmentConstPtr eoq() noexcept;
		static DateAdjustmentConstPtr eoy() noexcept;
		/**
		 * Obtains an instance that can calculate the spot date using market convention FX rules.
		 *
		 * <remarks>
		 *    - Special rules are used for latin American (Latam) currencies
		 *    - We use the calendar provider to allow caching of merged calendars
		 * </remarks>
		 */
		 // todo merge both cases
		static DateAdjustmentConstPtr // for a given currency against USD
			fxSpot(const CalendarProviderConstPtr& referenceData, const CurrencyLag& ccy, const Calendar::Code& usd);
		static DateAdjustmentConstPtr //  for a given non USD cross.
			fxSpot(const CalendarProviderConstPtr& referenceData, const CurrencyLag& left, const CurrencyLag& right, const Calendar::Code& usd);

		/**
		 * Obtains an instance that can calculate the delivery date of an fx forward
		 */
		 // todo merge both cases
		static DateAdjustmentConstPtr // for a given currency against USD
			fxForward(const CalendarProviderConstPtr& referenceData, const Tenor& t, const CurrencyLag& ccy, const Calendar::Code& usd);
		static DateAdjustmentConstPtr //  for a given non USD cross.
			fxForward(const CalendarProviderConstPtr& referenceData, const Tenor& t, const CurrencyLag& left, const CurrencyLag& right, const Calendar::Code& usd);

		/**
		 * Obtains an instance that can calculate the front and back delivery dates of an fx swap (first is the front date and second is the back date)
		 */
		 // todo merge both cases
		static std::pair<DateAdjustmentConstPtr, DateAdjustmentConstPtr> // for a given currency against USD
			fxSwap(const CalendarProviderConstPtr& referenceData, const Tenor& t, const CurrencyLag& ccy, const Calendar::Code& usd);
		static std::pair<DateAdjustmentConstPtr, DateAdjustmentConstPtr> //  for a given non USD cross.
			fxSwap(const CalendarProviderConstPtr& referenceData, const Tenor& t, const CurrencyLag& left, const CurrencyLag& right, const Calendar::Code& usd);

	};
}

