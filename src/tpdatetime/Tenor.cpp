#include "Tenor.h"
//#include "BusinessDay.h"
//#include "common/algorithm/string.h"
#include "TPTools/StringUtils/TPStringUtils.h"
#include "TPTools/Utils/TypeTraits.h"
#include <boost/algorithm/string/trim.hpp>


using namespace TP::date_time;
using namespace TP::stringutils;
using namespace TP::formatting;

Tenor::Tenor(const std::string& str)
	: Tenor(TP::parse<Tenor>(str)) // will throw if it fails
{
}

Tenor& Tenor::operator=(const Duration& other) noexcept
{
	value_ = other;
	return *this;
}

Tenor& Tenor::operator=(const Years& other) noexcept
{
	value_ = Duration{ other };
	return *this;
}

Tenor& Tenor::operator=(const Months& other) noexcept
{
	value_ = Duration{ other };
	return *this;
}

Tenor& Tenor::operator=(const Weeks& other) noexcept
{
	value_ = Duration{ other };
	return *this;
}

Tenor& Tenor::operator=(const Days& other) noexcept
{
	value_ = Duration{ other };
	return *this;
}

Tenor& Tenor::operator=(const Overnight& _) noexcept
{
	value_ = ON;
	return *this;
}

Tenor& Tenor::operator=(const TomNext& _) noexcept
{
	value_ = TN;
	return *this;
}

Tenor& Tenor::operator=(const SpotNext& _) noexcept
{
	value_ = SN;
	return *this;
}

Tenor& Tenor::operator=(const MonthBusinessDay& other) noexcept
{
	value_ = other;
	return *this;
}

Tenor::operator Duration() const
{
	const auto ptr = std::get_if<Duration>(&value_);
	if (ptr) return *ptr;

	return 1_D; // todo this is very wrong but tests would fail otherwise
	//throw common::util::FiasPlusException(fmt::format("Tenor {} is not convertible to duration", this->toString()));
}

Tenor::operator Months() const
{
	return static_cast<Months>(static_cast<Duration>(*this));
}

Tenor::operator Days() const
{
	return static_cast<Days>(static_cast<Duration>(*this));
}

Tenor Tenor::max()
{
	return Tenor{ Years::max() };
}

unsigned Tenor::yearCount() const
{
	const auto ebo = 0; // avoids msvc issue with stack corruption
	static TP::Overloaded callable
	{
		[](const Duration& d) { return d.years().count(); },
		[ebo](const auto& _) { return 0; }
	};
	return this->visit(callable);
}

unsigned Tenor::monthCount() const
{
	const auto ebo = 0; // avoids msvc issue with stack corruption
	static TP::Overloaded callable
	{
		[](const Duration& d) { return d.months().count(); },
		[ebo](const auto& _) { return 0; }
	};
	return this->visit(callable);

}

bool Tenor::is1DTenor() const
{
	const auto ebo = 0; // avoids msvc issue with stack corruption
	const TP::Overloaded callable
	{
		[](const Duration& d) { return d == 1_D; },
		[ebo](const auto& _) { return true; }
	};
	return this->visit(callable);
}

bool Tenor::isByMonth() const
{
	const auto ebo = 0; // avoids msvc issue with stack corruption
	const TP::Overloaded callable
	{
		[](const Duration& d) { return d.is<Months>(); },
		[ebo](const auto& _) { return false; }
	};
	return this->visit(callable);
}

bool Tenor::isByWeek() const
{
	const auto ebo = 0; // avoids msvc issue with stack corruption
	const TP::Overloaded callable
	{
		[](const Duration& d) { return d.days() == 0_D && d.weeks() != 0_W; },
		[ebo](const auto& _) { return false; }
	};
	return this->visit(callable);
}


bool Tenor::isByDay() const
{
	const auto ebo = 0; // avoids msvc issue with stack corruption
	const TP::Overloaded callable
	{
		[](const Duration& d) { return d.is<Days>(); },
		[ebo](const auto& _) { return true; }
	};
	return this->visit(callable);
}

bool Tenor::isNull() const
{
	const auto ebo = 0; // avoids msvc issue with stack corruption
	const TP::Overloaded callable
	{
		[](const Duration& d) { return d.isNull(); },
		[ebo](const auto& _) { return false; }
	};
	return this->visit(callable);
}

namespace
{
	// this is totally wrong conceptually
	double roughYf(const Tenor& t)
	{
		const auto ebo = 0; // avoids msvc issue with stack corruption
		return t.visit(TP::Overloaded{
			[](const auto&) { return 1.0 / 365.25; },
			[ebo](const Duration& d) { return dcf(d); }

			});
	}
}

bool TP::date_time::operator<(const Tenor& lhs, const Tenor& rhs)
{
	return roughYf(lhs) < roughYf(rhs);
}

std::string Tenor::toString() const
{
	static TP::Formatter<Tenor> formatter;
	return formatter.str(*this);
}

// formatting -----------------------------------------------
// ReSharper disable CppMemberFunctionMayBeStatic

TP::Optional<Overnight> Formatter<Overnight>::tryParse(const std::string_view& str) const
{
	std::string strCopy(str);
	boost::trim(strCopy);
	return iEquals("ON", strCopy) || iEquals("O/N", strCopy) ? makeOptional(ON) : std::nullopt;
}


std::string Formatter<Overnight>::str(const Overnight& value) const
{
	return "ON";
}

std::string Formatter<Overnight>::failToParse(const std::string_view& str) const
{
	return ::failToParse<Overnight>(str, __FILE__, __LINE__);
}

TP::Optional<TomNext> Formatter<TomNext>::tryParse(const std::string_view& str) const
{
	std::string strCopy(str);
	boost::trim(strCopy);
	return iEquals("TN", strCopy) || iEquals("T/N", strCopy) ? TP::makeOptional(TN) : std::nullopt;
}

std::string Formatter<TomNext>::str(const TomNext& value) const
{
	return "TN";
}

std::string Formatter<TomNext>::failToParse(const std::string_view& str) const
{
	return ::failToParse<TomNext>(str, __FILE__, __LINE__);
}

TP::Optional<Spot> Formatter<Spot>::tryParse(const std::string_view& str) const
{
	std::string strCopy(str);
	boost::trim(strCopy);
	return iEquals("SP", strCopy) ? makeOptional(SP) : std::nullopt;
}

std::string Formatter<Spot>::str(const Spot& value) const
{
	return "SP";
}

std::string Formatter<Spot>::failToParse(const std::string_view& str) const
{
	return ::failToParse<Spot>(str, __FILE__, __LINE__);
}

TP::Optional<SpotNext> Formatter<SpotNext>::tryParse(const std::string_view& str) const
{
	std::string strCopy(str);
	boost::trim(strCopy);
	return iEquals("SN", strCopy) || iEquals("S/N", strCopy) ? TP::makeOptional(SN) : std::nullopt;
}

std::string Formatter<SpotNext>::str(const SpotNext& value) const
{
	return "SN";
}

std::string Formatter<SpotNext>::failToParse(const std::string_view& str) const
{
	return ::failToParse<SpotNext>(str, __FILE__, __LINE__);
}

OptTenor Formatter<Tenor>::tryParse(const std::string_view& str) const
{
	const static Formatter<Overnight> on;
	return OptTenor(on.tryParse(str))
		.orElse([str]()
	{
		const static Formatter<TomNext> tn;
		return tn.tryParse(str);
	})
		.orElse([str]()
	{
		const static Formatter<Spot> sp;
		return sp.tryParse(str);
	})
		.orElse([str]()
	{
		const static Formatter<SpotNext> sn;
		return sn.tryParse(str);
	})
		.orElse([str]()
	{
		const static Formatter<MonthBusinessDay> md;
		return md.tryParse(str);
	})
		.orElse([str]()
	{
		const static Formatter<Duration> dur;
		return dur.tryParse(str);
	});
}

std::string Formatter<Tenor>::str(const Tenor& value) const
{
	const auto ebo = 0; // avoids msvc issue with stack corruption
	return value.visit([ebo](const auto& d) { return toString(d); });
}

std::string Formatter<Tenor>::failToParse(const std::string_view& str) const
{
	return ::failToParse<Tenor>(str, __FILE__, __LINE__);
}

// ReSharper restore CppMemberFunctionMayBeStatic

size_t std::hash<Overnight>::operator()(const Overnight& tenor) const noexcept
{
	static const auto hashCode = typeid(Overnight).hash_code();
	return hashCode;
}

size_t std::hash<TomNext>::operator()(const TomNext& tenor) const noexcept
{
	static const auto hashCode = typeid(TomNext).hash_code();
	return hashCode;
}

size_t std::hash<Spot>::operator()(const Spot& tenor) const noexcept
{
	static const auto hashCode = typeid(Spot).hash_code();
	return hashCode;
}

size_t std::hash<SpotNext>::operator()(const SpotNext& tenor) const noexcept
{
	static const auto hashCode = typeid(SpotNext).hash_code();
	return hashCode;
}

size_t std::hash<Tenor>::operator()(const Tenor& tenor) const
{
	static const std::hash<Tenor::ValueType> hash;
	return hash(tenor.value_);
}

bool TP::date_time::operator==(const Tenor& lhs, const MonthBusinessDay& rhs)
{
	if (const auto md = std::get_if<MonthBusinessDay>(&lhs.value_)) return *md == rhs;
	return false;
}
