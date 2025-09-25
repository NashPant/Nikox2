#pragma once
#include "DateIterator.h"
#include "DateInterface.h"
#include "TPTools/Time/TPDate.h"

namespace TP::date_time
{
	/**
	* \brief TODO document this
	*/
	/*
	struct IIMMIterator
	{
		// TODO need to rewrite the usage of object
		virtual ~IIMMIterator() = default;
		virtual IIMMIterator& next(Date& current) { return *this; } // = 0;
		virtual IIMMIterator& prev(Date& current) { return *this; } // = 0;

		[[nodiscard]] virtual bool isIMMDate(const Date& asOf) const { return false; } // = 0;
	};
	*/
	/**
	* @brief TODO document this
	*/
	struct  IMMIterator final :  DateIt<IMMIterator>
	{
		IMMIterator() = default;

		// remember years are just 12 months so let chrono take care of the implicit conversion
		IMMIterator(const Date& current, const Months& duration, const WeekdayIndexed& weekdayIndex)
			: DateIt<IMMIterator>(current), duration_(duration), weekday_(weekdayIndex) { }

		IMMIterator& next(Date& current);
		IMMIterator& prev(Date& current);

		/*
		 * @brief  Checks if the given date is an IMM date.
		 *		  We move forward first to the next date, then backwards and
		 *		  check if the dates match
		 */
		[[nodiscard]] virtual bool isIMMDate(const Date& asOf) const;

	private:
		Months			duration_{};
		WeekdayIndexed  weekday_{};
	};

	//IMMIterator::IMMIterator(const Date& current, const Months& duration, const WeekdayIndexed& weekdayIndex)
	//	: DateIt<IMMIterator>(current), duration_(duration), weekday_(weekdayIndex) { }

	inline IMMIterator& IMMIterator::next(Date& current)
	{
		const auto cym = current.ym();
		const auto thisMonthImm = cym / weekday_;
		const auto remainder = static_cast<unsigned>(cym.month()) % static_cast<unsigned>(duration_.count());
		if (remainder == 0 && current < thisMonthImm)
		{
			current = thisMonthImm;
		}
		else
		{
			const auto months = duration_ - Months{ remainder };
			current = (cym + months) / weekday_;
		}

		return *this;
	}

	inline IMMIterator& IMMIterator::prev(Date& current)
	{
		const auto cym = current.ym();
		const auto thisMonthImm = cym / weekday_;
		const auto remainder = static_cast<unsigned>(cym.month()) % static_cast<unsigned>(duration_.count());
		if (remainder == 0 && current > thisMonthImm)
		{
			current = thisMonthImm;
		}
		else
		{
			const auto months = remainder == 0 ? duration_ : Months{ remainder };
			current = (cym - months) / weekday_;
		}

		return *this;
	}

	inline bool IMMIterator::isIMMDate(const Date& asOf) const
	{
		auto it = IMMIterator(asOf, duration_, weekday_);
		++it; --it;
		return *it == asOf;
	}
	/**
	 * @brief TODO document this
	 */
	struct  ImmBrlDiIterator final :  DateIt<ImmBrlDiIterator>
	{
		ImmBrlDiIterator() = default;

		// remember years are just 12 months so let chrono take care of the implicit conversion
		ImmBrlDiIterator(const Date& current, const Months& duration);

		ImmBrlDiIterator& next(Date& current);
		ImmBrlDiIterator& prev(Date& current) ;

		/*
		 * @brief  Checks if the given date is an IMM date.
		 *		  We move forward first to the next date, then backwards and
		 *		  check if the dates match
		 */
		[[nodiscard]] bool isIMMDate(const Date& asOf) const ;

	private:
		Months duration_{};
	};

	/**
	 * @brief  IMM Iterator for the serial expiries where it it rolls monthly on IMM dates, i.e. Wed[3] but actual expiry is previous
	 *		   Friday
	 */
	struct  ImmSerialIterator final : DateIt<ImmSerialIterator>
	{
		ImmSerialIterator() = default;

		// remember years are just 12 months so let chrono take care of the implicit conversion
		ImmSerialIterator(const Date& current, const Months& duration = Months(1), const WeekdayIndexed& weekdayIndex=Wed[3], const Weekday& whichDay = Friday)
			: DateIt<ImmSerialIterator>(current), duration_(duration), weekday_(weekdayIndex), previousWeekday_(whichDay),
			actualExpiry_(std::nullopt){ }

		ImmSerialIterator& next(Date& current);
		ImmSerialIterator& prev(Date& current);

		[[nodiscard]] bool isIMMDate(const Date& asOf) const;

		Date   expiry() const { return actualExpiry_.value_or(Date()); }
		Date   expiry(const Date& expiry_dt) const { return IMMHelper::prevWeekday(expiry_dt, previousWeekday_);}
	private:
		Months			duration_{};
		WeekdayIndexed  weekday_{};
		Weekday			previousWeekday_{};
		std::optional<Date>			actualExpiry_;
	};

	inline ImmSerialIterator& ImmSerialIterator::next(Date& current)
	{
		size_t months_add = 0;
		//if (isIMMDate(current)) // add a month to pass the cycle
		//	months_add = 1;
		const auto cym = current.ym();
		auto thisMonthImm = cym / weekday_;
		thisMonthImm += Months(months_add);
		const auto remainder = static_cast<unsigned>(cym.month()) % static_cast<unsigned>(duration_.count());
		if (remainder == 0 && current < thisMonthImm)
		{
			current = thisMonthImm;
		}
		else
		{
			const auto months = duration_ - Months{ remainder };
			current = (cym + months) / weekday_;
		}
		actualExpiry_ = IMMHelper::prevWeekday(current, previousWeekday_);
		return *this;
	}

	inline ImmSerialIterator& ImmSerialIterator::prev(Date& current)
	{
		const auto cym = current.ym();
		const auto thisMonthImm = cym / weekday_;
		const auto remainder = static_cast<unsigned>(cym.month()) % static_cast<unsigned>(duration_.count());
		if (remainder == 0 && current > thisMonthImm)
		{
			current = thisMonthImm;
		}
		else
		{
			const auto months = remainder == 0 ? duration_ : Months{ remainder };
			current = (cym - months) / weekday_;
		}
		actualExpiry_ = IMMHelper::prevWeekday(current, previousWeekday_);
		return *this;
	}

	inline bool ImmSerialIterator::isIMMDate(const Date& asOf) const
	{
		auto it = ImmSerialIterator(asOf, duration_, weekday_, previousWeekday_);
		++it; --it;
		return *it == asOf;
	}
	
}
