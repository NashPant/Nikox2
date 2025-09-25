#include "TPTenor.h"
#include <boost/functional/hash.hpp>
#include <boost/algorithm/string/regex.hpp>
#include "../TPException.h"

#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/case_conv.hpp>

namespace TP {

        namespace Core {



            std::string ToString(const PERIOD& tenor)
            {
                std::string ret;

                switch (tenor)
                {
                case  PERIOD::ON: ret = "N"; break;
                case   PERIOD::TN: ret = "ON"; break;
                case  PERIOD::D: ret = "D"; break;
                case  PERIOD::BD: ret = "BD"; break;
                case  PERIOD::W: ret = "W"; break;
                case  PERIOD::M: ret = "M"; break;
                case  PERIOD::Y: ret = "Y"; break;
                }

                return ret;
            }

			const OptionalTenor& TPTenor::null()
			{
				static const OptionalTenor STATIC_INSTANCE;
				return STATIC_INSTANCE;
			}

			const TPTenor& TPTenor::tenorON()
			{
				static const TPTenor TENOR{ 1,PERIOD::ON };
				return TENOR;
			}


			void TPTenor::init()
			{
				switch (period_)
				{
				case PERIOD::ON:
				case PERIOD::TN:
				case PERIOD::D:
					yearCount_ = 0;
					monthCount_ = 0;
					weekCount_ = 0;
					dayCount_ = multiplier_;
					break;
				case PERIOD::W:
					yearCount_ = 0;
					monthCount_ = 0;
					weekCount_ = multiplier_;
					dayCount_ = 0;
					break;
				case PERIOD::M:
					// check if divisible by 12. then we convert it into years
					if (multiplier_ % 12 == 0)
					{
						multiplier_ = multiplier_ / 12;
						yearCount_ = multiplier_;
						monthCount_ = 0;
						weekCount_ = 0;
						dayCount_ = 0;
						period_ = PERIOD::Y;
					}
					else
					{
						yearCount_ = 0;
						monthCount_ = multiplier_;
						weekCount_ = 0;
						dayCount_ = 0;
					}
					break;
				case PERIOD::Y:
					yearCount_ = multiplier_;
					monthCount_ = 0;
					weekCount_ = 0;
					dayCount_ = 0;
					break;

				}
			}

			size_t TPTenor::hashValue() const
			{
				size_t seed = 0x712C985B;
				boost::hash_combine(seed, yearCount_ * 12 + monthCount_);
				boost::hash_combine(seed, weekCount_ * 7 + dayCount_);
				return seed;
			}

            std::pair<int, int> daysMinMax(const TPTenor& t)
            {
                //using namespace AletheiaInterface::Enum;

                switch (t.getPeriod())
                {
                case PERIOD::ON:
                case PERIOD::TN:
                case PERIOD::D:
                    return std::make_pair(t.getMultiplier(), t.getMultiplier());
                case PERIOD::W:
                    return std::make_pair(7 * t.getMultiplier(), 7 * t.getMultiplier());
                case PERIOD::M:
                    return std::make_pair(28 * t.getMultiplier(), 31 * t.getMultiplier());
                case PERIOD::Y:
                    return std::make_pair(365 * t.getMultiplier(), 366 * t.getMultiplier());
                default:
                    THROW ("Cannot convert [" + ToString(t.getPeriod()) + "]-based tenor into days.");
                }
            }

            TPTenor::TPTenor(int multiplier, PERIOD period)
                : multiplier_(multiplier), period_(period)
            {
                if (period == PERIOD::ON || period == PERIOD::TN)
                {
                    if (!(multiplier == 1 || multiplier == -1))
                     THROW ("ON and TN tenors should have a unitized multiplier. Either 1 or -1.");
                }
				init();
            }

            bool TPTenor::inMonths() const
            {
                //using namespace AletheiaInterface::Enum;

                switch (period_)
                {
                case PERIOD::M:
                    return true;
                default:
                    return false;
                }
            }

            bool TPTenor::inYears() const
            {
               

                switch (period_)
                {
                case PERIOD::Y:
                    return true;
                default:
                    return false;
                }
            }

            bool TPTenor::isBusinessPeriod() const
            {
               

                switch (period_)
                {
                case PERIOD::BD:
                    return true;
                default:
                    return false;
                }
            }

            bool TPTenor::isONTenor() const
            {
                return multiplier_ == 1 && period_ == PERIOD::ON;
            }

            bool TPTenor::is0BDTenor() const
            {
                return multiplier_ == 0 && period_ == PERIOD::BD;
            }

            bool TPTenor::is1BDTenor() const
            {
                return multiplier_ == 1 && period_ == PERIOD::BD;
            }

            bool TPTenor::is2BDTenor() const
            {
                return multiplier_ == 2 && period_ == PERIOD::BD;
            }

            TPTenor TPTenor::operator-() const
            {
                return TPTenor(-multiplier_, period_);
            }

            bool TPTenor::operator==(const TPTenor& other) const
            {
                return multiplier_ == other.multiplier_ && period_ == other.period_ ? true : false;
            }


            bool TPTenor::operator<(const TPTenor& other) const
            {
                

                // special cases
                if (getMultiplier() == 0)
                    return other.getMultiplier() > 0;
                if (other.getMultiplier() == 0)
                    return getMultiplier() < 0;

                // exact comparisons
                if (getPeriod() == other.getPeriod())
                    return getMultiplier() < other.getMultiplier();

                if (getPeriod() == PERIOD::Y)
                {
                    if (other.getPeriod() == PERIOD::M)
                        return 12 * getMultiplier() < other.getMultiplier();
                    if (other.getPeriod() == PERIOD::W)
                        return 52 * getMultiplier() < other.getMultiplier();//not exact
                }

                if (getPeriod() == PERIOD::M)
                {
                    if (other.getPeriod() == PERIOD::Y)
                        return getMultiplier() < 12 * other.getMultiplier();
                }

                if (getPeriod() == PERIOD::W)
                {
                    if (other.getPeriod() == PERIOD::D)
                        return 7 * getMultiplier() < other.getMultiplier();
                }

                if (getPeriod() == PERIOD::D)
                {
                    if (other.getPeriod() == PERIOD::W)
                        return getMultiplier() < 7 * other.getMultiplier();
                }

                // inexact comparisons (handled by converting to days and using limits)
                auto p1lim = daysMinMax(*this);
                auto p2lim = daysMinMax(other);

                if (p1lim.second < p2lim.first)
                    return true;
                if (p1lim.first > p2lim.second)
                    return false;

               THROW_INVALID_OPERATION_EXCEPTION("Undecidable comparison between tenor [" + this->toString() + "] and tenor [" + other.toString() + "].");
            }

            bool TPTenor::operator!=(const TPTenor& other) const
            {
                return multiplier_ == other.multiplier_ && period_ == other.period_ ? false : true;
            }

            std::string TPTenor::toString() const
            {
                const auto period = getPeriod();
                const auto mult = getMultiplier();
                const std::string periodStr = ToString(period);
                if (mult == 1 && (period == PERIOD::ON || period == PERIOD::TN))
                    return periodStr;
                else
                    return std::to_string(mult) + periodStr;
            }

			TPTenor::TPTenor(const std::string& in)
			{
				*this = fromString(in);
				init();
			}
            TPTenor TPTenor::fromString(const std::string& tenorSz)
           {
			auto s = tenorSz;
			boost::algorithm::to_upper(s);

			if (s == "ON")
			{
				TPTenor t;
				t.multiplier_ = 1;
				t.period_ = PERIOD::ON;
				return t;
			}

			if (s == "TN")
			{
				TPTenor t;
				t.multiplier_ = 1;
				t.period_ = PERIOD::TN;
				return t;
			}

			std::vector<std::string> splitVec;

			if (boost::ends_with(s, "BD"))//this must be checked before "D"!
			{
				TPTenor t;
				split_regex(splitVec, s, boost::regex("BD"));
				t.multiplier_ = stoi(splitVec[0]);
				t.period_ = PERIOD::BD;
				return t;
			}

			if (boost::ends_with(s, "D"))
			{
				TPTenor t;
				split_regex(splitVec, s, boost::regex("D"));
				t.multiplier_ = stoi(splitVec[0]);
				t.period_ = PERIOD::D;
				return t;
			}

			if (boost::ends_with(s, "W"))
			{
				TPTenor t;
				split_regex(splitVec, s, boost::regex("W"));
				t.multiplier_ = stoi(splitVec[0]);
				t.period_ = PERIOD::W;
				return t;
			}

			if (boost::ends_with(s, "M"))
			{
				TPTenor t;
				split_regex(splitVec, s, boost::regex("M"));
				t.multiplier_ = stoi(splitVec[0]);
				t.period_ = PERIOD::M;
				return t;
			}

			if (boost::ends_with(s, "Y"))
			{
				TPTenor t;
				split_regex(splitVec, s, boost::regex("Y"));
				t.multiplier_ = stoi(splitVec[0]);
				t.period_ = PERIOD::Y;
				return t;
			}

			THROW_INVALID_INPUT_EXCEPTION("Can't parse a tenor from string [" + s + "].");
      //          auto t = AletheiaInterface::Message::fromStringToTenor(s);
        //return TPTenor(t.multiplier_.get(), t.period_.get());
        }

            TPTenor operator*(const TPTenor& tenor, int mult)
            {
                return TPTenor(mult * tenor.getMultiplier(), tenor.getPeriod());
            }
            TPTenor operator*(int mult, const TPTenor& tenor)
            {
                return TPTenor(mult * tenor.getMultiplier(), tenor.getPeriod());
            }

            std::ostream& operator<<(std::ostream& out, const TPTenor& tenor)
            {
                out << tenor.toString();
                return out;
            }

            template <class Archive> std::string TPTenor::save_minimal(Archive const &) const
            {
                return toString();
            }

            template <class Archive> void TPTenor::load_minimal(Archive const &, std::string const & value)
            {
                *this = fromString(value);
            }
        }
}
//#include <TPTools\Serialisation\SerialMacros.h>
//NIKO_SERIALIZATION_IMPL_MINIMAL(TP::Core::TPTenor, std::string, TPNIKOPOLIS_IMPEXP);