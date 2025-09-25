#include "TPFrequency.h"
#include "TPDateTime/Frequency.h"
#include <boost/algorithm/string.hpp>

namespace TP
{

	namespace Core
	{

		class TPFrequency::Impl
		{
		public:
			Impl& operator=(const Impl&) = delete;
			virtual ~Impl() = default;
			virtual std::string toString() const = 0;
			virtual TPTenor toTenor() const = 0;
			virtual bool isBullet() const = 0;
		};

		/////////////////////////////////////////////////////////////////

		class ImplFrequencyByTenor final : public TPFrequency::Impl
		{
		public:
			explicit ImplFrequencyByTenor(const TPTenor& tenor) : tenor_(tenor) {}
			ImplFrequencyByTenor(const ImplFrequencyByTenor&) = delete;
			ImplFrequencyByTenor& operator=(const ImplFrequencyByTenor&) = delete;
			virtual ~ImplFrequencyByTenor() = default;

			std::string toString() const override { return tenor_.toString(); }
			TPTenor toTenor() const override { return tenor_; }
			bool isBullet() const override { return false; }
		private:
			const TPTenor tenor_;
		};

		///////////////////////////////////////////////////////////////////

		class Bullet final : public TPFrequency::Impl
		{
		public:
			Bullet() = default;
			Bullet& operator=(const Bullet&) = delete;
			virtual ~Bullet() = default;

			std::string toString() const override { return "BULLET"; }
			TPTenor toTenor() const override;
			bool isBullet() const override { return true; }
		};

		TPTenor Bullet::toTenor() const
		{
			THROW("Can't convert a bullet frequency to a tenor.");
		}

		////////////////////////////////////////////////////////////////////

		TPFrequency::TPFrequency(const TPTenor& tenor)
			: impl_(std::make_shared<const ImplFrequencyByTenor>(tenor))
		{
		}

		TPFrequency::TPFrequency(int multiplier, PERIOD period)
			: impl_(std::make_shared<const ImplFrequencyByTenor>(TPTenor(multiplier, period)))
		{
		}

		TPFrequency::TPFrequency(const date_time::Frequency& f): TPFrequency(12/f.count(), PERIOD::M)
		{
		}
		TPFrequency TPFrequency::bullet()
		{
			return TPFrequency(std::make_shared<const Bullet>());
		}

		bool TPFrequency::empty() const
		{
			return !impl_;
		}

		std::string TPFrequency::toString() const
		{
			if (empty())
				THROW_MISSING_VALUE_EXCEPTION("The frequency has not been initialized.");
			return impl_->toString();
		}

		TPTenor TPFrequency::toTenor() const
		{
			if (empty())
				THROW_MISSING_VALUE_EXCEPTION("The frequency has not been initialized.");
			return impl_->toTenor();
		}

		bool TPFrequency::isBullet() const
		{
			if (empty())
				THROW_MISSING_VALUE_EXCEPTION("The frequency has not been initialized.");
			return impl_->isBullet();
		}

		TPFrequency::TPFrequency(const std::shared_ptr<const Impl>& impl)
			: impl_(impl)
		{
		}

		double operator/(double num, const TPFrequency& den)
		{
			if (den.isBullet())
				THROW("Can't divide a double by a [BULLET] frequency.");

			return num / static_cast<int>(den.toEnum());
		}

		TPFrequency TPFrequency::fromString(const std::string& s)
		{
			TPFrequency f;

			auto sCopy = s;
			boost::algorithm::to_upper(sCopy);
			if(sCopy == "BULLET")
			{
				return TPFrequency::bullet();
			}
			else
			{
				TPTenor tenor(sCopy);
				return TPFrequency(tenor);
			}
			//auto f = AletheiaInterface::Message::fromStringToFrequency(s);
			//return Converter::InterfaceConverter::convert(f);
		}

		FREQUENCY TPFrequency::toEnum() const
		{
			
			if (isBullet())
				return FREQUENCY::ONCE;

			const auto tenor = toTenor();

			if (tenor == TPTenor(1, PERIOD::Y) || tenor == TPTenor(12, PERIOD::M))
				return FREQUENCY::ANNUAL;
			if (tenor == TPTenor(6, PERIOD::M))
				return FREQUENCY::SEMI_ANNUAL;
			if (tenor == TPTenor(3, PERIOD::M))
				return FREQUENCY::QUARTERLY;
			if (tenor == TPTenor(2, PERIOD::M))
				return FREQUENCY::BIMONTHLY;
			if (tenor == TPTenor(1, PERIOD::M))
				return FREQUENCY::MONTHLY;
			if (tenor == TPTenor(2, PERIOD::W))
				return FREQUENCY::BIWEEKLY;
			if (tenor == TPTenor(1, PERIOD::W))
				return FREQUENCY::WEEKLY;

			THROW("Can not convert tenor [" + tenor.toString() + "] to a frequency enum.");
		}

		TPFrequency TPFrequency::fromEnum(const FREQUENCY& f)
		{
			switch (f)
			{
			case FREQUENCY::ONCE: return bullet();
			case FREQUENCY::ANNUAL: return TPFrequency(1, PERIOD::Y);
			case FREQUENCY::SEMI_ANNUAL: return TPFrequency(6, PERIOD::M);
			case FREQUENCY::EVERY_FOURTH_MONTH: return TPFrequency(4, PERIOD::M);
			case FREQUENCY::QUARTERLY: return TPFrequency(3, PERIOD::M);
			case FREQUENCY::BIMONTHLY: return TPFrequency(2, PERIOD::M);
			case FREQUENCY::MONTHLY: return TPFrequency(1, PERIOD::M);
			case FREQUENCY::EVERY_FOURTH_WEEK: return TPFrequency(4, PERIOD::W);
			case FREQUENCY::BIWEEKLY: return TPFrequency(2, PERIOD::W);
			case FREQUENCY::WEEKLY: return TPFrequency(1, PERIOD::W);
			case FREQUENCY::DAILY: return TPFrequency(1, PERIOD::D);
			default:
			{
				std::string fstr = TP::Core::toString<FREQUENCY>(f);
				THROW_INVALID_INPUT_EXCEPTION("Can't convert enum [" + fstr + "] into a specific frequency.");
			}
			}
		}

		template <class Archive> std::string TPFrequency::save_minimal(Archive const &) const
		{
			return toString();
		}

		template <class Archive> void TPFrequency::load_minimal(Archive const &, std::string const & value)
		{
			*this = fromString(value);
		}

	}
}