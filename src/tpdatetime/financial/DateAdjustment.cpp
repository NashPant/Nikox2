#include "DateAdjustment.h"

#include "TPDateTime/financial/BusinessDayConvention.h"
#include "TPDateTime/financial/Lags.h"
#include "TPDateTime/CalendarProvider.h"
#include "TPDateTime/Tenor.h"
#include "TPDateTime/BusinessDay.h"

#include "TPTools/TPException.h"

#include <utility>

namespace TP::date_time
{
	/**
	 * \brief unadjusted date
	 */
	class None final : public DateAdjustment
	{
	public:
		[[nodiscard]] Date adjust(const Date& date) const override { return date; }
		[[nodiscard]] std::string getType() const override { return "None"; }
	};

	DateAdjustmentConstPtr DateAdjustment::none() noexcept
	{
		const static auto none = utils::makeNotNullShared<const None>();
		return none;
	}

	/**
	 * \brief Eow Adjustment
	 */
	class EoWAdjustment final : public DateAdjustment
	{
	public:
		[[nodiscard]] Date adjust(const Date& date) const override { return date.next(Fri,rule::weekday::same); }
		[[nodiscard]] std::string getType() const override { return "EoWAdjustment"; }
	};
	DateAdjustmentConstPtr DateAdjustment::eow() noexcept
	{
		return utils::makeNotNullShared<EoWAdjustment>();
	}
	/**
	 * \brief end of month Adjustment
	 */
	class EoMAdjustment final : public DateAdjustment
	{
	public:
		[[nodiscard]] Date adjust(const Date& date) const override { return date.eom(); }
		[[nodiscard]] std::string getType() const override { return "EoMAdjustment"; }
	};
	DateAdjustmentConstPtr DateAdjustment::eom() noexcept
	{
		return utils::makeNotNullShared<EoMAdjustment>();
	}
	/**
	 * \brief end of Quarter Adjustment
	 */
	class EoQAdjustment final : public DateAdjustment
	{
	public:
		[[nodiscard]] Date adjust(const Date& date) const override { return date.eoq(); }
		[[nodiscard]] std::string getType() const override { return "EoQAdjustment"; }
	};
	DateAdjustmentConstPtr DateAdjustment::eoq() noexcept
	{
		return utils::makeNotNullShared<EoQAdjustment>();
	}
	/**
	 * \brief end of YEar Adjustment
	 */
	class EoYAdjustment final : public DateAdjustment
	{
	public:
		[[nodiscard]] Date adjust(const Date& date) const override { return date.eoy(); }
		[[nodiscard]] std::string getType() const override { return "EoYAdjustment"; }
	};
	DateAdjustmentConstPtr DateAdjustment::eoy() noexcept
	{
		return utils::makeNotNullShared<EoYAdjustment>();
	}
	/**
	 * \brief maximum of the adjusted date
	 */
	class Max final : public DateAdjustment
	{
	public:
		explicit Max(std::vector<DateAdjustmentConstPtr>&& dateAdjusters)
			: adjusters_(std::forward<std::vector<DateAdjustmentConstPtr>>(dateAdjusters))
		{
		}

		[[nodiscard]] Date adjust(const Date& date) const override
		{
			auto d = date;
			for (const auto& adjuster : adjusters_)
			{
				d = std::max(d, adjuster->adjust(date));
			}
			return d;
		}

		[[nodiscard]] std::string getType() const override { return "Max"; }

	private:
		std::vector<DateAdjustmentConstPtr> adjusters_;
	};

	DateAdjustmentConstPtr DateAdjustment::max(std::vector<DateAdjustmentConstPtr>&& adjusters) noexcept
	{
		return utils::makeNotNullShared<Max>(std::forward<std::vector<DateAdjustmentConstPtr>>(adjusters));
	}

	/**
	 * \brief chains adjusted dates
	 */
	class Compose final : public DateAdjustment
	{
	public:
		explicit Compose(std::vector<DateAdjustmentConstPtr>&& dateAdjusters)
			: adjusters_(std::forward<std::vector<DateAdjustmentConstPtr>>(dateAdjusters))
		{
		}

		[[nodiscard]] Date adjust(const Date& date) const override
		{
			auto d = date;
			for (const auto& adjuster : adjusters_)
			{
				d = adjuster->adjust(d);
			}
			return d;
		}
		[[nodiscard]] std::string getType() const override { return "Compose"; }
	private:
		std::vector<DateAdjustmentConstPtr> adjusters_;
	};


	DateAdjustmentConstPtr DateAdjustment::compose(std::vector<DateAdjustmentConstPtr>&& adjusters) noexcept
	{
		return utils::makeNotNullShared<Compose>(std::forward<std::vector<DateAdjustmentConstPtr>>(adjusters));
	}

	class Following final : public DateAdjustment
	{
	public:
		explicit Following(CalendarConstPtr calendar)
			: calendar_(std::move(calendar))
		{}

		[[nodiscard]] Date adjust(const Date& date) const override { return calendar_->nextOrSame(date); }
		[[nodiscard]] std::string getType() const override { return "Following"; }
	private:
		CalendarConstPtr calendar_;
	};

	DateAdjustmentConstPtr DateAdjustment::following(const CalendarConstPtr& calendar) noexcept
	{
		return utils::makeNotNullShared<Following>(calendar);
	}

	DateAdjustmentConstPtr DateAdjustment::following(const CalendarProviderConstPtr& referenceData,
		const Calendar::Code& calendar) noexcept
	{
		return following(referenceData->getCalendar(calendar));
	}

	class Preceding final : public DateAdjustment
	{
	public:
		explicit Preceding(CalendarConstPtr calendar)
			: calendar_(std::move(calendar))
		{}

		[[nodiscard]] Date adjust(const Date& date) const override { return calendar_->previousOrSame(date); }
		[[nodiscard]] std::string getType() const override { return "Preceding"; }
	private:
		CalendarConstPtr calendar_;
	};

	DateAdjustmentConstPtr DateAdjustment::preceding(const CalendarConstPtr& calendar) noexcept
	{
		return utils::makeNotNullShared<Preceding>(calendar);
	}

	DateAdjustmentConstPtr DateAdjustment::preceding(const CalendarProviderConstPtr& referenceData,
		const Calendar::Code& calendar) noexcept
	{
		return preceding(referenceData->getCalendar(calendar));
	}

	class ModifiedFollowingBusinessDay final : public DateAdjustment
	{
	public:
		explicit ModifiedFollowingBusinessDay(CalendarConstPtr calendar)
			: calendar_(std::move(calendar))
		{}

		[[nodiscard]] Date adjust(const Date& date) const override
		{
			const auto following = calendar_->nextOrSame(date);
			return following.month() == date.month() ? following : calendar_->previous(date);
		}
		[[nodiscard]] std::string getType() const override { return "ModifiedFollowingBusinessDay"; }

	private:
		CalendarConstPtr calendar_;
	};

	DateAdjustmentConstPtr DateAdjustment::modifiedFollowing(const CalendarConstPtr& calendar) noexcept
	{
		return utils::makeNotNullShared<ModifiedFollowingBusinessDay>(calendar);
	}

	DateAdjustmentConstPtr DateAdjustment::modifiedFollowing(const CalendarProviderConstPtr& referenceData,
		const Calendar::Code& calendar) noexcept
	{
		return modifiedFollowing(referenceData->getCalendar(calendar));
	}

	class ModifiedPrecedingBusinessDay final : public DateAdjustment
	{
	public:
		explicit ModifiedPrecedingBusinessDay(CalendarConstPtr calendar)
			: calendar_(std::move(calendar))
		{}

		[[nodiscard]] Date adjust(const Date& date) const override
		{
			const auto preceding = calendar_->previousOrSame(date);
			return preceding.month() == date.month() ? preceding : calendar_->next(date);
		}
		[[nodiscard]] std::string getType() const override { return "ModifiedPrecedingBusinessDay"; }

	private:
		CalendarConstPtr calendar_;
	};

	DateAdjustmentConstPtr DateAdjustment::modifiedPreceding(const CalendarConstPtr& calendar) noexcept
	{
		return utils::makeNotNullShared<ModifiedPrecedingBusinessDay>(calendar);
	}

	DateAdjustmentConstPtr DateAdjustment::modifiedPreceding(const CalendarProviderConstPtr& referenceData,
		const Calendar::Code& calendar) noexcept
	{
		return modifiedPreceding(referenceData->getCalendar(calendar));
	}

	DateAdjustmentConstPtr DateAdjustment::fromConvention(const BusinessDayConvention& convention, const CalendarConstPtr& calendar)
	{
		switch (convention)
		{
		case BusinessDayConvention::None:
			return none();

		case BusinessDayConvention::P:
			return preceding(calendar);

		case BusinessDayConvention::MP:
			return modifiedPreceding(calendar);

		case BusinessDayConvention::F:
			return following(calendar);

		case BusinessDayConvention::MF:
			return modifiedFollowing(calendar);

		default:
			THROW(fmt::format("fromConvention does not support {} convention. Should not arrive here!", convention.toString()));
		}
	}

	DateAdjustmentConstPtr DateAdjustment::fromConvention(const BusinessDayConvention& convention,
		const CalendarProviderConstPtr& referenceData, const Calendar::Code& calendar)
	{
		return fromConvention(convention, referenceData->getCalendar(calendar));
	}

	class BusinessDaysAdjuster final : public DateAdjustment
	{
	public:
		explicit BusinessDaysAdjuster(const BusinessDays& lag, CalendarConstPtr calendar)
			: calendar_(std::move(calendar)), lag_(lag)
		{
		}

		[[nodiscard]] Date adjust(const Date& date) const override
		{
			return calendar_->shift(date, lag_);
		}

		[[nodiscard]] std::string getType() const override { return "BusinessDaysAdjuster"; }
	private:
		CalendarConstPtr calendar_;
		BusinessDays lag_;
	};

	DateAdjustmentConstPtr DateAdjustment::businessDaysLag(const BusinessDays& lag, const CalendarConstPtr& calendar) noexcept
	{
		return utils::makeNotNullShared<BusinessDaysAdjuster>(lag, calendar);
	}

	DateAdjustmentConstPtr DateAdjustment::businessDaysLag(const CalendarProviderConstPtr& referenceData,
		const BusinessDays& lag, const Calendar::Code& calendar) noexcept
	{
		return businessDaysLag(lag, referenceData->getCalendar(calendar));
	}

	DateAdjustmentConstPtr DateAdjustment::businessDaysLag(const CalendarProviderConstPtr& referenceData,
		const BusinessDaysLag& bDays) noexcept
	{
		return businessDaysLag(referenceData, bDays.lag, bDays.calendar);
	}

	class EomRuleAdjuster final : public DateAdjustment
	{
	public:
		explicit EomRuleAdjuster(CalendarConstPtr calendar)
			: calendar_(std::move(calendar))
		{
		}

		[[nodiscard]] Date adjust(const Date& date) const override
		{
			const auto ym = date.ym();
			const auto d = calendar_->nextOrSame(date);
			return calendar_->lastBusinessDay(ym) > d ? date : ym / last;
		}
		[[nodiscard]] std::string getType() const override { return "EomRuleAdjuster"; }

	private:
		CalendarConstPtr calendar_;
	};

	class DurationAdjuster final : public DateAdjustment
	{
	public:
		explicit DurationAdjuster(Duration lag, const bool eomRule)
			: lag_(std::move(lag)),
			eomRule_(eomRule)
		{
		}

		[[nodiscard]] Date adjust(const Date& date) const override
		{
			return date.add(lag_, eomRule_);
		}
		[[nodiscard]] std::string getType() const override { return "DurationAdjuster"; }

	private:
		Duration lag_;
		bool eomRule_;
	};

	DateAdjustmentConstPtr DateAdjustment::calendarDaysLag(const Days& days) noexcept
	{
		const Duration duration{ days };
		return utils::makeNotNullShared<DurationAdjuster>(duration, false);
	}

	DateAdjustmentConstPtr DateAdjustment::calendarMonthsLag(const Months& months, bool eomRule) noexcept
	{
		const Duration duration{ months };
		return utils::makeNotNullShared<DurationAdjuster>(duration, eomRule);
	}


	/**
	* An instance that adjusts the date to the next weekday (a Fx weekday is a non Sat, Sun or 1-Jan and essentially represents all days the FX market is quoting)
	* TODO Remove once calendars can support recurring holidays with rules!
	*/
	class NextFxWeekday final : public DateAdjustment
	{
	public:
		[[nodiscard]] Date adjust(const Date& date) const override
		{
			static auto cdr = Calendar::isoWeekday();
			auto d = cdr->next(date);
			const auto x = d.ymd();
			d = x.month() / x.day() == Jan / 1 ? cdr->next(d) : d;
			return d;
		}

		[[nodiscard]] std::string getType() const override { return "NextFxWeekday"; }


	};

	namespace
	{
		auto usdLegAdjustment(const CalendarProviderConstPtr& referenceData,
			const BusinessDays& lag, const Calendar::Code& usd, const bool isLatam)
		{
			static auto nextFx = utils::makeNotNullShared<const NextFxWeekday>();

			return isLatam || lag == 1_B
				? DateAdjustment::businessDaysLag(referenceData, lag, usd)
				: DateAdjustment::compose({ nextFx, DateAdjustment::businessDaysLag(referenceData, lag - 1_B, usd) });
		}

		auto ccyLag(const CalendarProviderConstPtr& referenceData, const CurrencyLag& ccy)
		{
			return DateAdjustment::businessDaysLag(referenceData, ccy.lag, ccy.calendar);
		}

	}

	DateAdjustmentConstPtr DateAdjustment::fxSpot(const CalendarProviderConstPtr& referenceData, const CurrencyLag& ccy, const Calendar::Code& usd)
	{
		const auto lag = ccy.lag;
		if (lag < 0_B || lag > 2_B) THROW(fmt::format("Cannot build a fx spot lag with {} valid values are 0B, 1B or 2B", lag));

		auto lastAdjustment = following(referenceData, ccy.calendar + usd);

		if (lag == 0_B) return lastAdjustment;

		const static auto weekday = following(Calendar::isoWeekday());
		const auto ccyAdj = ccyLag(referenceData, ccy);
		const auto usdAdj = usdLegAdjustment(referenceData, lag, usd, ccy.isLatam);

		return compose
		({
			weekday,  //next valid trade date (we can't trade on a weekend)
			max({ccyAdj, usdAdj}),
			lastAdjustment
			});
	}

	DateAdjustmentConstPtr DateAdjustment::fxSpot(const CalendarProviderConstPtr& referenceData,
		const CurrencyLag& left, const CurrencyLag& right, const Calendar::Code& usd)
	{
		const auto lag = std::max(left.lag, right.lag);
		if (lag < 0_B || lag > 2_B) THROW(fmt::format("Cannot build a fx spot lag with {} valid values are 0B, 1B or 2B", lag));

		const auto lastAdjustment = following(referenceData, left.calendar + right.calendar + usd);

		if (lag == 0_B) return lastAdjustment;

		const static auto weekday = following(Calendar::isoWeekday());
		const auto leftAdj = ccyLag(referenceData, left);
		const auto rightAdj = ccyLag(referenceData, right);
		const auto usdAdj = usdLegAdjustment(referenceData, lag, usd, left.isLatam || right.isLatam);

		return compose
		({
			weekday, //next valid trade date (we can't trade on a weekend)
			max({leftAdj, rightAdj, usdAdj}),
			lastAdjustment
			});
	}

	namespace
	{
		/**
		 * \brief Gets the adjustment for a regular fx swap period
		 * \param d the duration we want to add to spot date (1D, 2W, 3M...)
		 * \param combined the combined calendar used to adjust day to business day
		 */
		DateAdjustmentConstPtr fxDurationRule(const Duration& d, const DateAdjustmentConstPtr& spot,
			const CalendarConstPtr& combined)
		{
			const auto[ms, ds] = normalized(d);
			if (ds != 0_D && ms != 0_M)
			{
				return DateAdjustment::compose
				({
					spot,
					DateAdjustment::calendarMonthsLag(ms, false),
					DateAdjustment::calendarDaysLag(ds),
					DateAdjustment::following(combined)
					});
			}

			if (ds != 0_D)
			{
				return DateAdjustment::compose
				({
					spot,
					DateAdjustment::calendarDaysLag(ds),
					DateAdjustment::following(combined)
					});
			}

			if (ms != 0_M)
			{
				return DateAdjustment::compose
				({
					spot,
					utils::makeNotNullShared<EomRuleAdjuster>(combined), // if we are last business day of the month we roll to next end of month
					DateAdjustment::calendarMonthsLag(ms, true),
					DateAdjustment::modifiedFollowing(combined)
					});
			}

			return DateAdjustment::none();
		}
	}

	DateAdjustmentConstPtr DateAdjustment::fxForward(const CalendarProviderConstPtr& referenceData, const Tenor& t,
		const CurrencyLag& ccy, const Calendar::Code& usd)
	{
		const TP::Overloaded overloaded{
			[=](const Overnight&)
			{
				return fxSpot(referenceData, ccy.withLag(1_B), usd); // we do as if we had 1B spot
			},
			[=](const TomNext&)
			{
				const auto on = fxForward(referenceData, ON, ccy, usd);
				return fxDurationRule(1_D, on, referenceData->getCalendar(usd + ccy.calendar));
			},
			[=](const Spot&) { return fxSpot(referenceData, ccy, usd); },
			[=](const SpotNext&) { return fxForward(referenceData, 1_D, ccy, usd); }, // same as 1D
			[=](const Duration& d)
			{
				const auto fx = fxSpot(referenceData, ccy, usd);
				return fxDurationRule(d, fx, referenceData->getCalendar(usd + ccy.calendar));
			},
			[=](const MonthBusinessDay& md)
			{
				return compose(
					{
						fxSpot(referenceData, ccy, usd),
						md.adjustment(referenceData->getCalendar(usd + ccy.calendar))
					});
			}
		};

		return t.visit(overloaded);
	}

	DateAdjustmentConstPtr DateAdjustment::fxForward(const CalendarProviderConstPtr& referenceData, const Tenor& t,
		const CurrencyLag& left, const CurrencyLag& right, const Calendar::Code& usd)
	{
		const TP::Overloaded overloaded{
			[=](const Overnight&)
			{
				return fxSpot(referenceData, left.withLag(1_B), right.withLag(1_B), usd); // we do as if we had 1B spot
			},
			[=](const TomNext&)
			{
				const auto on = fxForward(referenceData, ON, left, right, usd);
				return fxDurationRule(1_D, on, referenceData->getCalendar(usd + left.calendar + right.calendar));
			},
			[=](const Spot&) { return fxSpot(referenceData, left, right, usd); },
			[=](const SpotNext&) { return fxForward(referenceData, 1_D, left, right, usd); }, // same as 1D
			[=](const Duration& d)
			{
				const auto spot = fxSpot(referenceData, left, right, usd);
				return fxDurationRule(d, spot, referenceData->getCalendar(usd + left.calendar + right.calendar));
			},
			[=](const MonthBusinessDay& md)
			{
				return compose(
					{
						fxSpot(referenceData, left, right, usd),
						md.adjustment(referenceData->getCalendar(usd + left.calendar + right.calendar))
					});
			}
		};

		return t.visit(overloaded);
	}

	std::pair<DateAdjustmentConstPtr, DateAdjustmentConstPtr> DateAdjustment::fxSwap(
		const CalendarProviderConstPtr& referenceData, const Tenor& t, const CurrencyLag& ccy, const Calendar::Code& usd)
	{
		const static auto tradeDateAdj = following(Calendar::isoWeekday()); // valid trade date
		const TP::Overloaded overloaded{
			[=](const Overnight&) { return tradeDateAdj; }, // just valid trade date
			[=](const TomNext&) { return fxForward(referenceData, ON, ccy, usd); },
			[=](const Spot&) { return tradeDateAdj; },
			[=](const auto&) { return fxSpot(referenceData, ccy, usd); } // all those are after spot date so short leg is spot
		};

		return { t.visit(overloaded), fxForward(referenceData, t, ccy, usd) };
	}

	std::pair<DateAdjustmentConstPtr, DateAdjustmentConstPtr> DateAdjustment::fxSwap(
		const CalendarProviderConstPtr& referenceData, const Tenor& t, const CurrencyLag& left,
		const CurrencyLag& right, const Calendar::Code& usd)
	{
		const static auto tradeDateAdj = following(Calendar::isoWeekday()); // valid trade date
		const TP::Overloaded overloaded{
			[=](const Overnight&) { return tradeDateAdj; }, // just valid trade date
			[=](const TomNext&) { return fxForward(referenceData, ON, left, right, usd); },
			[=](const Spot&) { return tradeDateAdj; },
			[=](const auto&) { return fxSpot(referenceData, left, right, usd); } // all those are after spot date so short leg is spot
		};

		return { t.visit(overloaded), fxForward(referenceData, t, left, right, usd) };
	}
}
