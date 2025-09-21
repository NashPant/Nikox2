#ifndef TP_GENERATOR_H_123
#define TP_GENERATOR_H_123

#include <TPNikopolisUtils.h>
#include <memory>
#include <string>
#include "TPTools/Utils/enum_class.h"

namespace TP {
	namespace templates
	{
		//using date_time::IRIMMDateConvention;

		enum class TemplateInstrumentType_e
		{
			Bond = 0,
			Currency = 1,
			CapFloor = 2,
			FRA = 3,
			Future = 4,
			FxForward = 5,
			FxSpot = 6,
			Swap = 7,
			SwapLeg = 8,
			MoneyMarket = 9,
			Option = 10,
			SpreadOption = 11,
			DatesGenerator = 12,
			Other = 13
		};


		class Generator;
		using GeneratorPtr = std::shared_ptr<Generator>;
		using  GeneratorConstPtr = std::shared_ptr<const Generator>;

		class Generator
		{

		public:
			virtual ~Generator() = default;
			const std::string& templateId() const { return templateId_; }

			virtual const TemplateInstrumentType_e& type() const = 0;

		protected:
			explicit Generator(std::string templateid):templateId_(std::move(templateid))
			{}
		private:
			std::string templateId_;
		};
	}
}

#endif
