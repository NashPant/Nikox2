#include "CurrencyPair.h"
//#include <boost/container_hash/hash.hpp>
#include "TPTools/Utils/hash_replace.h"
#include <fmt/format.h>
#include "TPTools/Utils/Formatting.h"

using namespace TP::formatting;

namespace TP
{
	namespace
	{
		std::string makeFmtFormat(const std::string_view fmt)
		{
			std::string format;
			format.reserve(fmt.size());
			auto isCmd = false;
			for (auto c : fmt)
			{
				switch (c)
				{
				case '%':
					isCmd = true; break;
				case 'f':
				case 'b':
				case 'l': // left currency
					if (isCmd)
					{
						format += "{0}";
						isCmd = false;
					}
					else
					{
						format += c;
					}
					break;
				case 'd':
				case 'q':
				case 'r': // right currency
					if (isCmd)
					{
						format += "{1}";
						isCmd = false;
					}
					else
					{
						format += c;
					}
					break;
				default:
					if (isCmd)
						throw std::invalid_argument(fmt::format("unsupported format {} for currency pair", fmt));
					format += c;

				}
			}

			return format;
		}
	}

	const std::string_view CurrencyPair::defaultFormat = "%f%d";

	CurrencyPair::CurrencyPair(std::string_view str, std::string_view format) :
		CurrencyPair(TP::parse<CurrencyPair>(str, format))
	{}

	CurrencyPair CurrencyPair::createCurrencyPair(const Currency& domccy, const Currency& forccy)
	{
		return CurrencyPair(domccy, forccy);
	}


	OptCurrency CurrencyPair::noMatching(const Currency& in) const
	{
		return left == in ? OptCurrency(right) : right == in ? OptCurrency(left) : std::nullopt;
	}

	bool CurrencyPair::contains(const Currency& in) const
	{
		return left == in || right == in;
	}
	std::string CurrencyPair::toString() const
	{
		return fmt::format("{}/{}", left.toString(), right.toString());
		//return std::string("test");
	}
}


namespace TP::formatting
{
	Formatter<CurrencyPair>::Formatter(std::string_view fmtOverride)
		: format_(std::move(fmtOverride))
	{
	}

	OptCurrencyPair Formatter<CurrencyPair>::tryParse(const std::string_view& str) const
	{
		std::string_view left, right;
		auto isCmd = false;
		auto it = 0;
		for (auto c : format_)
		{
			switch (c)
			{
			case '%':
				if (isCmd) return std::nullopt;
				isCmd = true; break;
			case 'f':
			case 'b':
			case 'l': // left currency
				if (isCmd)
				{
					const auto begin = it;
					it += 3;
					left = str.substr(begin, it);
					isCmd = false;
				}
				else
				{
					++it;
				}
				break;
			case 'd':
			case 'q':
			case 'r': // right currency
				if (isCmd)
				{
					const auto begin = it;
					it += 3;
					right = str.substr(begin, it);
					isCmd = false;
				}
				else
				{
					++it;
				}
				break;
			default:
				if (isCmd) return std::nullopt;
				++it;
			}
		}

		static const Formatter<Currency> ccyFmt;
		auto ll = ccyFmt.tryParse(left);
		auto rr = ccyFmt.tryParse(right);
		if(ll.has_value() && rr.has_value())
			return CurrencyPair(ll.value(), rr.value());
		else
			return std::nullopt;

		
	}

	std::string Formatter<CurrencyPair>::str(const CurrencyPair& value) const
	{
		return fmt::format("{}/{}", value.left.toString(), value.right.toString());

	}

	std::string Formatter<CurrencyPair>::failToParse(const std::string_view& str) const
	{
		return TP::failToParse<CurrencyPair>(str, __FILE__, __LINE__);
	}
}


namespace std
{
	size_t hash<TP::CurrencyPair>::operator()(const TP::CurrencyPair& in) const noexcept
	{
		size_t h = 0U;
		TP::hash_combine(h, in.left.id);
		TP::hash_combine(h, in.right.id);
		return h;
	}
}