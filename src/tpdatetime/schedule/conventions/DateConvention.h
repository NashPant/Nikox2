
#ifndef TP_DATECONVENTION_H_138_
#define TP_DATECONVENTION_H_138_

#include <TPNikopolisUtils.h>
#include <string>
#include <map>
#include "TPDateTime\Calendar.h"
//#include "TPTools\time\TPDate.h"
//#include "TPTools\TPAny.h"
#include "tptools/serialisation/include.h"


namespace TP {
	namespace date_time {

		class DateConvention;
		using DateConventionConstPtr = std::shared_ptr<const DateConvention>;
		using DateConventionPtr = std::shared_ptr<DateConvention>;

		class TPNIKOPOLIS_IMPEXP DateConvention
		{
		public:
			DateConvention(
				const CalendarConstPtr& calendar,
				const unsigned int businessDayLag
			) :
				calendar_(calendar),
				businessDayLag_{ businessDayLag }
			{}

			DateConvention(
				const std::string& calendar_str,
				const unsigned int businessDayLag
			);

			virtual ~DateConvention() = default;

			DateConvention(const DateConvention& other) = default;
			DateConvention& operator=(const DateConvention& other) = default;

			CalendarConstPtr calendar() const { return calendar_; }
			unsigned int businessDayLag() const { return businessDayLag_; }

			virtual Date rollDate(const Date& date) const = 0;

			//virtual std::map<std::string, TPAny> attributes(const std::optional<std::string> prefix = std::nullopt) const;

		protected:
			CalendarConstPtr calendar_;
			unsigned int businessDayLag_;

			PROPERTIES(DateConvention, calendar_, businessDayLag_);
		};

	}
}

#endif