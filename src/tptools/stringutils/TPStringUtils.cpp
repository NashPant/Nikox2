#include "../TPGlobals.h"
#include "TPStringUtils.h"


#include <algorithm>



const std::string FIRST_CHAR_LIST = "-+.0123456789";
const std::string OTHER_CHAR_LIST = ".0123456789";


// From a list of vector remove dublicates and empty strings
std::vector<std::string> TPStringUtils::removeduplicatesAndEmtyStrings(const std::vector<std::string>& myvector)
{
    //make a copy
    std::vector<std::string> myuniquelist(myvector);
    std::sort(myuniquelist.begin(), myuniquelist.end());
    // Keep only unique Elements
    std::unique(myuniquelist.begin(), myuniquelist.end());
    //remove empty string
    // let's split it up to see what it does
    // remove empty string
    myuniquelist.erase(std::remove_if(myuniquelist.begin(),
        myuniquelist.end(),
        [](std::string const& s) { return s.empty() || s.size() ==0 || s==""; }),
        myuniquelist.end());

    return myuniquelist;

}

// Boost Replace all string function implementation
void TPStringUtils::replaceAll(std::string& s, const std::string &search, const std::string &replace)
{
	size_t pos = 0;
	while ((pos = s.find(search, pos)) != std::string::npos) {
		s.replace(pos, search.length(), replace);
		pos += replace.length(); // Move past the replaced substring
	}
}


// Split Left and split right

std::string TPStringUtils::split(const std::string& str, const std::string& needle, 
                                            const std::optional<bool>& blLeft)
{
    std::string res;
    if (blLeft.value_or(true))
    {
        res = str.substr(0, str.find(needle));
    }
    else
    {
        res = str.substr( str.find(needle)+needle.size());
    }
    return res;
}

/**
 * Convert the std::string s to uppercase and return it
 */
std::string& TPStringUtils::toUpper( std::string& s )
{
   for( int i=0; i<s.size(); i++ )
   {
      s[i] = toupper( s[i] );
   }

   return( s );
}


/**
 * Convert the std::string s to lowercase and return it
 */
std::string& TPStringUtils::toLower( std::string& s )
{
   for( int i=0; i<s.size(); i++ )
   {
      s[i] = tolower( s[i] );
   }

   return( s );
}


/**
 * Strip out blank characters of the std::string s and return it
 */
std::string& TPStringUtils::noSpace( std::string& s )
{
   std::string::iterator i=s.begin();

   while( i != s.end() )
   {
      if ( isspace( *i ) )
      {
         s.erase( i );
      }
      else
      {
         i++;
      }
   }

   return( s );
}


/**
 * Strip out trailing blank characters of the std::string s and return it
 */
std::string& TPStringUtils::noTrailingSpace( std::string& s )
{
   std::string::size_type pos = s.find_last_not_of( ' ' );
   pos++;
   s.erase( pos );

   return( s );
}


/**
 * Strip out leading blank characters of the std::string s and return it
 */
std::string& TPStringUtils::noLeadingSpace( std::string& s )
{
   std::string::size_type pos = s.find_first_not_of( ' ' );
   pos;
   s.erase( 0, pos );

   return( s );
}


/**
 * Check if the std::string s is blank
 */
bool TPStringUtils::isBlank( const std::string& s )
{
   return( s.find_first_not_of( ' ' ) == std::string::npos );
}


std::string TPStringUtils::itos( int n )
{
   return( ToString( (int)n ) );
}

/*
std::string TPStringUtils::dtos( double d )
{
   return( ToString( d ) );
}
*/

int TPStringUtils::nocasecmp( const std::string& s1, const std::string& s2 )
{
   std::string::const_iterator p1 = s1.begin();
   std::string::const_iterator p2 = s2.begin();

   while( ( p1 != s1.end() ) && ( p2 != s2.end() ) )
   {
      if ( toupper( *p1 ) != toupper( *p2 ) )
      {
         return( ( toupper( *p1 ) < toupper( *p2 ) ) ? -1 : 1 );
      }
      p1++;
      p2++;
   }

   return( ( s2.size() == s1.size() ) ? 0 : ( s1.size()<s2.size() ) ? -1 : 1 );
}



void TPStringUtils::tok( const std::string& s, char sep, 
    std::list<std::string>& tokens )
{
   tokens.clear();

   std::string::size_type len = 1;
   std::string::size_type p1 = 0;
   std::string::size_type p2 = s.find( sep );

   tokens.push_back( s.substr( p1, p2-p1 ) );

   while ( p2 != std::string::npos )
   {
      p1 = p2 + len;
      p2 = s.find( sep, p1 );
      tokens.push_back( s.substr( p1, p2-p1 ) );
   }
}



void TPStringUtils::tok( const std::string& s, const std::string& sep, 
    std::list<std::string>& tokens )
{
   tokens.clear();

   std::string::size_type len = sep.size();
   std::string::size_type p1 = 0;
   std::string::size_type p2 = s.find( sep );
#ifdef _DEBUG
	std::string pipa(s.substr( p1, p2-p1 ));
#endif
   tokens.push_back( s.substr( p1, p2-p1 ) );

   while ( p2 != std::string::npos )
   {
      p1 = p2 + len;
      p2 = s.find( sep, p1 );
#ifdef _DEBUG
		std::string pipa(s.substr( p1, p2-p1 ));
#endif
      tokens.push_back( s.substr( p1, p2-p1 ) );
   }
}


double TPStringUtils::atof( const std::string& str )
{
   if ( !isNumeric( str ) )
   {
      THROW( "TPStringUtils::atof(: Invalid numeric value : ")
   }

   const char* tmpSpr = str.c_str();
   int pos = 0;
   double sign = 1.0;

   if ( str[0] == '+' )
   {
      pos = 1;
   }
   else if ( str[0] == '-' )
   {
      pos = 1;
      sign = -1.0;
   }

   return sign * atof( &tmpSpr[pos] );
}


bool TPStringUtils::isNumeric( const std::string& str )
{
   if ( str.length() == 0 )
   {
      return false;
   }

   size_t n = 0;

#ifdef TPSTATIC
   n = count( str.begin(), str.end(), '.');
#else
   n = count(str.begin(), str.end(), '.');
//   count( str.begin(), str.end(), '.', n );
#endif

   if ( ( FIRST_CHAR_LIST.find_first_of( str[0] ) == std::string::npos ) ||
        ( n > 1 ) )
   {
      return false;
   }

   for( int i=1; i<str.length(); i++ )
   {
      if ( OTHER_CHAR_LIST.find_first_of( str[i] ) == std::string::npos )
      {
         return false;
      }
   }

   return true;
}

std::string toUpper(const std::string& strIn)
{
    std::string rtn(strIn);
    
    std::string::iterator s = rtn.begin(), e = rtn.end();
    
    // loop through each individual character
    while(s != e)
    {
        (*s) = toupper(*s);
        ++s;
    }
    
    return rtn;
}


//////////
//	Will search std::string "tobesearched" and if std::string "tobefound" is found will return true.
//   Otherwise wil return false.
//
bool	TPStringUtils::contains(const std::string& tobesearched, const std::string& tobefound, bool matchlength )
{
	if (matchlength)
	{
		return (tobesearched.find(tobefound.c_str(), 0, tobefound.length()) != std::string::npos);
	}
	else
	{
		return (tobesearched.find(tobefound) != std::string::npos);
	}
}


//////////
//	Will remove all occurences of the std::string "toberemoved" from the std::string "tobesearched "
//
//   if casesensitive =TRUE, case sensitive search is done(default)
//   otherwise make everything lowercase and do the same thing.
void	TPStringUtils::TPEraseAllSubstrings(std::string& tobesearched, std::string& toberemoved,
																				const bool& casesensitive)
{
	    if(!casesensitive)
		{
			toLower(tobesearched);
			toLower(toberemoved);
		}
		size_t pos = tobesearched.find(toberemoved);

		 int ni = toberemoved.size();
		 while(pos < std::string::npos)
		 {
			 tobesearched.erase(pos,ni);
			 pos = tobesearched.find(toberemoved);

		 }
}

namespace
{
#ifdef __GNUG__
	std::string demangle(const char* name)
	{
		std::string result;
		int status = -4; // some arbitrary value to eliminate the compiler warning
		size_t length = 1024;
		char* str = static_cast<char*>(std::malloc(length));
		if (str)
		{
			str[0] = 0;
			abi::__cxa_demangle(name, str, &length, &status);
			if (status == 0)
				result = str;
			free(str);
		};
		if (result.empty())
			result = name;
		return result;
	}
#else
	// does nothing if not g++
	std::string demangle(const char* name)
	{
		return name;
	}
#endif
}

std::string TPStringUtils::stripTypeName(const std::type_info& type)
{
	// We trim everything before the last space
	const std::string& typeName = demangle(type.name());
	const char* lastColonOrSpace;
	const char* endOfTypeName;

	{
		const char find1 = ':';
		const char find2 = ' ';
		const char eos = '<';
		const char* const str = typeName.c_str();

		// Finds the last occurrence of find1 or find2 before the end of the string or any occurrence of the end-of-string character (eos)
		const char* found = nullptr;
		const char* i = str;
		while (*i != 0 && *i != eos)
		{
			if (*i == find1 || *i == find2)
				found = i;
			++i;
		}

		lastColonOrSpace = (found) ? found + 1 : str;
		endOfTypeName = i;
	}

	return std::string(lastColonOrSpace, endOfTypeName);
}

//bool TPStringUtils::sameSplit(const std::string& s1, const std::string& s2, char sep)
//{
//	std::vector<std::string> s1_v, s2_v;
//	boost::split(s1_v, TP::stringutils::toLower(s1), boost::is_any_of(sep + " "), boost::token_compress_on);
//	boost::split(s2_v, TP::stringutils::toLower(s2), boost::is_any_of(sep + " "), boost::token_compress_on);
//
//	return s1_v.size() == s2_v.size() && std::is_permutation(s1_v.begin(), s1_v.end(), s2_v.begin());
//}


int TPStringUtils::str_to_int(const char* str, char thousands_separator)
{
	// convert a string to a float
	int result = 0;
	int sign = *str == '-' ? static_cast<void>(str++), -1 : 1;
	while ((*str >= '0' && *str <= '9') || (*str == thousands_separator)) {
		if (*str == thousands_separator) {
			str++;
			continue;
		}
		result *= 10;
		result += *str - '0';
		str++;
	}
	result *= sign;
	return result;
}

float TPStringUtils::str_to_float(const char* str, char decimal_point, char thousands_separator) {
	// convert a string to a float
	float result = 0;
	float sign = *str == '-' ? static_cast<void>(str++), -1.0f : 1.0f;
	while ((*str >= '0' && *str <= '9') || (*str == thousands_separator)) {
		if (*str == thousands_separator) {
			str++;
			continue;
		}
		result *= 10;
		result += *str - '0';
		str++;
	}

	float multiplier = 0.1f;
	if (*str == decimal_point) {
		str++;
		while (*str >= '0' && *str <= '9') {
			result += (*str - '0') * multiplier;
			multiplier /= 10;
			str++;
		}
	}

	float power = 0.0f;
	result *= sign;
	if (*str == 'e' || *str == 'E') {
		str++;
		float powerer = *str == '-' ? static_cast<void>(str++), 0.1f : 10.0f;

		while ((*str >= '0') && (*str <= '9')) {
			power *= 10;
			power += *str - '0';
			str++;
		}
		result *= pow(powerer, power);
	}
	return result;
}


std::string TPStringUtils::removeDigits(const std::string& s)
{
	std::string source(s);
	source.erase(std::remove_if(source.begin(), source.end(),
		[](char c) { return std::isdigit(c); }),
		source.end());

	return source;
}

