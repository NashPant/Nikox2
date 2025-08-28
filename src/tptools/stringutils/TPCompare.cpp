#include "TPCompare.h"
//#include "boost/functional/hash.hpp"
#include "TPTools/Utils/hash_replace.h"
#include "../TPException.h"
#include <cstdlib>
#include <wctype.h>

// #include "boost/algorithm/string/replace.hpp"

namespace TP
{
        namespace Text
        {
            /// returns a number that can be used by as an hash key in a way not sensitive to case and blanks
            std::size_t caseAndWhitespaceInsensitiveHash(
                const char* s
            )
            {
                std::size_t seed = 0;
                const int mbCurMax = MB_CUR_MAX;
                wchar_t wch = 0;
                int chLen = 0;
                while ((chLen = std::mbtowc(&wch, s, mbCurMax)) > 0)
                {
                    s += chLen;
                    if (iswalpha(wch))
                        //boost::hash_combine(seed, towupper(wch));
                        hash_combine(seed, towupper(wch));
                }
                return seed;
            }

            const char* skipLeadingWhitespaces(
                const char* const s
            )
            {
                if (0 == s)
                    THROW("NULL string pointer");
                const char* result = s;
                //lint -e{613}
                while ((*result != 0) && isspace(*result))
                    ++result;
                return result;
            }

            const char* lastNonWhitespace(
                const char* s
            )
            {
                if (0 == s)
                    THROW ("NULL string pointer");
                const char* result = NULL;
                const int mbCurMax = MB_CUR_MAX;
                wchar_t wch = 0;
                int chLen = 0;
                while ((chLen = std::mbtowc(&wch, s, mbCurMax)) > 0)
                {
                    if (!iswspace(wch))
                        result = s;
                    s += chLen;
                }
                return (result == NULL) ? s : result;
            }

            // ------------------------------------------------------------------------------------------------
            /// Case insensitive compare between two trimmed strings (i.e. leading and trailing blanks removed). 
            /// Ordering of -1,0,+1 is as with wcscmp: lhs<rhs -> -1, lhs==rhs -> 0, lhs>rhs ->+1
            int trimmedCaseInsensitiveCompare(
                const char* lhs
                , const char* rhs
            )
            {
                if (lhs == rhs)
                    return 0;
                const char* lhsIt = skipLeadingWhitespaces(lhs);
                const char* lhsEnd = lastNonWhitespace(lhs);
                const char* rhsIt = skipLeadingWhitespaces(rhs);
                const char* rhsEnd = lastNonWhitespace(rhs);
                while ((lhsIt <= lhsEnd) && (rhsIt <= rhsEnd))
                {
                    wchar_t lhc = *lhsIt;
                    lhc = (lhc == L'\\') ? L'/' : towupper(lhc);
                    wchar_t rhc = *rhsIt;
                    rhc = (rhc == L'\\') ? L'/' : towupper(rhc);
                    if (lhc < rhc)
                        return -1;
                    if (lhc > rhc)
                        return +1;
                    ++lhsIt;
                    ++rhsIt;
                }
                // One of two strings has been scanned to the end, so the other is longer
                if (lhsIt <= lhsEnd)
                    return +1;
                if (rhsIt <= rhsEnd)
                    return -1;
                return 0;
            }
        }
}
