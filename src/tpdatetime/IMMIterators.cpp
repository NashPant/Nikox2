#include "IMMIterators.h"

namespace TP::date_time
{

	ImmBrlDiIterator::ImmBrlDiIterator(const Date& current, const Months& duration)
		: duration_(duration)
	{}

	ImmBrlDiIterator& ImmBrlDiIterator::next(Date& current)
	{
		auto condidateDate = current;
		const auto cym = current.ym();
		const auto thisMonthImm = cym / 1;
		const auto remainder = static_cast<unsigned>(cym.month()) % static_cast<unsigned>(duration_.count());
		if (remainder == 0 && current < thisMonthImm)
		{
			current = thisMonthImm;
		}
		else
		{
			const auto months = duration_ - Months{ remainder };
			current = (cym + months) / 1;
		}

		return *this;
	}

	ImmBrlDiIterator& ImmBrlDiIterator::prev(Date& current)
	{
		const auto cym = current.ym();
		const auto thisMonthImm = cym / 1;
		const auto remainder = static_cast<unsigned>(cym.month()) % static_cast<unsigned>(duration_.count());
		if (remainder == 0 && current > thisMonthImm)
		{
			current = thisMonthImm;
		}
		else
		{
			const auto months = remainder == 0 ? duration_ : Months{ remainder };
			current = (cym - months) / 1;
		}

		return *this;
	}

	bool ImmBrlDiIterator::isIMMDate(const Date& asOf) const
	{
		auto it = ImmBrlDiIterator(asOf, duration_);
		++it; --it;
		return *it == asOf;
	}


	


}
