#pragma once

//#include "common/algorithm/string.h"
#include "TPTools/StringUtils/TPStringUtils.h"
#include <TPNikopolisUtils.h>
// those are used in the macro definition
// ReSharper disable CppUnusedIncludeDirective
#include "Operators.h"
#include "TPOptional.h"

#include <array>
#include <vector>
#include <map>
#include <set>
#include <string_view>
#include <string>
#include <ostream>
#include <functional>

// ReSharper restore CppUnusedIncludeDirective

/*

TPEnum.h provides macros to create Enum classes.

To create a new enum class, you must call the following macros in strict order:

TP_ENUM_CREATE -    Defines the name of the Enum, and the enum values (with corresponding underlying type values).
					  For a new enum this is called only once.

TP_ENUM_MAP_VALUE - This maps enumeration values to string representation. A value can map to a number of string representations,
					  but the first mapping will be the primary representation (used for string representation of the enum).
					  This can be called a number of times.

TP_ENUM_CREATE_END - This completes the enum class definition, and is mandatory.

Sample usage:

	TP_ENUM_VALUE(Red, "Red") // First mapping defines primary representation
	TP_ENUM_VALUE(Red, "rgb(255,0,0)") // Subsequent mappings define additional string representations.
	TP_ENUM_VALUE(Green, "Green")

	Colour red("Red"); // Can build from Red or red
	Colour green("Green");

	// Convert to primary string rep
	red.toString(); // returns "Red"
	std::cout << red << std::endl;

	// Number of enum values
	Colour::size(); // returns 2 (2 enum values)

	// Convenient switching
	switch (red)
	{
		case Colour::Red:
			break;
		case Colour::Green:
			break;
	}

	// Comparisons
	if (red < green)
		...

	if (red == green)
		...

	if (red != green)
		...

Limitations:

Sometimes, one must prefix constants such as Colour::Red with a '+' to explicitly promote them to type Colour:

	colour == +Colour::Red
*/
// NEVER USE OUTSIDE THIS SCOPE FILE
namespace TP::details
{
	using UnderlyingType = int;
	using CaseInsensitiveLess = TP::stringutils::ILess;

	using EnumToStringVector = std::vector<std::pair<UnderlyingType, std::string>>;
	using CodeSet = std::set<std::string_view, CaseInsensitiveLess>;
	using ValueToCodeMap = std::map<UnderlyingType, std::string>; // todo make it string_view
	using CodeToValueMap = std::map<std::string_view, UnderlyingType, CaseInsensitiveLess>;

	// Arg wrappers are simple ways to interact with __VA_ARGS__ that have assigned values!
	struct ArgWrapper final
	{
		constexpr ArgWrapper() : value(0), empty(true) {}
		constexpr ArgWrapper(UnderlyingType in) : value(in), empty(false) {}

		constexpr ArgWrapper& operator=(UnderlyingType in)
		{
			value = in;
			empty = false;
			return *this;
		}

		UnderlyingType value;
		bool empty;
	};

	// Helper that resolves all the enum possible values when passed __VA_ARGS__
	template <typename Enumerated, typename ...ArgWrapperT>
	constexpr auto resolveEnumValues(const ArgWrapperT&... in)
	{
		const ArgWrapper wrappers[] = { in... };

		std::array<Enumerated, sizeof...(ArgWrapperT)> result{};
		UnderlyingType value = 0;
		size_t i = 0;
		for (auto&& wrapper : wrappers)
		{
			value = wrapper.empty ? value : wrapper.value;
			result[i] = static_cast<Enumerated>(value);
			++value;
			++i;
		}

		return result;
	}

	TPNIKOPOLIS_IMPEXP int  initializeValues(const EnumToStringVector& values, CodeSet& codes, CodeToValueMap& codesToValues, ValueToCodeMap& valuesToCodes);

	TPNIKOPOLIS_IMPEXP const std::string& getEnumString(UnderlyingType val, const ValueToCodeMap& map, const std::string_view& enumName);

	TPNIKOPOLIS_IMPEXP Optional<UnderlyingType> tryParseEnum(std::string_view valStr, const CodeToValueMap& map);

	TPNIKOPOLIS_IMPEXP UnderlyingType getEnumValue(std::string_view valStr, const CodeToValueMap& map, const std::string_view& enumName, const CodeSet& list);

}


#define TP_ENUM_CREATE(Enum, ...) \
    class Enum : TP::operators::totally_ordered<Enum>  \
    { \
        using ValueToCodeMap = TP::details::ValueToCodeMap;\
        using CodeToValueMap = TP::details::CodeToValueMap;\
        using CodeSet	     = TP::details::CodeSet;\
\
    public: \
        using UnderlyingType = TP::details::UnderlyingType; \
\
        enum _enumerated : UnderlyingType { __VA_ARGS__ }; \
\
        Enum() = default; \
        Enum(const Enum& other) = default; \
        Enum(Enum&& other) = default; \
        Enum& operator =( const Enum &) = default; \
        Enum& operator =( Enum &&) = default ; \
        ~Enum() = default; \
        explicit Enum(_enumerated value) noexcept : value_(value) {} \
        Enum(const std::string& str)\
            : value_(TP::details::getEnumValue(str, codeValues(), #Enum, codes())) {} \
\
        static TP::Optional<Enum> tryParse(const std::string_view& val) \
        {\
            const auto value = TP::details::tryParseEnum(val, codeValues());\
            return value.transform([](const auto v) { return Enum(v); });\
        } \
\
        constexpr UnderlyingType toIntegral() const noexcept { return value_; } \
        constexpr operator _enumerated() const { return _enumerated(value_); }\
        friend const Enum operator +(Enum::_enumerated enumerated) { return static_cast<Enum>(enumerated); } \
\
        const auto& toString() const { return TP::details::getEnumString(value_, valueCodes(), #Enum); } \
\
        constexpr static auto size() { return size_; }\
        constexpr static auto range() { return range_; }\
        static const CodeSet& codes() { return codes_(); }\
\
    private:\
        constexpr static auto size_ = [] () constexpr {\
            using ArgWrapper = TP::details::ArgWrapper; \
            ArgWrapper __VA_ARGS__; \
            return std::initializer_list<ArgWrapper>{__VA_ARGS__}.size(); \
        }();\
\
        constexpr static auto range_ = [] () constexpr {\
            using ArgWrapper = TP::details::ArgWrapper; \
            ArgWrapper __VA_ARGS__; \
            return TP::details::resolveEnumValues<_enumerated>(__VA_ARGS__);\
        }();\
\
        explicit Enum(UnderlyingType value) : value_(value) {} \
\
        static const void initValueCodes() \
        { \
            static TP::details::EnumToStringVector valueCodeVec = { \


#define TP_ENUM_MAP_VALUE(Enum, Value, Code) { _enumerated::Value, Code },\


#define TP_ENUM_VALUE(Value, Code) { _enumerated::Value, Code },\


#define TP_ENUM_CREATE_END(Enum) \
            }; \
\
            TP::details::initializeValues(valueCodeVec, codes_(), codeValues_(), valueCodes_());\
        } \
\
        static const ValueToCodeMap& valueCodes() { initValueCodes(); return valueCodes_(); }\
        static const CodeToValueMap& codeValues() { initValueCodes(); return codeValues_(); }\
\
        UnderlyingType value_; \
        static ValueToCodeMap& valueCodes_() { static ValueToCodeMap val; return val; }\
        static CodeToValueMap& codeValues_() { static CodeToValueMap val; return val; }\
        static CodeSet&	codes_() { static CodeSet val; return val; }\
    public:\
\
        friend constexpr bool operator ==(const Enum &a, const Enum &b) noexcept { return a.toIntegral() == b.toIntegral(); } \
        friend constexpr bool operator <(const Enum &a, const Enum &b)  noexcept { return a.toIntegral() < b.toIntegral(); } \
\
        template <typename Char, typename Traits> \
        friend std::basic_ostream<Char, Traits>&  operator <<(std::basic_ostream<Char, Traits>& stream, const Enum &value)  {  return stream << value.toString(); } \
    }; \

#define TP_ENUM_MAKE_HASHABLE(Enum)\
namespace std\
{\
    template<> struct hash<Enum>\
    {\
        size_t operator()(const Enum& e) const noexcept	{\
            const auto h = std::hash<int>();\
            return h(e.toIntegral());\
        }\
    };\
}

