
#pragma once
#include <fstream> 
#include <iostream>
#include <sstream> 
#include <tuple>
namespace TP
{
	namespace utils
	{
	template<typename T> struct PropertyWrapper;
	template <typename T> void serialize(const T& t, std::ostream& out); // namespace utils
	}
}
#define PARENS ()
#define EXPAND(...)  EXPAND4(EXPAND4(EXPAND4(EXPAND4(__VA_ARGS__))))
#define EXPAND4(...) EXPAND3(EXPAND3(EXPAND3(EXPAND3(__VA_ARGS__))))
#define EXPAND3(...) EXPAND2(EXPAND2(EXPAND2(EXPAND2(__VA_ARGS__))))
#define EXPAND2(...) EXPAND1(EXPAND1(EXPAND1(EXPAND1(__VA_ARGS__))))
#define EXPAND1(...) __VA_ARGS__

#define FOR_EACH(macro, ...) \
	__VA_OPT__(EXPAND(FOR_EACH_HELPER(macro, __VA_ARGS__)))
#define FOR_EACH_HELPER(macro, a1, ...) \
	macro(a1) __VA_OPT__(FOR_EACH_AGAIN PARENS(macro, __VA_ARGS__))
#define FOR_EACH_AGAIN() FOR_EACH_HELPER


#define MAKE_PROPERTY(CLASSNAME, ...) \
    __VA_OPT__(EXPAND(MAKE_PROPERTY_(CLASSNAME, __VA_ARGS__)))

#define MAKE_PROPERTY_(CLASSNAME, a1, ...)\
	std::make_tuple (#a1, &CLASSNAME::a1) \
		__VA_OPT__(, MAKE_PROPERTY_AGAIN PARENS(CLASSNAME, __VA_ARGS__))
#define MAKE_PROPERTY_AGAIN() MAKE_PROPERTY_


/* PROPERTIES: creates get_properties() returning a tuple of property tuples */
#define PROPERTIES(CLASSNAME, ...) \
    static const auto& get_properties() { \
        static const auto tuple = \
            std::make_tuple(MAKE_PROPERTY(CLASSNAME, __VA_ARGS__)); \
        return tuple; \
    } \
    static const auto classname() { return #CLASSNAME; } \
    friend struct TP::utils::PropertyWrapper<CLASSNAME>;


/* PROPERTIES_PLUS: same as PROPERTIES but also adds defaulted spaceship operator */
#define PROPERTIES_PLUS(CLASSNAME, ...) \
    PROPERTIES(CLASSNAME, EXPAND1(__VA_ARGS__)) \
    auto operator<=>(const CLASSNAME&) const = default

namespace TP
{
	namespace utils
	{
		template<typename T> void json_serialize(std::ostream& ostream, const T& t);
		template<typename T> void json_deserialize(std::istream& istream, T& t);

		template<typename T> std::string json_serialize(const T& t)
		{
			std::stringstream ss;
			utils::json_serialize(ss, t);
			return ss.str();

		}
		template<typename T> T json_deserialize(const std::string& str)
		{

			std::istringstream ss(str);
			T t;
			utils::json_deserialize(ss, t);
			return t;
		}
		template<typename T> void to_json_file(const std::string& filename, T& t)
		{

			std::ofstream file(filename);
			utils::json_serialize(file, t);
			file.close();
		}
		template<typename T> T from_json_file(std::ifstream& file)
		{
			T t;
			utils::json_deserialize(file, t);
			file.close();
			return t;
		}
		template<typename T> T from_json_file(const std::string& filename)
		{
			std::ifstream file(filename);
			return from_json_file<T>(file);
		}
	}
	// namespace utils
}