////////////////////////////////////////////////////////////////////////////
//
//  This program is the  property of'
//   Athanasios Pantas. Feel free to use, but no guarantees from me!
//  
//
// Copyright (C) 2003  - Nikopolis
//  Copyright (C) 2003 - Athanasios Pantas.
//  ALL RIGHTS RESERVED.
/////////////////////////////////////////////////////////////////////////////


#ifndef TP_STRING_UTILS_H
#define TP_STRING_UTILS_H


//#include <boost/optional/optional.hpp>
#include <optional>
#include <TPNikopolisUtils.h>
//#include "../Time/TPDateUtils.h"
#include "../TPException.h"
#include <sstream>

#ifdef TPSTATIC
	#include <sstream>
#else
	#ifndef WIN32
		#include <fstream>
	#else
       
		//#include <fstream.h>
	#endif
#endif
#include <algorithm>
#include <iomanip> 

#include <string>
#include <vector>
#include <cctype>

#include <list>
#include <map>
#include <ranges>


#pragma warning (disable : 4267)

//#include <string>

//#define NOMINMAX

#ifdef TP_STATIC
template <class out_type, class in_value>
TPNIKOPOLIS_IMPEXP out_type TP_static_cast(const in_value & t)
{
	std::stringstream stream;
	if ( ! (stream << t) ) // insert value to stream
	{
		THROW("TP_static_cast: conversion not possible because of invalid source type");
	}
	out_type result; // store conversion’s result here
	if (! (stream >> result) ) // write value to result
	{
		THROW("TP_static_cast: conversion not possible because of invalid target type");
	}
	return result;
}
#endif

namespace TP {
	namespace stringutils
	{
		/**
		* @brief Each element of the input sequence is converted to lower case. The input sequence is not modified
		*/
		inline std::string toLower(std::string input)
		{
			std::transform(input.begin(), input.end(), input.begin(), [](unsigned char c) { return std::tolower(c); });
			return input;
		}

		/**
		* @brief Each element of the input sequence is converted to upper case. The input sequence is not modified
		*/
		inline std::string toUpper(std::string input)
		{
			std::transform(input.begin(), input.end(), input.begin(), [](unsigned char c) { return std::toupper(c); });
			return input;
		}

		/**
		* @brief A case-insensitive less predicate for strings (equivalent to is_iless but without locale dependency)
		*/
		static constexpr struct ILess
		{
			bool operator() (const std::string_view& x, const std::string_view& y) const
			{
				return std::lexicographical_compare(x.begin(), x.end(), y.begin(), y.end(), less);
			}

		private:
			static bool less(const unsigned char x, const unsigned char y)
			{
				static const std::less<> comparer;
				return comparer(std::toupper(x), std::toupper(y));
			}
		} iLess{};

		/**
		* @brief A case-insensitive equals predicate for strings (equivalent to iequals but without locale dependency)
		*/
		static constexpr struct IEquals
		{
			bool operator() (const std::string_view& x, const std::string_view& y) const
			{
				return std::equal(x.begin(), x.end(), y.begin(), y.end(), equals);
			}

		private:
			static bool equals(const unsigned char x, const unsigned char y)
			{
				return std::toupper(x) == std::toupper(y);
			}
		} iEquals{};

		static constexpr struct IStartsWith
		{
			bool operator() (const std::string_view& str, const std::string_view& substr) const
			{
				return std::equal(str.begin(), str.begin()+substr.size(), substr.begin(), substr.end(), equals);
			}

		private:
			static bool equals(const unsigned char x, const unsigned char y)
			{
				return std::toupper(x) == std::toupper(y);
			}
		}iStartsWith{};

		/*
		* last (length) char of a string
		*/
		inline std::string tail(std::string const& source, size_t const length) {
			if (length >= source.size()) { return source; }
			return source.substr(source.size() - length);
		}


		template <typename T>
		std::string to_string_with_precision(const T a_value, const int n = 3)
		{
			std::ostringstream out;
			out.precision(n);
			out << std::fixed << a_value;
			return out.str();
		}

		inline std::string toCamelCase(std::string in)
		{
			auto upperCase = true;
			std::transform(in.begin(), in.end(), in.begin(), [&upperCase](unsigned char c)
			{
				if (std::isalpha(c))
				{
					if (upperCase)
					{
						upperCase = false;
						return std::toupper(c);
					}
					return std::tolower(c);
				}
				if (std::isspace(c) || std::ispunct(c)) { upperCase = true; }
				return static_cast<int>(c);
			});

			return in;
		}
		/*
		*  Since Rest SDK required each strike with the correct number of digits and zeros are excluded
		*  so for example 98.00 should be "98", 98.250 should be "98.25" and 98.125 "98.125"
		*/

		class EqualFn
		{
		public:
			bool operator() (std::string const& t1, std::string const& t2) const
			{
				return toLower(t1) == toLower(t2);
			}
		};
		// Another Hasher
		class Hasher
		{
		public:
			size_t operator() (std::string const& key) const
			{
				auto const hasher = std::hash<std::string>{}(toLower(key));
				return hasher;
			}
		};

		inline std::string trim_copy_std(const std::string& s) {
			
			auto trimmed_view = s
				| std::views::drop_while(::isspace)
				| std::views::reverse
				| std::views::drop_while(::isspace)
				| std::views::reverse;

			return std::string(trimmed_view.begin(), trimmed_view.end());
		}

		inline std::string join(const auto& range) {
			auto it = std::ranges::begin(range);
			auto end = std::ranges::end(range);
			if (it == end) return "";

			std::string result = *it; // First element without delimiter
			++it;
			for (; it != end; ++it) {
				result += ", " + std::string(*it);
			}
			return result;
		}

	}
}


class  TPNIKOPOLIS_IMPEXP TPStringUtils
{
public:
  static void TPStripChar(std::string& tobestripped, const char* c);
  static void TPStripString(std::string& tobestripped, const std::string& c);
  // removes all the occurences of the std::string c in the std::string "tobestripped"
  static void TPEraseAllSubstrings(std::string& tobestripped,  std::string& c, const bool& casesensitive=true);

  
  static std::string& toUpper( std::string& s );
  static std::string& toLower( std::string& s );
  static std::string& noSpace( std::string& s );
  static std::string& noTrailingSpace( std::string& s );
  static std::string& noLeadingSpace( std::string& s );

  static void replaceAll(std::string& s, const std::string &search, const std::string &replace);
  
    // remove dublicate from a list of strings..
  static std::vector<std::string> removeduplicatesAndEmtyStrings(const std::vector<std::string>& myvector);

  static unsigned hash(const std::string& s) ;
  
  static bool isBlank( const std::string& s );
  
	template <class T>
	static void to_string(std::string & result, const T & t)
	{
		std::ostringstream oss; // create a stream
		oss << t; // insert value to stream
		result=oss.str(); // extract value and write it to result
	}

	// Extracts the sole name of the type without namespace information.
	//	 \p type is a template, then the name extracted is that of the template type without any template type arguments.
	static std::string stripTypeName(const std::type_info& type);
  /// Convert an integer to a std::string
  
  static std::string itos( int n );

  static std::string removeDigits(const std::string& s);
  
  /// Convert an double to a std::string
  
//  static std::string dtos( double d );

  // check whether string2 is contained in string1, returns false otherwise.
  // Will use the strstr() function for the comparison
  static bool	contains(const std::string& tobesearched, const std::string& tobefound, bool matchlength=true);

  // implementation of c++20 starts_with for c++17
  static bool starts_with(const std::string& str, const std::string& prefix) 
  {
	  return str.size() >= prefix.size() && str.compare(0, prefix.size(), prefix) == 0;
  }
  /** Convert from std::string to numeric.
  *  It checks that the first character is in "-+.0123456789",
  *  that all other characters are in ".0123456789",
  *  and there is no more than one '.' in the std::string.
  *  And convert to double
  *  @param str the std::string to convert to double
  *  @return the converted value (double)
  *  @throw cb::Error if one of the previous check is invalid
  */
  static double atof( const std::string& str );
  
  
  /// Non-case sensitive std::string comparison
  
  static int nocasecmp( const std::string& s1, const std::string& s2 );
  
  
  /// String tokenizer
  
  static void tok( const std::string& s, char sep,
    std::list<std::string>& tokens );
  
  
  /// String tokenizer
  
  static void tok( const std::string& s, const std::string& sep,
      std::list<std::string>& tokens );
  
  //Misc
  
  static bool isNumeric( const std::string& str );  
// Split left or right
  static std::string split(const std::string& str, const std::string& needle, const std::optional<bool>& blLeft);
  
  /*
   Split 2 strings given the seperators and make sure they have the same elements
   for example
   strings LON + EUR and EUR+ LON would be the same given separator + (including any blanks)
  */
  //static bool sameSplit(const std::string& s1, const std::string& s2, char sep);
  /*
    @brief. String to int given the seperator for the thousands (i.e.  , or ., etc)
  */
  static int str_to_int(const char* str, char thousands_separator);
  /*
	@brief. String to float given the seperator for the thousands and the comma (i.e.  , or ., etc)
  */
  static float str_to_float(const char* str, char decimal_point, char thousands_separator);
private:
  
  TPStringUtils();
  TPStringUtils( const TPStringUtils& );
  TPStringUtils& operator=( const TPStringUtils& );
};

class  StringUtilsNoCase
{
public:
  bool operator() ( const std::string& s1, 
    const std::string& s2 ) const
		{
		  return( TPStringUtils::nocasecmp( s1, s2 ) < 0 );
		}
};

typedef std::map<std::string,std::string,StringUtilsNoCase> StrMap;



std::string TPNIKOPOLIS_IMPEXP toUpper(const std::string& strIn);


const unsigned TP_HASH_SHIFT = 5;
const unsigned TP_BITRCERBYTE = 8;

//#ifdef TPSTATIC
inline
static void mash(unsigned& hash, unsigned chars)
{
  hash = (chars ^
    ((hash << TP_HASH_SHIFT) |
    (hash >> (TP_BITRCERBYTE*sizeof(unsigned) - TP_HASH_SHIFT))));
}

inline
unsigned TPStringUtils::hash(const std::string& s) 
{
  unsigned hv       = (unsigned)s.size(); // Mix in the std::string length.
  unsigned i        = s.size()*sizeof(char)/sizeof(unsigned);
  const char* p ;
  {
    while (i--)
    {
      p = &s[i];
      mash(hv, *p);			// XOR in the characters.
    }
  }
  // XOR in any remaining characters:
  if ((i = s.size()*sizeof(char)%sizeof(unsigned)) != 0) 
  {
    unsigned h = 0;
    const char* c = (const char*)p;
    while (i--) 
      h = ((h << TP_BITRCERBYTE*sizeof(char)) | *c++);
    mash(hv, h);
  }
  return hv;
}

//#endif

///////////////////////////////////////////////////////////////////////////////
inline
void TPStripChar(std::string& tobestripped, const char* c)
{
  std::string cache;
  for(auto i = 0 ; i < tobestripped.size() ; ++i)
  {
    char t = tobestripped[i];
    if(t != *c) cache += t;
  }
  tobestripped = cache;
}

///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
inline
void TPStripString(std::string& tobestripped, const std::string& c)
{
  if(c.size() == 0) return;

  size_t pos = 0;
  size_t prev_pos = 0;

  std::string cache;
  size_t i ;
  
  while(pos < tobestripped.size())
  {
    bool found_match = false;

    pos = tobestripped.find_first_of(c.at(0), pos);

    if(c.size() == 1 && pos != std::string::npos) found_match = true;

    if(pos != std::string::npos)
    {
      for(i = 1 ; i < c.size() && pos+i < tobestripped.size() ; ++i)
      {
        found_match = true;
        const char spy1 = tobestripped.at(pos+i);
        const char spy2 = c.at(i);

        if( spy1 != spy2) 
        {
          found_match = false;
          break;
        }
      }
    }
    else
    {
      pos = tobestripped.size();
    }

    for(size_t i = prev_pos ; i < pos ; ++i)
    {
      char t = tobestripped[i];
      cache += t;
    }

    if(!found_match) 
    {
      ++pos;
      prev_pos = pos;
    }
    else
    {
      prev_pos = pos + c.size() ;
      pos = prev_pos; 
    }
  }

  tobestripped = cache;
}

inline std::ostream&
operator << (std::ostream& o, const std::pair<std::string,std::string>&	mystr_pair)
{
	o << mystr_pair.first  << "\t" << mystr_pair.second ;
	return o;
}
#endif // TP_STRING_UTILS_H
