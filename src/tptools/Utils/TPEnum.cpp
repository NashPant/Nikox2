// CPP
#include "TPEnum.h"
//#include "common/util/FiasPlusException.h"
#include "TPTools/TPException.h"
#include <fmt/format.h>
#include <fmt/ranges.h>

namespace TP::details
{
	bool existEnumValue(const std::string& valStr, const CodeToValueMap& map)
	{
		const auto it = map.find(valStr);
		return it != map.end();
	}

	Optional<UnderlyingType> tryParseEnum(const std::string_view valStr, const CodeToValueMap& map)
	{
		const auto it = map.find(valStr);
		return (it != map.end()) ? makeOptional(it->second) : std::nullopt;
	}

	const std::string& getEnumString(UnderlyingType val, const ValueToCodeMap& map, const std::string_view& enumName)
	{
		const auto it = map.find(val);
		if (it == map.end())
		{
			THROW(fmt::format("Value {} of enum {} has no matching string representation.", val, enumName));
		}
		return it->second;
	}

	UnderlyingType getEnumValue(
		std::string_view valStr,
		const CodeToValueMap& map,
		const std::string_view& enumName,
		const CodeSet& list
	)
	{
		const auto it = map.find(valStr);
		if (it == map.end())
		{
			THROW(format("String {0} is not recognized as a valid {1}. Valid {1} names are [{2}]", valStr, enumName, fmt::join(list.begin(), list.end(), ", ")));
		}
		return it->second;
	}

	int initializeValues(
		const EnumToStringVector& values,
		CodeSet& codes,
		CodeToValueMap& codesToValues,
		ValueToCodeMap& valuesToCodes
	)
	{
		for (const auto& value : values)
		{
			codesToValues[value.second] = value.first;
			if (valuesToCodes.find(value.first) == valuesToCodes.end())
			{
				codes.insert(value.second);
				valuesToCodes[value.first] = value.second;
			}
		}
		return 0;
	}
}

