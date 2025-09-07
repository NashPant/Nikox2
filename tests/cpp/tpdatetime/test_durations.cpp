#include <gtest/gtest.h>
#include <sstream>
#include "TPTools/Utils/Formatting.h"
#include "TPTools/Utils/Operators.h"
#include "TPDatetime/Durations.h" // Assuming the provided code is in Duration.h

namespace TP::date_time {

    class DurationTest : public ::testing::Test {
    protected:
        void SetUp() override {}
    };

    // Test Duration constructors
    TEST_F(DurationTest, DefaultConstructor) {
        Duration d;
        EXPECT_TRUE(d.isNull());
        EXPECT_EQ(d.years(), Years(0));
        EXPECT_EQ(d.months(), Months(0));
        EXPECT_EQ(d.weeks(), Weeks(0));
        EXPECT_EQ(d.days(), Days(0));
    }

    TEST_F(DurationTest, YearsConstructor) {
        Duration d(Years(2));
        EXPECT_FALSE(d.isNull());
        EXPECT_EQ(d.years(), Years(2));
        EXPECT_EQ(d.months(), Months(0));
        EXPECT_EQ(d.weeks(), Weeks(0));
        EXPECT_EQ(d.days(), Days(0));
        EXPECT_TRUE(d.is<Years>());
        EXPECT_TRUE(d.is<Months>());
        EXPECT_FALSE(d.is<Weeks>());
        EXPECT_FALSE(d.is<Days>());
    }

    TEST_F(DurationTest, MonthsConstructor) {
        Duration d(Months(15));
        EXPECT_FALSE(d.isNull());
        EXPECT_EQ(d.years(), Years(1));
        EXPECT_EQ(d.months(), Months(3));
        EXPECT_EQ(d.weeks(), Weeks(0));
        EXPECT_EQ(d.days(), Days(0));
        EXPECT_FALSE(d.is<Years>());
        EXPECT_TRUE(d.is<Months>());
        EXPECT_FALSE(d.is<Weeks>());
        EXPECT_FALSE(d.is<Days>());
    }

    TEST_F(DurationTest, WeeksConstructor) {
        Duration d(Weeks(3));
        EXPECT_FALSE(d.isNull());
        EXPECT_EQ(d.years(), Years(0));
        EXPECT_EQ(d.months(), Months(0));
        EXPECT_EQ(d.weeks(), Weeks(3));
        EXPECT_EQ(d.days(), Days(0));
        EXPECT_FALSE(d.is<Years>());
        EXPECT_FALSE(d.is<Months>());
        EXPECT_TRUE(d.is<Weeks>());
        EXPECT_TRUE(d.is<Days>());
    }

    TEST_F(DurationTest, DaysConstructor) {
        Duration d(Days(10));
        EXPECT_FALSE(d.isNull());
        EXPECT_EQ(d.years(), Years(0));
        EXPECT_EQ(d.months(), Months(0));
        EXPECT_EQ(d.weeks(), Weeks(1));
        EXPECT_EQ(d.days(), Days(3));
        EXPECT_FALSE(d.is<Years>());
        EXPECT_FALSE(d.is<Months>());
        EXPECT_FALSE(d.is<Weeks>());
        EXPECT_TRUE(d.is<Days>());
    }

    //// Test equality operators
    //TEST_F(DurationTest, EqualityWithDuration) {
    //    Duration d1(Months(2), Days(3));
    //    Duration d2(Months(2), Days(3));
    //    Duration d3(Months(1), Days(4));
    //    EXPECT_TRUE(d1 == d2);
    //    EXPECT_FALSE(d1 == d3);
    //}

    TEST_F(DurationTest, EqualityWithYears) {
        Duration d(Years(2));
        EXPECT_TRUE(d == Years(2));
        EXPECT_FALSE(d == Years(3));
    }

    TEST_F(DurationTest, EqualityWithMonths) {
        Duration d(Months(5));
        EXPECT_TRUE(d == Months(5));
        EXPECT_FALSE(d == Months(6));
    }

    TEST_F(DurationTest, EqualityWithWeeks) {
        Duration d(Weeks(4));
        EXPECT_TRUE(d == Weeks(4));
        EXPECT_FALSE(d == Weeks(5));
    }

    TEST_F(DurationTest, EqualityWithDays) {
        Duration d(Days(7));
        EXPECT_TRUE(d == Days(7));
        EXPECT_FALSE(d == Days(8));
    }

    //// Test arithmetic operators
    //TEST_F(DurationTest, Addition) {
    //    Duration d1(Months(2), Days(3));
    //    Duration d2(Months(1), Days(4));
    //    d1 += d2;
    //    EXPECT_EQ(d1.months(), Months(3));
    //    EXPECT_EQ(d1.days(), Days(7));
    //}

    //TEST_F(DurationTest, Subtraction) {
    //    Duration d1(Months(3), Days(5));
    //    Duration d2(Months(1), Days(2));
    //    d1 -= d2;
    //    EXPECT_EQ(d1.months(), Months(2));
    //    EXPECT_EQ(d1.days(), Days(3));
    //}

    //TEST_F(DurationTest, Multiplication) {
    //    Duration d(Months(2), Days(3));
    //    d *= 2;
    //    EXPECT_EQ(d.months(), Months(4));
    //    EXPECT_EQ(d.days(), Days(6));
    //}

    //TEST_F(DurationTest, Negation) {
    //    Duration d(Months(2), Days(3));
    //    Duration neg = -d;
    //    EXPECT_EQ(neg.months(), Months(-2));
    //    EXPECT_EQ(neg.days(), Days(-3));
    //}

    // Test conversion to Days
    //TEST_F(DurationTest, ConversionToDays) {
    //    Duration d(Weeks(2)); // 14 days
    //    Days days = static_cast<Days>(d);
    //    EXPECT_EQ(days, Days(14));

    //    Duration d2(Months(1), Days(3));
    //    EXPECT_THROW(static_cast<Days>(d2), std::invalid_argument); // Should throw due to months
    //}

    //// Test conversion to Months
    //TEST_F(DurationTest, ConversionToMonths) {
    //    Duration d(Years(1)); // 12 months
    //    Months months = static_cast<Months>(d);
    //    EXPECT_EQ(months, Months(12));

    //    Duration d2(Months(2), Days(3));
    //    EXPECT_THROW(static_cast<Months>(d2), std::invalid_argument); // Should throw due to days
    //}

    //// Test dcf function
    //TEST_F(DurationTest, DcfCalculation) {
    //    Duration d(Months(6), Days(30));
    //    double expected = (6.0 / 12.0) + (30.0 / 365.25); // 0.5 years + ~0.082 days
    //    EXPECT_NEAR(dcf(d), expected, 1e-6);
    //}

    //// Test Formatting for Duration and related types
    //TEST_F(DurationTest, FormatDays) {
    //    Formatter<Days> formatter;
    //    Days d(5);
    //    EXPECT_EQ(formatter.str(d), "5D");

    //    auto parsed = formatter.tryParse("5D");
    //    ASSERT_TRUE(parsed.has_value());
    //    EXPECT_EQ(parsed.value(), Days(5));

    //    parsed = formatter.tryParse("invalid");
    //    EXPECT_FALSE(parsed.has_value());
    //    EXPECT_EQ(formatter.failToParse("invalid"), "Failed to parse invalid as Days");
    //}

    //TEST_F(DurationTest, FormatWeeks) {
    //    Formatter<Weeks> formatter;
    //    Weeks w(3);
    //    EXPECT_EQ(formatter.str(w), "3W"); // Assuming str formats as "3W"

    //    auto parsed = formatter.tryParse("3W");
    //    ASSERT_TRUE(parsed.has_value());
    //    EXPECT_EQ(parsed.value(), Weeks(3));

    //    parsed = formatter.tryParse("invalid");
    //    EXPECT_FALSE(parsed.has_value());
    //    EXPECT_EQ(formatter.failToParse("invalid"), "Failed to parse invalid as Weeks");
    //}

    //TEST_F(DurationTest, FormatMonths) {
    //    Formatter<Months> formatter;
    //    Months m(4);
    //    EXPECT_EQ(formatter.str(m), "4M"); // Assuming str formats as "4M"

    //    auto parsed = formatter.tryParse("4M");
    //    ASSERT_TRUE(parsed.has_value());
    //    EXPECT_EQ(parsed.value(), Months(4));

    //    parsed = formatter.tryParse("invalid");
    //    EXPECT_FALSE(parsed.has_value());
    //    EXPECT_EQ(formatter.failToParse("invalid"), "Failed to parse invalid as Months");
    //}

    //TEST_F(DurationTest, FormatYears) {
    //    Formatter<Years> formatter;
    //    Years y(2);
    //    EXPECT_EQ(formatter.str(y), "2Y"); // Assuming str formats as "2Y"

    //    auto parsed = formatter.tryParse("2Y");
    //    ASSERT_TRUE(parsed.has_value());
    //    EXPECT_EQ(parsed.value(), Years(2));

    //    parsed = formatter.tryParse("invalid");
    //    EXPECT_FALSE(parsed.has_value());
    //    EXPECT_EQ(formatter.failToParse("invalid"), "Failed to parse invalid as Years");
    //}

    //TEST_F(DurationTest, FormatDuration) {
    //    Formatter<Duration> formatter;
    //    Duration d(Months(2), Days(3));
    //    EXPECT_EQ(formatter.str(d), "2M3D"); // Assuming str formats as "2M3D"

    //    auto parsed = formatter.tryParse("1Y2M3D");
    //    ASSERT_TRUE(parsed.has_value());
    //    Duration expected(Years(1));
    //    expected += Duration(Months(2));
    //    expected += Duration(Days(3));
    //    EXPECT_EQ(parsed.value(), expected);

    //    parsed = formatter.tryParse("invalid");
    //    EXPECT_FALSE(parsed.has_value());
    //    EXPECT_EQ(formatter.failToParse("invalid"), "Failed to parse invalid as Duration");
    //}

    // Test ostream output
    TEST_F(DurationTest, OstreamDays) {
        std::ostringstream oss;
        Days d(5);
        oss << d;
        EXPECT_EQ(oss.str(), "5D");
    }

    TEST_F(DurationTest, OstreamWeeks) {
        std::ostringstream oss;
        Weeks w(3);
        oss << w;
        EXPECT_EQ(oss.str(), "3W"); // Assuming Weeks formats as "3W"
    }

    TEST_F(DurationTest, OstreamMonths) {
        std::ostringstream oss;
        Months m(4);
        oss << m;
        EXPECT_EQ(oss.str(), "4M"); // Assuming Months formats as "4M"
    }

    TEST_F(DurationTest, OstreamYears) {
        std::ostringstream oss;
        Years y(2);
        oss << y;
        EXPECT_EQ(oss.str(), "2Y"); // Assuming Years formats as "2Y"
    }

} // namespace TP::date_time
