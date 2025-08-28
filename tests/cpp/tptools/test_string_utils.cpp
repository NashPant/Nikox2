#include <gtest/gtest.h>
#include <type_traits>
#include "tptools/stringutils/TPStringUtils.h"

using namespace TP::stringutils;



// Test case for string with only whitespace
TEST(TrimCopyTest, HandlesOnlyWhitespace) 
{
    std::string input = "   \t\n\r ";
	auto ss = trim_copy_std(input);
    EXPECT_EQ(trim_copy_std(input), "");
}


// Test case for string with leading whitespace
TEST(TrimCopyTest, HandlesLeadingWhitespace) {
    std::string input = "   Hello";
    EXPECT_EQ(trim_copy_std(input), "Hello");
}

// Test case for string with trailing whitespace
TEST(TrimCopyTest, HandlesTrailingWhitespace2) {
    std::string input = "Hello   ";
    EXPECT_EQ(trim_copy_std(input), "Hello");
}

// Test case for string with leading and trailing whitespace
TEST(TrimCopyTest, HandlesLeadingAndTrailingWhitespace) {
    std::string input = "   Hello World   ";
    EXPECT_EQ(trim_copy_std(input), "Hello World");
}

// Test case for string with no whitespace
TEST(TrimCopyTest, HandlesNoWhitespace) {
    std::string input = "HelloWorld";
    EXPECT_EQ(trim_copy_std(input), "HelloWorld");
}

// Test case for string with internal whitespace
TEST(TrimCopyTest, PreservesInternalWhitespace) {
    std::string input = "   Hello   World   ";
    EXPECT_EQ(trim_copy_std(input), "Hello   World");
}

// Test case for single character string
TEST(TrimCopyTest, HandlesSingleCharacter) {
    std::string input = " a ";
    EXPECT_EQ(trim_copy_std(input), "a");
}