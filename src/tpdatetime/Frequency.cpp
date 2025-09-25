#include "Frequency.h"

#include "Date.h"
//#include "common/CharConv.h"
#include "TPTools/StringUtils/stringtools.h"
#include "TPTools/utils/TPEnum.h"
#include "TPTools/Time/TPFrequency.h"

#include <ctype.h>

using namespace TP::date_time;
using namespace TP::formatting;

Frequency::Frequency(const std::string_view& freqStr, const TP::Optional<std::string>& fmt)
	: rep_(TP::parse<Frequency>(freqStr, fmt).count())
{
}

Frequency::Frequency(const Duration& duration)
	: rep_(tryMake(duration).valueOrThrow<std::invalid_argument>(fmt::format("Cannot create frequency: 1Y is not divisible by {} periods", duration)).count())
{
}

Frequency::Frequency(const TP::Core::TPFrequency& tpfreq): Frequency(tpfreq.toString())
{
}

OptFrequency Frequency::tryMake(const Duration& duration)
{
	auto[m, d] = normalized(duration);
	if (d != 0_D && m != 0_M) return std::nullopt;
	return d == 0_D ? tryMake(m) : tryMake(d);
}

Duration Frequency::toDuration() const
{
	if (rep_ == 0) return Years::max(); // never happens!
	if (rep_ <= 12) return std::chrono::duration_cast<Months>(1_Y) / static_cast<int>(rep_);
	if (rep_ <= 52) return std::chrono::duration_cast<Weeks>(1_Y) / static_cast<int>(rep_);
	return std::chrono::duration_cast<Days>(1_Y) / static_cast<int>(rep_);
}

TP_ENUM_CREATE(VerboseFrequency, NEVER = 0, ANNUALLY = 1, SEMIANNUALLY = 2, TRIANNUALLY = 3, QUARTERLY = 4, BIMONTHLY = 6, MONTHLY = 12, FORTNIGHTLY = 26, WEEKLY = 52, DAILY = 365)
TP_ENUM_VALUE(NEVER, "NEVER")
TP_ENUM_VALUE(NEVER, "ZERO_COUPON") // should not be there but will make formatting more robust
TP_ENUM_VALUE(NEVER, "ZERO COUPON")
TP_ENUM_VALUE(ANNUALLY, "ANNUAL")
TP_ENUM_VALUE(ANNUALLY, "ANNUALLY")
TP_ENUM_VALUE(SEMIANNUALLY, "SEMI ANNUAL")
TP_ENUM_VALUE(SEMIANNUALLY, "SEMI_ANNUAL")
TP_ENUM_VALUE(SEMIANNUALLY, "SEMI ANNUALLY")
TP_ENUM_VALUE(TRIANNUALLY, "TRI ANNUAL")
TP_ENUM_VALUE(TRIANNUALLY, "TRI ANNUALLY")
TP_ENUM_VALUE(QUARTERLY, "QUARTERLY")
TP_ENUM_VALUE(BIMONTHLY, "BI MONTHLY")
TP_ENUM_VALUE(MONTHLY, "MONTHLY")
TP_ENUM_VALUE(FORTNIGHTLY, "FORTNIGHTLY")
TP_ENUM_VALUE(FORTNIGHTLY, "FORTNIGHT")
TP_ENUM_VALUE(WEEKLY, "WEEKLY")
TP_ENUM_VALUE(DAILY, "DAILY")
TP_ENUM_CREATE_END(VerboseFrequency)

TP_ENUM_CREATE(BondFrequency, TERM = 0, A = 1, SA = 2)
TP_ENUM_VALUE(TERM, "TERM")
TP_ENUM_VALUE(TERM, "ZERO_COUPON")
TP_ENUM_VALUE(TERM, "ZERO COUPON")
TP_ENUM_VALUE(A, "A")
TP_ENUM_VALUE(SA, "SA")
TP_ENUM_CREATE_END(BondFrequency)

namespace
{
	std::string getRegularOrFuture(const char str)
	{
		// between %f and %r
		switch (std::toupper(str))
		{
		case 'S': case 'Q': case 'B': case 'A': case 'Z':
			return "%f";
		default:
			return "%r";
		}
	}

	std::string getDefaultFmt(const std::string_view str)
	{
		if (str.length() == 0) return "%r";
		if (std::isdigit(str[0])) return "%d";
		if (str.length() == 1) return getRegularOrFuture(str[0]);
		if (str.length() > 4) return "%v";
		return "%b";
	}

	template<typename EnumT>
	auto fromEnumStr(const std::string_view& str)
	{
		return EnumT::tryParse(str)
			.transform([](const EnumT& i)
		{
			return Frequency(static_cast<unsigned short>(i.toIntegral()));
		});
	}

}

OptFrequency Formatter<Frequency>::tryParse(const std::string_view& str) const
{
	const auto fmt = override_.valueOr([str]() { return getDefaultFmt(str); });
	if (str.empty() || fmt.length() != 2 || fmt[0] != '%') return std::nullopt;

	if (fmt[1] == 'd')
	{
		static Formatter<Duration> monthFmt;
		return monthFmt.tryParse(str).andThen([](const auto& d) { return Frequency::tryMake(d); });
	}

	if (fmt[1] == 'v') return fromEnumStr<VerboseFrequency>(str);

	if (fmt[1] == 'b') return fromEnumStr<BondFrequency>(str);

	if (str.length() != 1) return std::nullopt;

	if (fmt[1] == 'f')
	{
		switch (std::toupper(str[0]))
		{
		case 'S':
			return S;
		case 'Q': case 'Z':
			return Q;
		case 'B':
			return B;
		case 'A':
			return A;
		default:
			return std::nullopt;
		}
	}

	if (fmt[1] == 'r')
	{
		switch (std::toupper(str[0]))
		{
		case 'D':
			return D;
		case 'W':
			return W;
		case 'M':
			return M;
		case 'Q':
			return Q;
		case 'Y':
			return Y;
		default:
			return std::nullopt;
		}
	}
	return std::nullopt;
}

namespace
{
	template<typename EnumT>
	std::string fromFrequency(const Frequency& value, const std::string& format)
	{
		const auto supported = EnumT::range();
		const auto it = std::find_if(supported.begin(), supported.end(), [x = static_cast<int>(value.count())](const auto& e)
		{
			return e == x;
		});
		if (it != supported.end()) return (+*it).toString();
		throw std::invalid_argument(fmt::format("Unsupported frequency (rep:{}) for selected format {}.", value.count(), format));
	}

}

std::string Formatter<Frequency>::str(const Frequency& value) const
{
	const auto fmt = override_.valueOr([&value]() -> std::string
	{
		return value.count() == 0 ? "%v" : "%d";
	});
	if (fmt.length() != 2 || fmt[0] != '%')
	{
		throw std::invalid_argument(fmt::format("Unsupported frequency format {}, supported formats are [%b, %f, %v, %r, %d].", fmt));
	}

	if (value.count() == 0) return "NEVER";

	if (fmt[1] == 'd')
	{
		static Formatter<Duration> monthFmt;
		return monthFmt.str(value.toDuration());
	}

	if (fmt[1] == 'v') return fromFrequency<VerboseFrequency>(value, fmt);
	if (fmt[1] == 'b') return fromFrequency<BondFrequency>(value, fmt);

	if (fmt[1] == 'f')
	{
		if (value == S) return "S";
		if (value == Q) return "Q";
		if (value == B) return "B";
		if (value == A) return "A";

		const auto message = fmt::format("Unsupported future frequency (rep:{}). Supported frequencies are [S, Q, B, A]", value.count());
		throw std::invalid_argument(message);
	}

	if (fmt[1] == 'r')
	{
		if (value == D) return "D";
		if (value == W) return "W";
		if (value == M) return "M";
		if (value == Q) return "Q";
		if (value == Y) return "Y";
		const auto message = fmt::format("Unsupported future frequency (rep:{}). Supported frequencies are [D, W, M, Q, Y]", value.count());
		throw std::invalid_argument(message);
	}

	throw std::invalid_argument(fmt::format("Unsupported frequency format {}, supported formats are [%b, %f, %v, %r, %d].", fmt));

}

std::string Formatter<Frequency>::failToParse(const std::string_view& str) const
{
	return ::failToParse<Frequency>(str, __FILE__, __LINE__);
}

TP::Optional<FrequencyIndexed> Formatter<FrequencyIndexed>::tryParse(const std::string_view& str) const
{
	if (str.length() < 2 || !std::isdigit(str[1])) return std::nullopt;

	const Formatter<Frequency> freqFmt{ override_.valueOr([x = str[0]]()
	{
		return getRegularOrFuture(x);
	}) };

	const auto result = freqFmt
		.tryParse(str.substr(0, 1))
		.andThen([v = str.substr(1)](const Frequency& f)
	{
		return TP::fromStringView<unsigned>(v)
			.transform([f](const auto i)
		{
			return f[i];
		});
	});
	return result;
}

std::string Formatter<FrequencyIndexed>::str(const FrequencyIndexed& value) const
{
	const auto format = override_.valueOr("%r");
	if (format.length() != 2 || format[0] != '%' || !(format[1] == 'r' || format[1] == 'f'))
		throw std::invalid_argument(fmt::format("Unsupported frequency index format {}, supported formats are [%f, %r].", format));

	const Formatter<Frequency> freqFmt{ format };
	return fmt::format("{}{}", freqFmt.str(value.basis()), value.index());
}

std::string Formatter<FrequencyIndexed>::failToParse(const std::string_view& str) const
{
	return ::failToParse<FrequencyIndexed>(str, __FILE__, __LINE__);
}
// ReSharper restore CppMemberFunctionMayBeStatic

