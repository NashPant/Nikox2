#pragma once
#include "TPNikopolisUtils.h"
#include "IMMIterators.h"
#//include "date_time/futures/ImmDateDefinitionEnum.h"
#include "TPTools/Utils/NotNull.h"
#include "Utils/CBDateIndexed.h"
#include "TPDateTime/financial/DateAdjustment.h"

namespace TP::date_time
{
	FWD_DECLARE_NOTNULLSHARED(DateAdjustment)
	FWD_DECLARE_NOTNULLSHARED(Calendar)
}
namespace TP::date_time
{
	/*
	 * @brief.  IMMDateGenerator. Composite class which contains:
	 *			1) IMMIterator. This will be responsible for generating the actual IMM Schedule.
	 *			2) DateConventionConstPtr. Given the above schedule of IMMdate, DateConventionConstPtr will produce
	 *			   the actual expiry dates for the given future.
	 *
	 */
	class TPNIKOPOLIS_IMPEXP IMMDateGenerator
	{
	public:

		struct IMMDates
		{
			IMMDates(Date&& ex, Date&& im) :expiry_(std::forward<Date>(ex)), effective_(std::forward<Date>(im))
			{}
			Date expiry_;
			Date effective_;
		};

		IMMDateGenerator(
			const DateAdjustmentConstPtr&  busdaylag,
			const CalendarConstPtr& calendar,
			const IMMIterator&	immiterator
		) :
			dateadjuster_(busdaylag),
			calendar_(calendar),
			immiterator_(immiterator)
		{}

		IMMDateGenerator(const IMMDateGenerator& other) = default;
		IMMDateGenerator& operator=(const IMMDateGenerator& other) = delete;

		/*
		 * @brief.  Computes next IMM Date Pair as <Expiry,IMMDate>
		 *			so expiry is simply: IMMDate - busdayslag (According to calendar)
		 */

		[[nodiscard]] virtual IMMDates nextIMMDatePair(const Date& asof);
		[[nodiscard]] IMMDates nextIMMDatePair();
		/*
		 * @brief.  THis will just return the imm date (and not the expiry)
		 */
		[[nodiscard]] Date nextIMMDate(const Date& asof);
		/*
		 * @brief.  THis will just return the previous imm date (and not the expiry)
		 */
		[[nodiscard]] Date	prevIMMDate(const Date& asof);

		[[nodiscard]] bool isIMMDate(const Date& date) { return immiterator_.isIMMDate(date); }

		[[nodiscard]] Date nthContract(
			const Date& asOfDate,
			unsigned int nthContract,
			bool includeToday,
			std::optional<unsigned> earlyRollOffset = std::nullopt
		);


		/*
		 * @brief.  Given an asof and the number of contracts, N, will return
		 *		    the set of ExpiryDates only (i.e. IMMDate - busdayslag (According to calendar)) for
		 *		    NContracts  from asof.
		 *
		 *		    NContracts: Number of contracts to include from asofdate.
		 */
		virtual std::set<Date> Chain(const Date& asof,
			const size_t& NContracts,
			std::optional<unsigned> earlyRollOffset = std::nullopt);


		const IMMIterator& immDateSpec() const { return immiterator_; }

		[[nodiscard]] Date    expiryDate(const Date& asof) const;


		[[nodiscard]] Date    shiftDate(const Date& asof, const size_t& d) const;


	protected:
		DateAdjustmentConstPtr                            dateadjuster_;
		CalendarConstPtr                                  calendar_; // for early roll offset
		IMMIterator									      immiterator_;
	};


	/*
	* @brief.   Specific case for IMM otions where we mix serial and standard expiries.
	*		     For this case we have for example as base 1M IMM Iterator up to the cuttoffdate
	*		     and after that we switch to 3M Iterator. But there is a caveat in this case.
	*		     IF immdate < cuttoffdate and month(nextIMMDate(1M IMM Iterator)) = month(nextIMMDate(3M IMM Iterator))
	*		     we simply choose nextIMMDate(3M IMM Iterator)
	*/
	class TPNIKOPOLIS_IMPEXP IMMOptionsDateGenerator :public IMMDateGenerator
	{

	public:

		IMMOptionsDateGenerator(const IMMOptionsDateGenerator& other) = default;
		IMMOptionsDateGenerator& operator=(const IMMOptionsDateGenerator& other) = delete;

		/*
		 * brief.  Default constructors which by default uses base immiterator 1M/Fri[2] and
		 *	       second one 3M/Wed[3]
		 */
		IMMOptionsDateGenerator(const DateAdjustmentConstPtr&  busdaylag,
			const CalendarConstPtr& calendar,
			const Date&		cutoff) :
			IMMDateGenerator(busdaylag, calendar, IMMIterator(Date(), 1_M, Fri[2])),
			secondimmgenerator_(IMMIterator(Date(), 3_M, Wed[3])),
			cutoffdate_(cutoff)
		{}

		IMMOptionsDateGenerator(const DateAdjustmentConstPtr&  busdaylag,
			const CalendarConstPtr& calendar,
			const IMMIterator&		baseimmiterator,
			const IMMIterator&		secondimmiterator,
			const Date&		cutoff) :
			IMMDateGenerator(busdaylag, calendar, baseimmiterator),
			secondimmgenerator_(secondimmiterator),
			cutoffdate_(cutoff)
		{}

		[[nodiscard]] IMMDates nextIMMDatePair(const Date& asof) override;

		[[nodiscard]] std::set<Date> Chain(const Date& asof,
			const size_t& NContracts,
			std::optional<unsigned> earlyRollOffset = std::nullopt) override;

		/*
		 * @brief.  special case of the above where we specify the total number of contracts
		 *          and the number of serial contracts to use. so for example for N1=3 serial contracts
		 *          sequence can look like
		 *          [s1,s2,q1,s3,q2,q2,...] , where s_i are the monthly generators and q_i the quarterly
		 */
		std::set<Date> Chain_serial(const Date& asof,
			const size_t& Ntotal,
			const size_t& N1,
			std::optional<unsigned> earlyRollOffset = std::nullopt);

	private:
		IMMIterator secondimmgenerator_;
		Date        cutoffdate_;
	};

	/*
	 * @brief. Difference from the above is the serial generator that is of type ImmSerialIterator
	 */
	class TPNIKOPOLIS_IMPEXP IMMFiOptionsDateGenerator :public IMMDateGenerator
	{

	public:

		IMMFiOptionsDateGenerator(const IMMFiOptionsDateGenerator& other) = default;
		IMMFiOptionsDateGenerator& operator=(const IMMFiOptionsDateGenerator& other) = delete;

		/*
		 * brief.  Default constructors which by default uses base immiterator 1M/Fri[2] and
		 *	       second one 3M/Wed[3]
		 */
		IMMFiOptionsDateGenerator(const DateAdjustmentConstPtr&  busdaylag,
			const CalendarConstPtr& calendar,
			const Date&		cutoff) :
			IMMDateGenerator(busdaylag, calendar, IMMIterator(Date(), 3_M, Wed[3])),
			secondimmgenerator_(ImmSerialIterator(Date(), 1_M, Wed[3], Friday)),
			cutoffdate_(cutoff),
			secondDateAdjuster_ (DateAdjustment::preceding(calendar))
		{
		}

		IMMFiOptionsDateGenerator(const DateAdjustmentConstPtr&  busdaylag,
			const CalendarConstPtr& calendar,
			const IMMIterator&		baseimmiterator,
			const ImmSerialIterator&		secondimmiterator,
			const Date&		cutoff) :
			IMMDateGenerator(busdaylag, calendar, baseimmiterator),
			secondimmgenerator_(secondimmiterator),
			cutoffdate_(cutoff),
			secondDateAdjuster_(DateAdjustment::preceding(calendar))
		{}

		/*[[nodiscard]] IMMDates nextIMMDatePair(const Date& asof) override;

		[[nodiscard]] std::set<Date> Chain(const Date& asof,
			const size_t& NContracts,
			std::optional<unsigned> earlyRollOffset = std::nullopt) override;*/

		/*
		 * @brief.  special case of the above where we specify the total number of contracts
		 *          and the number of serial contracts to use. so for example for N1=3 serial contracts
		 *          sequence can look like
		 *          [s1,s2,q1,s3,q2,q2,...] , where s_i are the monthly generators and q_i the quarterly
		 */
		std::set<Date> Chain_serial(const Date& asof,
			const size_t& Ntotal,
			const size_t& N1,
			std::optional<unsigned> earlyRollOffset = std::nullopt,
			bool isMidurve = false);

	private:
		ImmSerialIterator secondimmgenerator_;
		Date        cutoffdate_;
		DateAdjustmentConstPtr secondDateAdjuster_;
	};


	/**
	 * @brief  Bond Futures Expiry Date Generation. Rules as follows: (from Eurex Website)
	 *
	 *  Last trading day shall be the last Friday prior to the first calendar day of the expiration month, that is followed
		by at least two exchange days prior to the first calendar day of the expiration month. Unless at least two
		exchange days lie between the last Friday of a month and the first calendar day of the expiration month,
		the last trading day shall be the Friday preceding the last Friday of a month. If such Friday is not an exchange
		day, then the exchange day immediately preceding such Friday shall be the last trading day. Close of trading
		in all option series on the last trading day is at 17:15.
	 *
	 */
	struct TPNIKOPOLIS_IMPEXP  BondFutFiOptionsDateGenerator final 
	{
		BondFutFiOptionsDateGenerator() = default;

		// remember years are just 12 months so let chrono take care of the implicit conversion
		BondFutFiOptionsDateGenerator(const DateAdjustmentConstPtr&  busdaylag,
			const CalendarConstPtr& calendar,
			const Weekday& previousDate = Friday) :
			dateadjuster_(busdaylag),
			calendar_(calendar),
			previousWeekday_(previousDate)
		{
		}

		std::set<Date> Chain_serial(const Date& asof,
			const size_t& Ntotal,
			std::optional<unsigned> earlyRollOffset = std::nullopt);
	protected:
		DateAdjustmentConstPtr                            dateadjuster_;
		CalendarConstPtr                                  calendar_; // for early roll offset
		Weekday											previousWeekday_{};
	};

	/*
	 * @brief. Weekly dates generators. Basically all Fridays between 2 successive monthly options that are not
	 *		   the give monthly options. (The weekday is usually either Friday or Wed
	 */
	//struct TPNIKOPOLIS_IMPEXP  BondFutFiOptionsWeeklyDateGenerator final
	//{
	//	BondFutFiOptionsWeeklyDateGenerator() = default;

	//	// remember years are just 12 months so let chrono take care of the implicit conversion
	//	BondFutFiOptionsWeeklyDateGenerator(const DateAdjustmentConstPtr&  busdaylag,
	//		const CalendarConstPtr& calendar,
	//		const Date& fistMDate,
	//		const Date& secondMDate,
	//		const Weekday& previousDate = Friday) :
	//		dateadjuster_(busdaylag),
	//		calendar_(calendar),
	//		firtMonthlyDate_(fistMDate),
	//		secondMonthlyDate_(secondMDate),
	//		previousWeekday_(previousDate)
	//	{
	//		TP_REQUIRE(secondMonthlyDate_ > firtMonthlyDate_, "Expecting Second expiry to be less than first expiry");
	//	}

	//	std::set<Date> Chain_serial(const Date& asof,
	//		std::optional<unsigned> earlyRollOffset = std::nullopt);
	//protected:
	//	DateAdjustmentConstPtr                            dateadjuster_;
	//	CalendarConstPtr                                  calendar_; // for early roll offset
	//	Date											firtMonthlyDate_;
	//	Date											secondMonthlyDate_;
	//	Weekday											previousWeekday_{};
	//};


}