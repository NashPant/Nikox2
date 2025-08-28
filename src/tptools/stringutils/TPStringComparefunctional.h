
#ifndef TP_STRINGCOMPARESFUNCTIONAL_0381038013840830
#define TP_STRINGCOMPARESFUNCTIONAL_0381038013840830

#include "TPCompare.h" 

#include <string>
#include <functional>
#include <algorithm>
#include <cctype>

namespace TP
{
        namespace Text
        {

			/*
			*  Case insesitive less predicate for strings
			*/
			  struct ILess
			{
				bool operator()(const std::string& x, const std::string& y) const
				{
					return std::lexicographical_compare(x.begin(), x.end(),y.begin(), y.end(), less);
				}
			private:
				static bool less(const unsigned char x, const unsigned char y)
				{
					static const std::less<> comparer;
					return comparer(std::toupper(x), std::toupper(y));

				}
			};
			/*
		  *  Case insesitive equal predicate for strings
		 */
		  struct IEQuals
			{
				bool operator()(const std::string& x, const std::string& y) const
				{
					return std::lexicographical_compare(x.begin(), x.end(), y.begin(), y.end(), equals);
				}
			private:
				static bool equals(const unsigned char x, const unsigned char y)
				{
					return  std::toupper(x) ==std::toupper(y);

				}
			};


			/**
            struct CaseAndWhitespaceInsensitiveEqual
                : std::binary_function<std::string, std::string, bool>
            {
                bool operator()(const std::string& x1, const std::string& x2) const
                {
                    return 0 == trimmedCaseInsensitiveCompare(x1.c_str(), x2.c_str());
                }
            };

            template < typename StringType >
            struct CaseAndWhitespaceInsensitiveLessThan
                : std::binary_function< StringType, StringType, bool>
            {
                bool operator()(const StringType& x1, const StringType& x2) const
                {
                    return 0 > trimmedCaseInsensitiveCompare(x1.c_str(), x2.c_str());
                }
            };

            struct CaseAndWhitespaceInsensitiveHash
                : std::unary_function<std::string, std::size_t>
            {
                std::size_t operator()(const std::string& x) const
                {
                    return caseAndWhitespaceInsensitiveHash(x.c_str());
                }
            };
			*/

        }
}

#endif // BASIC_TEXT_FUNCTIONAL_HPP
