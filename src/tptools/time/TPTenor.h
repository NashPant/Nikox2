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

#ifndef TP_TENOR_H_38406848340384083_
#define TP_TENOR_H_38406848340384083_

#include <TPNikopolisUtils.h>
#include <string>
//#include <boost/optional.hpp>
#include <optional>

namespace TP {
    namespace Core {

        enum class PERIOD
        {
            ON = 0,
            TN = 1,
            D = 2,
            BD = 3,
            W = 7,
            M = 30,
            Y = 360
        };

		class TPTenor;
		using OptionalTenor = std::optional<TPTenor>;
		//using Tenor = TPTenor;

        class TPNIKOPOLIS_IMPEXP TPTenor final
        {
        public:
			static const OptionalTenor& null();
			static const TPTenor& tenorON();

			TPTenor() : multiplier_(0), period_(PERIOD::D) { init(); }
            TPTenor(int multiplier, PERIOD period);

			explicit TPTenor(const std::string& in);

            static TPTenor ON() { return TPTenor(1, PERIOD::ON); }
            static TPTenor TN() { return TPTenor(1, PERIOD::TN); }
            static TPTenor fromString(const std::string& s);

			unsigned yearCount() const { return yearCount_; }
			unsigned monthCount() const { return monthCount_; }
			unsigned weekCount() const { return weekCount_; }
			unsigned dayCount() const{ return dayCount_; }

			bool isByMonth() const { return weekCount() == 0U && dayCount() == 0U; }
			bool isByDay() const { return monthCount() == 0U && yearCount() == 0U; }
			bool isZeroLength() const
			{
				return yearCount() == 0U && monthCount() == 0U && weekCount() == 0U && dayCount() == 0U;
			}

            int getMultiplier() const { return multiplier_; }
            PERIOD getPeriod() const { return period_; }
            bool inMonths() const;
            bool inYears() const;
            bool isBusinessPeriod() const;
            bool isONTenor() const;
            bool is0BDTenor() const;
            bool is1BDTenor() const;
            bool is2BDTenor() const;
            TPTenor operator-() const;
            bool operator==(const TPTenor& other) const;
            bool operator<(const TPTenor& other) const;
            bool operator!=(const TPTenor& other) const;

            std::string toString() const;
			std::string operator+(const std::string& s) const { return toString() + s; }

			size_t hashValue() const;
            template <class Archive> std::string save_minimal(Archive const &) const;
            template <class Archive> void load_minimal(Archive const &, std::string const & value);
        private:
			// rough yf
			double rough_yf() const;

            int multiplier_;
            PERIOD period_;
			// internal variables to keep count of what it is
			unsigned yearCount_, monthCount_, weekCount_, dayCount_;
			// assign multiplier to the correct variables
			void init();
        };

        TPNIKOPOLIS_IMPEXP TPTenor operator*(const TPTenor& tenor, int mult);
        TPNIKOPOLIS_IMPEXP TPTenor operator*(int mult, const TPTenor& tenor);
        TPNIKOPOLIS_IMPEXP std::ostream& operator<<(std::ostream&, const TPTenor& period);

        TPNIKOPOLIS_IMPEXP std::string	ToString(const PERIOD&);

    }    

}

namespace std
{
	using namespace TP::Core;
	template<>
	struct hash<TPTenor>
	{
		size_t operator() (const TPTenor& tenor) const
		{
			return tenor.hashValue();
		}
	};
}


#endif
