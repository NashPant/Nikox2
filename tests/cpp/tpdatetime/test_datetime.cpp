#include <gtest/gtest.h>
#include "TPDateTime/datetime.h"

//#include <TPTools/Time/TPDate.h>


using namespace TP::date_time;

class DATETIMETEST : public ::testing::Test
{
protected:
	void SetUp() override
	{
		// Code here will be called immediately after the constructor (right
		// before each test).
	}
	void TearDown() override
	{
		// Code here will be called immediately after each test (right
		// before the destructor).
	}
	// Objects declared here can be used by all tests in the test suite for Foo.
};

//TEST_F(DATETIMETEST, ConstructionTest)
//{
//	Year myy{ 2017y };
//	Month m{ Jan };
//	Day d = 15d;
//	Date dt(2017y, January, 17d);
//
//	Date d2 = 2017y / Jan / last;
//
//}



TEST_F(DATETIMETEST, TestZonedTime)
{
	DateTimeLocal datetime = get_datetime_local(2025, 5, 7, 10, 37, 0);
	ZonedTime zoned_time("America/New_York", datetime);
	DateTime datetime_utc = zoned_time.get_sys_time();

	EXPECT_EQ(datetime_utc, get_datetime(2025, 5, 7, 14, 37, 0));
	Date date = get_date(2025, 5, 7); 
	ZonedDate zoned_date("UTC", date.to_sys_days());
	DateLocal date_local =	std::chrono::floor<std::chrono::days>(zoned_date.get_local_time());
	EXPECT_EQ(date_local, get_date_local(2025, 5, 7));


	DateTime datetime2 = get_datetime(2025, 5, 7, 2, 0, 0);
	ZonedTime zoned_datetime2("America/New_York", datetime2);
	DateLocal date2_local = to_date_local(zoned_datetime2.get_local_time());
	EXPECT_EQ(date2_local, get_date_local(2025, 5, 6));
}

	//TEST(TestDateTime, TestDateStringConversion)
	//{
	//	const std::string date_str= 	"2025-05-07";
	//utils::Date date = utils::get_date(2025, 5, 7);
	//const std::string datetime_str
	//	=
	//	"2025-05-07 10:37:13";
	//utils::DateTime datetime = utils::get_datetime(2025, 5, 7, 10, 37, 13);
	//EXPECT_EQ(utils::to_string(date), date_str); EXPECT_EQ(utils::to_string(datetime), datetime_str); EXPECT_EQ(utils::date_from_string(date_str), date); EXPECT_EQ(utils::datetime_from_string(datetime_str), datetime);
	//const std::string date_str_2 = "2025-11-24"; utils::Date date_2 = utils::get_date(2025, 11, 24); const std::string datetime_str_2 = "2025-10-17 05:04:02"; utils::DateTime datetime_2 = utils::get_datetime(2025, 10, 17, 5, 4, 2);
	//EXPECT_EQ(utils::to_string(date_2), date_str_2); EXPECT_EQ(utils::to_string(datetime_2), datetime_str_2); EXPECT_EQ(utils::date_from_string(date_str_2), date_2); EXPECT_EQ(utils::datetime_from_string(datetime_str_2), datetime_2);
	//TEST(TestDateTime, TestOperators)
	//{
	//	utils::Date date_2 = utils::get_date(2025, 11, 24);
	//	auto new_dt = utils::get_date(2025, 11, 25);
//		EXPECT_EQ(++date_2, new_dt);