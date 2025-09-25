#ifndef TP_CORE_FREQUENCY_H
#define TP_CORE_FREQUENCY_H

#pragma once


#include <TPNikopolisUtils.h>
#include <string>
#include <boost/optional.hpp>
#include "TPTenor.h"
#include <memory>
#include "TPTools\Utils\Enum_Class.h"

namespace TP::date_time
{
	class Frequency;
}

namespace TP {
	namespace Core {

		enum class FREQUENCY
		{
			ONCE = 0,
			// only once, e.g., a zero-coup (aka bullet)
			ANNUAL = 1,
			// once a year
			SEMI_ANNUAL = 2,
			// twice a year
			EVERY_FOURTH_MONTH = 3,
			// every fourth month
			QUARTERLY = 4,
			// every third month
			BIMONTHLY = 6,
			// every second month
			MONTHLY = 12,
			// once a month
			EVERY_FOURTH_WEEK = 13,
			// every fourth week
			BIWEEKLY = 26,
			// every second week
			WEEKLY = 52,
			// once a week
			DAILY = 365,
			// once a day
		};

		class TPNIKOPOLIS_IMPEXP TPFrequency final
		{
		public:
			TPFrequency() {}
			explicit TPFrequency(const TPTenor& TPTenor);
			explicit TPFrequency(int multiplier, PERIOD period);
			TPFrequency(const date_time::Frequency& f);
			static TPFrequency bullet();
			static TPFrequency fromString(const std::string& s);
			static TPFrequency fromEnum(const FREQUENCY& f);

			~TPFrequency() = default;

			bool empty() const; //Returns whether or not the frequency is initialized
			std::string toString() const;
			TPTenor toTenor() const;
			FREQUENCY toEnum() const;
			bool isBullet() const;

			template <class Archive> std::string save_minimal(Archive const &) const;
			template <class Archive> void load_minimal(Archive const &, std::string const & value);
			class Impl;
		private:
			std::shared_ptr<const Impl> impl_;
			explicit TPFrequency(const std::shared_ptr<const Impl>& impl);
		};

		TPNIKOPOLIS_IMPEXP double operator/(double num, const TPFrequency& den);
	}

	
	template <>
	//TPNIKOPOLIS_IMPEXP
	const TPEnum<Core::FREQUENCY>::SymbolVector TPEnum<Core::FREQUENCY>::symbolVector_ = {
		{ "ONCE", Core::FREQUENCY::ONCE },
		{ "ANNUAL", Core::FREQUENCY::ANNUAL },
		{ "SEMI_ANNUAL", Core::FREQUENCY::SEMI_ANNUAL },
		{ "EVERY_FOURTH_MONTH", Core::FREQUENCY::EVERY_FOURTH_MONTH },
		{ "QUARTERLY", Core::FREQUENCY::QUARTERLY },
		{ "BIMONTHLY", Core::FREQUENCY::BIMONTHLY },
		{ "EVERY_FOURTH_WEEK", Core::FREQUENCY::EVERY_FOURTH_WEEK },
		{ "BIWEEKLY", Core::FREQUENCY::BIWEEKLY },
		{ "WEEKLY", Core::FREQUENCY::WEEKLY },
		{ "DAILY", Core::FREQUENCY::DAILY },
	};
}

#endif