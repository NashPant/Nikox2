#if 1
#include <gtest/gtest.h>
#include "TPDateTime/Date.h"

//#include <TPTools/Time/TPDate.h>


using namespace TP::date_time;

TEST(NEWDATETEST, ConstructionTest)
{
	Year myy{ 2017y };
	Month m{ Jan };
	Day d = 15d;
	Date dt(2017y, January, 17d);

	Date d2 = 2017y / Jan / last;

}
/*
TEST(NEWDATETEST, TimeZoneTest)
{
	auto d1 = date::zoned_time{ "Asia/Shanghai",std::chrono::system_clock::now() };
	auto d1dt = Date(d1.get_sys_time());
	//std::cout << date::zoned_time{ "Asia/Shanghai",
	//							  std::chrono::system_clock::now() } << '\n';
	using namespace std::chrono;
	//using namespace date;
	//auto ny= date::zoned_time{ "America/New_York", date::local_days{July / 8 / 2016} +8h };
//	zoned_time moscow{ "Europe/Moscow", ny };
}
*/

TEST(NEWDATETEST, buildsValid)
{
	ASSERT_NO_THROW(Date(2000y, January, 31d));
	ASSERT_NO_THROW(Date(2000y / January / 31d));
	ASSERT_NO_THROW(Date(2000y / February / 29d));
	ASSERT_NO_THROW(Date(2000y / August / last));
	ASSERT_NO_THROW(Date(2000y / January / Sunday[3]));
	ASSERT_NO_THROW(Date(2019y / September / Monday[5]));
	ASSERT_NO_THROW(Date(2000y / March / Sunday[last]));
}

TEST(NEWDATETEST, failsBuildInvalid)
{
	ASSERT_THROW(Date(2000y, January, 32d), std::invalid_argument);
	ASSERT_THROW(Date(2000y / January / 32d), std::invalid_argument);
	ASSERT_THROW(Date(2002y / February / 29d), std::invalid_argument);
	ASSERT_THROW(Date(2000y / Month{ 20 } / last), std::invalid_argument);
	ASSERT_THROW(Date(2000y / January / Sunday[6]), std::invalid_argument);
	ASSERT_THROW(Date(2000y / January / Friday[5]), std::invalid_argument);
	ASSERT_THROW(Date(2000y / Month{ 13 } / Sunday[last]), std::invalid_argument);
}


TEST(NEWDATETEST, parseValid)
{

	const Date d(2000y, January, 31d);

	ASSERT_EQ(d, Date("20000131"));
	ASSERT_EQ(d, Date("2000-01-31", "%Y-%m-%d"));
	ASSERT_EQ(d, Date("2000/01/31", "%Y/%m/%d"));
	ASSERT_EQ(d, Date("2000-Jan-31", "%Y-%b-%d"));
	//	ASSERT_EQ(d, Date("00-January-31", "%y-%B-%d"));
	ASSERT_EQ(d, Date("Monday January 31, 2000", "%A %B %d, %Y"));
}


TEST(NEWDATETEST, stringConvertValid)
{

	const Date dd(2000y, January, 31d);
	auto s2 = dd.toString("{:%Y-%m-%d}");
	auto s1 = dd.toString();
	

	ASSERT_EQ(dd.toString(), "20000131");
	ASSERT_EQ(dd.toString("{:%Y-%m-%d}"), "2000-01-31");
	ASSERT_EQ(dd.toString("{:%Y/%m/%d}"), "2000/01/31");
	ASSERT_EQ(dd.toString("{:%Y-%b-%d}"), "2000-Jan-31");
	ASSERT_EQ(dd.toString("{:%y-%B-%d}"), "00-January-31");
	ASSERT_EQ(dd.toString("{:%A %B %d, %Y}"), "Monday January 31, 2000");
}



#endif