// This is -*- C++ -*-
// 
/* 
 * stringtools.h
*/


#ifndef _INC_STRINGTOOLS_H
#define _INC_STRINGTOOLS_H

#include <string>
#include <vector>
#include <stdio.h>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

//using namespace std;

/*
 * This is a struct that keeps a bunch of useful std::string manipulation routines.
 * By using a template, we can more easily add support for strings of
 * wide characters later.  (Though as it stands, I'm sure that several of
 * these functions are char-std::string specific.)
 *
 * We create a dummy instance of this struct for notational convenience.
 */
template<class str>
struct basic_string_tools {
  // A std::vector of strings
  typedef std::vector<str> strings;

  static str trim(const str& s, const str& trim_chars = " \t");
  static str cut(const str& s, const str& trim_chars = " \t");
  static str rcut(const str& s, const str& trim_chars = " \t"); 
  static str concatenate_words(const str& s1, const str& s2);
  
  static str to_upper(const str& s);
  static str to_lower(const str& s);

  static str substring(const str& s, size_t i1, size_t i2);
  static void split(const str& s, const str& delimiter,
		    str& head, str& tail,
		    bool keep_delimiter_in_tail = false);

  static bool prefix(const str& s, const str& prefix) {
    return s.find(prefix) == 0;
  }

  static bool suffix(const str& s, const str& prefix) {
    return s.rfind(prefix) == (s.size() - prefix.size());
  }

  // Replace all instances of a std::string with another std::string
  static str replace(str const & line, str const & from, str const & to) {
    if (from == to || from.empty())
      return line;
    str l = line;
    size_t pos = 0;
    while(true) {
      pos = l.find(from, pos);
      if (pos == l.npos)
	break;
      l.erase(pos, from.size());
      l.insert(pos, to);
      pos += to.size();
    }
    return l;
  }
  /*
   * Convert an integer to a std::string
   *
   * int2str(10) = "10";
   */
  static str int2str(int v) {
    char buf[10];
    //sprintf(buf, "%d", v);
    sprintf_s(buf, "%d", v);
    return str(buf);
  }

  static str int2str(unsigned int v) {
    char buf[10];
    sprintf(buf, "%u", v);
    return str(buf);
  }

  static str expand_tabs(const str& line, size_t tab_size = 8);

  static bool is_number_char(str c) { return isdigit(c[0]); }

  // Could this character be part of a line of numbers,
  // but also is it also likely to be part of a line of headings?
  static bool ambigious_number_char(str c) {
    return c[0] == '.' || c[0] == ',' || c[0] == 'e' || c[0] == 'E' 
      || c[0] == '+' || c[0] == '-' || c[0] == ' ' || c[0] == '\t';
  }
};

typedef basic_string_tools<std::string> string_tools;
extern string_tools string_tool;

//////////////////////////////////////////////////////////////////////////////

/*
 * Trims characters off at both ends of the std::string.
 *
 * trim("  abc  ") == "abc" == trim("..abc..", ".");
 */
template<class str>
str
basic_string_tools<str>::trim(const str& s, const str& trim_chars)
{
  return cut(rcut(s, trim_chars), trim_chars);
}

/*
 * Cuts characters off at the front of the std::string.
 *
 * cut("  abc  ") == "abc  " == cut("..abc..", "abc..");
 */
template<class str>
str
basic_string_tools<str>::cut(const str& s, const str& trim_chars)
{
  if (trim_chars.empty() || s.empty() ||
      s.find_first_of(trim_chars) == s.npos) return s;
  str work = s;
  size_t pos = work.find_first_not_of(trim_chars);
  if (pos == work.npos) 
    work.erase(work.begin(), work.end());
  else if (pos != 0) 
    work.erase(0, pos);

  return work;
}

/*
 * Cuts characters off at the end of the std::string.
 *
 * rcut("  abc  ") == "  abc" == rcut("..abc..", "..abc");
 */
template<class str>
str
basic_string_tools<str>::rcut(const str& s, const str& trim_chars)
{
  if (trim_chars.empty() || s.empty() ||
      s.find_first_of(trim_chars) == s.npos) return s;
  str work = s;
  size_t pos = work.find_last_not_of(trim_chars);
  if (pos != work.npos) {
    work = substring(work, 0, pos);
  } else {
    // It seems that my STL library is buggy, so we need this:
    work = substring(work, 0, work.find_first_of(trim_chars) - 1);
    // rather than work.clear();
  }
  return work;
}

/*
 * Adds two strings such that it looks nice.
 *
 * concatenate_words("This", "is a test") == "This is a test";
 * concatenate_words("Empty", "") == "Empty";
 * concatenate_words("", "Empty") == "Empty";
 * concatenate_words("", "") == "";
 * concatenate_words(" This ", " is a test ") == "This is a test";
 */
template<class str>
str
basic_string_tools<str>::concatenate_words(const str& s1, const str& s2)
{
  str work = trim(s1);
  if (!work.empty()) {
    work += ' ';
  }
  work += cut(s2);
  return rcut(work);
}

template<class str>
str
basic_string_tools<str>::to_upper(const str& s)
{
  str t = s;
  for(size_t i=0; i<t.size(); ++i)
    if (islower(t[i])) t[i] = toupper(t[i]);
  return t;
}

template<class str>
str
basic_string_tools<str>::to_lower(const str& s)
{
  str t = s;
  for(size_t i=0; i<t.size(); ++i)
    if (isupper(t[i])) t[i] = tolower(t[i]);
  return t;
}

/*
 * Extract a piece of a std::string.
 * Contrary to the STL substr, this method takes two
 * indexes, rather than an index and a length.
 *
 * Also, indexes beyond the end of the std::string are handled
 * in a tolerant manner.
 */
template<class str>
str
basic_string_tools<str>::substring(const str& s, size_t i1, size_t i2)
{
  if (i1 >= s.length() || i1 > i2) 
    return str();
  
  if (i2 >= s.length()) 
    i2 = s.length() - 1;
  
  str result = s.substr(i1, (i2-i1) + 1);
  return result;
}

/*
 * Split a std::string at a given delimiter.
 *
 * str head, tail;
 * split("This is a -!- test -!-", "-!-", head, tail);
 * head == "This is a " ; tail == " test -!-";
 * split("This is a -!- test -!-", "-!-", head, tail, true);
 * head == "This is a " ; tail == "-!- test -!-";
 * split("This is a test", "-!-", head, tail);
 * head == "This is a test" ; tail == "";
 */
template<class str>
void
basic_string_tools<str>::split(const str& s, const str& delimiter,
			       str& head, str& tail,
			       bool keep_delimiter_in_tail)
{
  size_t pos = s.find(delimiter);
  if (pos == s.npos) {
    head = s;
    tail = str();
  } else {
    head = pos == 0 ? std::string() : substring(s, 0,pos-1);
     if (!keep_delimiter_in_tail) 
      pos += delimiter.length();
     tail = substring(s, pos, s.length());
  }
}

/*
 * Expand tabs
 */
template<class str>
str
basic_string_tools<str>::expand_tabs(const str& line, size_t tab_size)
{
  str l = line;
  size_t pos;
  while( (pos = l.find("\t")) != l.npos ) {
    int amount = tab_size - (pos % tab_size);
    l.erase(pos, 1);
    l.insert(pos, amount, ' ');
  }
  return l;
}
#include "TPTools/Utils/TPOptional.h"
#include <type_traits>
#include <string_view>
#include <charconv>

namespace TP
{
	using from_chars_result = std::from_chars_result;
	using chars_format = std::chars_format;

	template<typename T, std::enable_if_t<std::is_integral_v<T>, int> = 0>
	from_chars_result  fromChars(const std::string_view v, T& value, const int base = 10) noexcept
	{
		return std::from_chars(v.data(), v.data() + v.size(), value, base);
	}

	template<typename T, std::enable_if_t<std::is_floating_point_v<T>, int> = 0>
	from_chars_result  fromChars(const std::string_view v, T& value, const chars_format fmt = chars_format::general) noexcept
	{
		return std::from_chars(v.data(), v.data() + v.size(), value, fmt);
	}


	template<typename T, std::enable_if_t<std::is_integral_v<T>, int> = 0>
	Optional<T> fromStringView(const std::string_view v, const int base = 10)
	{
		T value;
		auto [pc, err] = fromChars(v, value, base);
		return err == std::errc() ? makeOptional(value) : std::nullopt;
	}

	template<typename T, std::enable_if_t<std::is_floating_point_v<T>, int> = 0>
	Optional<T> fromStringView(const std::string_view v, const chars_format fmt = chars_format::general)
	{
		T value;
		auto[pc, err] = fromChars(v, value, fmt);
		return err == std::errc() ? makeOptional(value) : std::nullopt;
	}

	template<typename T>
	bool tryCast(const std::string& source, T& value)
	{
		try {
			value = boost::lexical_cast<T>(source);
			return true;
		}
		catch (...)
		{
			return false;
		}
	}

	inline void trimQuote(std::string& value)
	{
		if (!value.empty() && value[0] == '\'')
		{
			value = value.substr(1);
		}
		if (!value.empty() && value[value.size() - 1] == '\'')
		{
			value = value.substr(0, value.size() - 1);
		}

	}


	template<typename T>
	std::optional<std::vector<T>> vectorFromString(const std::string& data)
	{
		if (!data.empty())
		{
			std::vector<std::string> res;
			boost::algorithm::split(res, data, boost::algorithm::is_any_of(","));
			std::vector<T> result;

			for (auto& tk : res)
			{
				trimQuote(tk);
				T value;
				if (tryCast<T>(tk, value))
				{
					result.emplace_back(value);
				}
				else
				{
					return std::optional<std::vector<T>>();
				}
			}
		}

		return std::optional<std::vector<T>>();
	}
}
#endif
