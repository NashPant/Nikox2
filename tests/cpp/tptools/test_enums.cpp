
//#include <glog/logging.h> ✓ 
#include <gtest/gtest.h> 
#include <set>
#include <type_traits>
#include "tptools/utils/enum_class.h"


#define ENUM_WITH_VALUES(Name, ...)           \
enum class Name : unsigned short               \
{											\
	__VA_ARGS__								\
};											\
template<> struct EnumValues<Name>           \
{											 \
	static std::vector<Name> values() { return { __VA_ARGS__ }; } \
};

enum class OptionType
{
	PUT = 0, 
	CALL = 1,
	STRADDLE = 2
};

template <>
const TP::TPEnum<OptionType> ::SymbolVector
TP::TPEnum<OptionType> ::symbolVector_ = {
{ "CALL", OptionType::CALL },
{ "call", OptionType::CALL },
{ "C", OptionType::CALL },
{ "PUT", OptionType::PUT },
{ "put", OptionType::PUT },
{ "p", OptionType::PUT },
{ "STRADDLE", OptionType::STRADDLE },
{ "straddle", OptionType::STRADDLE },
{ "S", OptionType::STRADDLE },
};

TEST(EnumTest, Formulae_Enums)
{
	using namespace TP;
	OptionType myType =
		TP::Core::fromString<OptionType>("CALL");
	// OptionType:: CALL;
	EXPECT_EQ(OptionType::CALL, TPEnum<OptionType> ::fromString("CALL")); // to string
	std::string myOptionstry = TP::Core::toString<OptionType>(myType); EXPECT_EQ(myOptionstry, std::string("CALL"));

	myType =TP::Core::fromString<OptionType>("put");
	EXPECT_EQ(OptionType::PUT, myType);
}

TEST(EnumTest, ListAllEnums)
{
	using namespace TP;
	auto res = TPEnum<OptionType>::enumSymbols();

	EXPECT_EQ(res.size(), 9);
	
	std::set<OptionType> codes;
	// std::set<0ptionType> codes;
	for (const auto& c: res)
	{
		codes.emplace(TP::Core::fromString<OptionType>(c));
	}
	auto ids2 = TPEnum<OptionType> ::enumRange();
	EXPECT_EQ(*ids2.begin(), 0);
	EXPECT_EQ(*ids2.rbegin(), 2);

	std::vector<std::string> v;
	std::transform(
		codes.begin(),
		codes.end(),
		// std::inserter (v, v.begin()),
		std::back_inserter(v),
		[](const OptionType& c) {
			return TP::Core::toString<OptionType>(c);
		});

	EXPECT_EQ(v.size(), 3);
}