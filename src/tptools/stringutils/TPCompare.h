#ifndef _TPCOMPARE_2390854672390874752938475239084_
#define _TPCOMPARE_2390854672390874752938475239084_


#include "TPNikopolisUtils.h"

#include <cstddef>

namespace TP
{
        namespace Text
        {
            /// returns s advanced to the first non-whitespace character or the terminating 0.
            /// Throws if the pointer is NULL.
			TPNIKOPOLIS_IMPEXP const char* skipLeadingWhitespaces(
                const char* const s   ///<(I) Input string
            );

            /// Case insensitive compare between two trimmed strings (i.e. leading and trailing blanks removed). 
            /// Ordering of -1,0,+1 is as with wcscmp: lhs<rhs -> -1, lhs==rhs -> 0, lhs>rhs ->+1
			TPNIKOPOLIS_IMPEXP int trimmedCaseInsensitiveCompare(
                const char* lhs
                , const char* rhs
            );

            /// returns a number that can be used by as an hash key in a way not sensitive to case and blanks
			TPNIKOPOLIS_IMPEXP std::size_t caseAndWhitespaceInsensitiveHash(
                const char* s
            );
        }
}


#endif
