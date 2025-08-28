#include "Currency.h"
#include "TPTools/StringUtils/TPStringComparefunctional.h"
#include <fmt/format.h>
#include <map>

namespace TP
{
	namespace
	{
		std::map<int, Currency> ids()
		{
			std::map<int, Currency> idMap;
			for (auto c : currency::currencies)
			{
				idMap.insert(std::make_pair(c.id, c));
			}

			return idMap;
		}


	}

	OptCurrency Currency::maybeGet(const int id)
	{
		static const auto idMap = ids();
		const auto it = idMap.find(id);

		if (it != idMap.end())
			return it->second;

		return std::nullopt;
	}

	const Currency& Currency::operator=(const Currency& rhs)
	{
		if (this != &rhs)
		{
			code = rhs.code;
			id = rhs.id;
			minorUnit = rhs.minorUnit;
			onshore = rhs.onshore;
			official = rhs.official;
		}
		return *this;
	}

	//std::map< CCY_TYPE, Currency> Currency::tpCcytoCurrencyMap = {{CCY_TYPE::EUR, currency::EUR},
	//{CCY_TYPE::GBP, currency::GBP},
	//{CCY_TYPE::USD,currency::USD},
	//{CCY_TYPE::JPY,currency::JPY},
	//{CCY_TYPE::DKK,currency::DKK},
	//{CCY_TYPE::SEK,currency::SEK},
	//{CCY_TYPE::NOK,currency::NOK},
	//{CCY_TYPE::CHF,currency::CHF},
	//{CCY_TYPE::CAD,currency::CAD},
	//{CCY_TYPE::RUB,currency::RUB},
	//{CCY_TYPE::TND,currency::TND},
	//{CCY_TYPE::CZK,currency::CZK},
	//{CCY_TYPE::HUF,currency::HUF},
	//{CCY_TYPE::PLN,currency::PLN},
	//{CCY_TYPE::KGS,currency::KGS},
	//{CCY_TYPE::AUD,currency::AUD},
	//{CCY_TYPE::NZD,currency::NZD},
	//	};

	Currency::Currency() noexcept
		: Currency(currency::USD)
	{
	}

	//Currency Currency::fromTPCcy(const TPCcy& tpccy)
	//{
	//	return Currency(tpCcytoCurrencyMap[tpccy.getCcyType()]);
	//}

	Currency::Currency(const int id)
		: Currency(maybeGet(id).value_or(currency::NONE))
	{
		if (id == 0)
		{
			std::string error = fmt::format("Cannot find currency, id {} is not in the ISO 4217 list 	See https ://fr.wikipedia.org/wiki/ISO_4217 for more details", id);
			throw std::invalid_argument(error);
		}
	}

	Currency::Currency(const std::string_view code)
		: Currency(maybeGet(code).value_or(currency::NONE))
	{
		if (id == 0)
		{
			std::string error = fmt::format("Cannot find currency, {} is not in the ISO 4217 list. See https://fr.wikipedia.org/wiki/ISO_4217 for more details", code);
			throw std::invalid_argument(error);
		}
	}

/*
	Currency::Currency(const int id)
		: Currency(maybeGet(id).valueOrThrow<std::invalid_argument>(
			fmt::format("Cannot find currency, id {} is not in the ISO 4217 list. See https://fr.wikipedia.org/wiki/ISO_4217 for more details", id)))
	{}
	Currency::Currency(const std::string_view code)
		: Currency(maybeGet(code).valueOrThrow<std::invalid_argument>(
			fmt::format("Cannot find currency, {} is not in the ISO 4217 list. See https://fr.wikipedia.org/wiki/ISO_4217 for more details", code)))

	{	}
	*/
	namespace
	{
		using namespace Text;
		auto codes()
		{
			//std::map<std::string_view, Currency, ILess> codeMap;
			std::map<std::string_view, Currency> codeMap;
			for (auto c : currency::currencies)
			{
				codeMap.insert(std::make_pair(c.code, c));
			}

			return codeMap;
		}
	}

	OptCurrency Currency::maybeGet(const std::string_view code)
	{
		static const auto codeMap = codes();
		const auto it = codeMap.find(code);

		if (it != codeMap.end())
			return it->second;

		return std::nullopt;
	}

	[[nodiscard]] std::string Currency::toString() const
	{
		return std::string(code);
	}


	namespace formatting
	{
		

		OptCurrency  Formatter<Currency>::tryParse(const std::string_view& str) const
		{
			static const auto codemap = codes();
			auto it = codemap.find(str);
			if (it == codemap.end()) return std::nullopt;
			return it->second;
		}

		std::string Formatter<Currency>::str(const Currency& value) const
		{
			return std::string(value.code);
		}

		std::string Formatter<Currency>::failToParse(const std::string_view& str) const
		{
			return fmt::format("Cannot find currency {} in the list", str);
		}
	}
}