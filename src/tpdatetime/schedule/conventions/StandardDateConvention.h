#pragma once
#include "DateConvention.h"
#include "TPDateTime/Date.h"
#include "TPDateTime/financial/BusinessDayConvention.h"
#include <memory>
#include <string>

namespace TP::date_time
{
	class StandardDateConvention;
	using StandardDateConventionConstPtr = std::shared_ptr<const StandardDateConvention>;

	class TPNIKOPOLIS_IMPEXP StandardDateConvention : public DateConvention
	{
	public:
		//enum class RollConvention { NONE, F, P, MF, MP };

		static Date rollDate(
			const Date& date,
			const CalendarConstPtr& calendar,
			const BusinessDayConvention& rollConvention,
			unsigned int businessDayLag
		);

		StandardDateConvention(
			const CalendarConstPtr calendar,
			const BusinessDayConvention rollConvention,
			const unsigned int businessDayLag
		) :
			DateConvention(calendar, businessDayLag),
			rollConvention_{ rollConvention }
		{}

		StandardDateConvention(
			const std::shared_ptr<const Calendar>& calendar,
			const std::string& rollConvention_str,
			const unsigned int businessDayLag
		) :
			DateConvention(CalendarConstPtr(calendar), businessDayLag),
			rollConvention_{ BusinessDayConvention(rollConvention_str) }
		{}


		StandardDateConvention(const StandardDateConvention& other) = default;
		StandardDateConvention& operator=(const StandardDateConvention& other) = default;

		BusinessDayConvention rollConvention() const { return rollConvention_; }
		Date rollDate(const Date& date) const override { return rollDate(date, calendar_, rollConvention_, businessDayLag_); }

		static StandardDateConventionConstPtr createStandardOptionConvention();
		// hashing function
		size_t hash() const;
		// attributes
		std::map<std::string, TPAny> attributes(const std::optional<std::string> prefix = std::nullopt) const override;
	protected:
		BusinessDayConvention rollConvention_;
	};

	using TenorConvention = StandardDateConvention;

	inline size_t StandardDateConvention::hash() const
	{
		size_t sizet = size_t(businessDayLag()) * 10;
		sizet ^= static_cast<size_t>(rollConvention_) * 1;
		std::hash<std::string> hasher;
		sizet ^= hasher(calendar()->code().underlying());
		return sizet;
	}
}