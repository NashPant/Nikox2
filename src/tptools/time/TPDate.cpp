#include <TPTools/Time/TPDate.h>
#include "../TPException.h"
#ifndef WIN32
#include <ctime>
#else
#include <stdio.h>
#include <time.h>
#endif
#include <math.h>
#include "TPHolidays.h"
#include <algorithm>
#include <boost/date_time/gregorian/gregorian.hpp>
#include "TPDateTime/Date.h"

TPDate TPDate::InvalidDate_ = TPDate(0);

int TPDate::DaysInMonth_[2][13]= { {0,31,28,31,30,31,30,31,31,30,31,30,31 }, 
                                          {0,31,29,31,30,31,30,31,31,30,31,30,31 } };

int TPDate:: AccumaltiveDaysInYear_[2][13] = { {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365},    
                                      {0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366} };


//ctors
//
//

TPDate	TPDate::AsOfDate()
{

    time_t mytime;
 /*
    struct tm *asofdate;

    time(&mytime);
    asofdate = localtime(&mytime);
*/
    // new way
    time(&mytime);
    struct tm asofdate;
    localtime_s(&asofdate, &mytime);

   // return TPDate(asofdate->tm_mday, asofdate->tm_mon + 1, asofdate->tm_year + 1900);
    return TPDate(asofdate.tm_mday, asofdate.tm_mon + 1, asofdate.tm_year + 1900);
}


TPDate::TPDate(const unsigned day, const unsigned month, const unsigned year)
        :ExcelDate_(0), Day_(day), Month_((TPDate::MonthsEnum)(month)), Year_(year)
{

        //Excel date start at 1900
        
        if (year<1900)
        {
	  THROW("Excel Date year must be 1900 or later")
        }
        
        //1-    years * 365 plus the leap ones
        //2-    add number of days since begining of current year and up to the end of last month
        //3-    add days in current month

        ExcelDate_      = (Year_ - 1900)*365 + (Year_ - 1900 - 1)/4 + 
                        AccumaltiveDaysInYear_[isLeap(Year_)][Month_-1]+        
                        Day_;   
        
        if ((Year_>1900)) ExcelDate_++;
        
        WeekDay_        = (TPDate::DaysEnum) ( ((ExcelDate_-1)%7) + 1);
        
}

TPDate::TPDate(const TP::date_time::Date& d):
	TPDate(static_cast<unsigned>(d.day()), static_cast<unsigned>(d.month()), static_cast<int>(d.year()))
{
}

TPDate::TPDate(const unsigned day, const TPDate::MonthsEnum month, const unsigned year)
{
        *this = TPDate(day, (int) month, year);
}

TPDate::TPDate()
{
        *this = InvalidDate_;
}



TPDate::TPDate(const std::string& str)
{
  if( str.length() != 10 || str.at(4) != '-' || str.at(7) != '-' )
  {
    if(str.length() != 8 || str.at(2) != '/' || str.at(5) != '/' )
      THROW("Bad ISO Date string: YYYY-MM-DD expected")
  }

  if( str.length() == 10 )
  {
    int day = atoi( str.substr(8,2).data() );
    int month = atoi( str.substr(5,2).data() );
    int year = atoi( str.substr(0,4).data() );

    if( day < 1 || day > 31 || month < 1 || month > 12 )    
    {
    THROW("BAD ISO Date String: month and/or day number are out of range")
    }

    *this = TPDate( day, month, year );
  }
  else
  {
    int day = atoi( str.substr(0,2).data() );
    int month = atoi( str.substr(3,2).data() );
    int year = atoi( str.substr(6,2).data() );
    year += 2000;

    if( day < 1 || day > 31 || month < 1 || month > 12 )    
    {
    THROW("BAD ISO Date String: month and/or day number are out of range")
    }

    *this = TPDate( day, month, year );
  }
}


TPDate::TPDate(const TPDate& date)
{
        *this = date;
}


TPDate TPDate::fromGregorianDayNumber(const long gregorianDayNumber)
{
    // simply pass it to the constructor and will take care of it

    TPDate result= TPDate();

    result.ExcelDate_ = gregorianDayNumber - DateOffsets::JULIAN;
    result.regenerate();
    return result;
}


bool TPDate::fromString(const std::string& delimitedString, TPDate& result, const std::locale& loc)
{
    std::stringstream ss(delimitedString);
    ss.imbue(loc);
    boost::gregorian::date dt;
    if (ss >> dt)
    {
        result.ExcelDate_ = dt.day_number() - DateOffsets::JULIAN;
		result.regenerate();
        return true;
    }
    return false;;
}

bool TPDate::fromString(const std::string& delimitedString, TPDate& result)
{
    static const std::vector<std::locale> locales = {
        std::locale(std::locale::classic(), new boost::gregorian::date_input_facet("%Y%m%d")),
        std::locale(std::locale::classic(), new boost::gregorian::date_input_facet("%Y-%m-%d")),
        std::locale(std::locale::classic(), new boost::gregorian::date_input_facet("%m-%d-%Y")),
        std::locale(std::locale::classic(), new boost::gregorian::date_input_facet("%d-%b-%Y"))
    };
    for (const auto& loc : locales)
        if (fromString(delimitedString, result, loc))
            return true;
    return false;
}

void TPDate::addTenor(const TP::Core::TPTenor& tenor, const bool preventsnaptomonthend)
{
	if (tenor.isByMonth())
	{
		addYears(tenor.yearCount());
		addMonths(tenor.monthCount());
	}
	*this += tenor;


}
void TPDate::subtractTenor(const TP::Core::TPTenor& tenor, const bool preventsnaptomonthend )
{
	if (tenor.isByMonth())
	{
		addYears(-static_cast<int>(tenor.yearCount()));
		addMonths(-static_cast<int>(tenor.monthCount()));
	}
*this -= tenor;

}

TPDate& TPDate::operator+=(const TP::Core::TPTenor& tenor)
{
	addYears(tenor.yearCount());
	addYears(tenor.monthCount());
	addYears(tenor.dayCount());
	addYears(tenor.weekCount());


	return *this;
}

TPDate& TPDate::operator-=(const TP::Core::TPTenor& tenor)
{
	addYears(-static_cast<int>(tenor.yearCount()));
	addYears(-static_cast<int>(tenor.monthCount()));
	addYears(-static_cast<int>(tenor.dayCount()));
	addYears(-static_cast<int>(tenor.weekCount()));


	return *this;
}


TPDate::TPDate(const long&      date):ExcelDate_(0)
{
        //Invalid Date ctor
        //
        
        if (date == 0) {}

        //Excel Date ctor
        //
        
        else if (date<250000)
        {
                ExcelDate_      =       date;
                regenerate();
        }

        //Julian Date ctor
        //
        
        else if (date<20000000)
        {

                ExcelDate_      =       date - JULIAN;
                regenerate();
        }
        else
        {
                THROW("Date is out of range")
        }
        
}

void TPDate::regenerate()
{
        
        WeekDay_        = (TPDate::DaysEnum) ( ((ExcelDate_-1)%7) + 1);
        Year_           =     (int)  1900 + (int) (ExcelDate_ - 1)/365.25;

        //construct day and month
        //we first get the number of days since the begining of the year
        
        Day_            =       static_cast<int>(ExcelDate_ -
static_cast<int>((Year_ -1900)*365.25+0.8));

        //then loop through by substracting the total of days within each month
        //from the above value (Day_) until we find a number that is less than
        //the number of days of a certain month.
        //By then we would have found the right number of days
        //and the right month.
        //
        int monthindex=1;
        
        for (int i =1;  Day_ > DaysInMonth_[isLeap(Year_)][i];i++)
        {
                Day_ -= DaysInMonth_[isLeap(Year_)][i];
                monthindex++;
        }

        Month_ = (MonthsEnum) (monthindex);
}

bool    TPDate::isWeekend(void)                 const
{
        return ( (WeekDay_ == SATURDAY) || (WeekDay_ == SUNDAY));

}
bool    TPDate::isMonthEnd(void)                const
{
        return (getNumberOfDaysInMonth(this->getYear(), this->getMonth()) == this->getDayInMonth());
}

int     TPDate::getNumberOfDaysInThisMonth()    const
{
        return getNumberOfDaysInMonth(this->getYear(), this->getMonth());

}


void TPDate::getDMY(unsigned short& day, unsigned short& month, unsigned short& year) const
{
   

    day = getDayInMonth();
    month = getMonth();
    year = getYear();
}

std::string  TPDate::ToString(int index)             const
{
        if (isInvalid()) return "InValid Date";

        char day_string[10], month_string[10], ret_string[12];
        
        if (Day_ <10)
				sprintf_s(day_string, "0%d", Day_);
        else
                sprintf_s(day_string, "%d", Day_);
        if (Month_ < 10)
                sprintf_s(month_string, "0%d", Month_);
        else    
                sprintf_s(month_string, "%d", Month_);

        if (index == 0)         //ISO
        {
                sprintf_s(ret_string, "%d-%s-%s", Year_, month_string, day_string);
                return ret_string;
        }
        else if (index == 1)    // EU
        {
                sprintf_s(ret_string, "%s-%s-%d", day_string, month_string, Year_);
                return ret_string;
        }
        else if (index == 2)            //US
        {
                sprintf_s(ret_string, "%s-%s-%d", month_string, day_string, Year_);
                return ret_string;
        }
		else if (index == 3)            //%Y%m%d
		{
			sprintf_s(ret_string, "%d%s%s", Year_, month_string, day_string);
			return ret_string;
		}
        else
        {
                THROW("Index out of range for TPDate::ToString")
        }
        
        
}


std::string  TPDate::ToISOString(void)       const
{
	return TPDate::ToString(0);
}

std::string  TPDate::ToEURString(void)       const
{
	return TPDate::ToString(1);
}
std::string  TPDate::ToUSAString(void)       const
{
	return TPDate::ToString(2);
}
std::string  TPDate::ToYYYYMMDD(void)               const
{
	return TPDate::ToString(3);
}
TPDate::Day TPDate::dayOfMonthEnd() const
{
	// convert to boost date and use the function
	boost::gregorian::date d(DateOffsets::JULIAN + ExcelDate_);
	return d.end_of_month().day();
}

TPDate  TPDate::eom() const
{
	return TPDate(dayOfMonthEnd(), getMonth(), getYear());
}



int             TPDate::getNumberOfDaysInMonth(const int& y, const int& m)
{
        return DaysInMonth_[isLeap(y)][m];
}
int             TPDate::getDaysToMonthEnd(const int& y, const int& m)
{
        return AccumaltiveDaysInYear_[isLeap(y)][m];
}
int             TPDate::getDaysInYear(const int& y)
{
        return (365 + isLeap(y));
}

bool TPDate::isHoliday(const TPDate& date, TP_City city)
{
	return TPHolidays::isHoliday(date, city) || date.isWeekend();
}

bool TPDate::isHoliday(TP_City city) const
{
	return isHoliday(*this, city);
}


void TPDate::regenerateAfterMonthIncrement()
{

	if (Month_ > 12)
	{	
		if (!(Month_ % 12))
		{
			Year_ += -1 + (Month_ - Month_ % 12) / 12;
			Month_ = MonthsEnum(12);
		}
		else
		{
			Year_ += (Month_ - Month_ % 12) / 12;
     		Month_ = MonthsEnum(Month_ % 12);
		}
	}
	else if (Month_ < 1)
	{
		Year_ += -1 + (Month_ - Month_%12)/12;
		Month_ = MonthsEnum(Month_%12 + 12);
	}

	int newday = (Day_>getNumberOfDaysInMonth(Year_,Month_)) ? getNumberOfDaysInMonth(Year_, Month_) : Day_;

	*this = TPDate(newday, Month_, Year_);
	

}

void TPDate::moveForward(double number, TPPERIODTYPE type)
{
	switch(type)
	{
		case DAY:
		{
			if ( number >= 0.0)
			{
				int i = static_cast<unsigned int>(number);
				operator+=(i);
			}
			else
			{
				int i = static_cast<unsigned int>(- number);
				operator-=(i);
			}
		break;
		}
		case BUSINESSDAY: 
		{
/*			int i = static_cast<unsigned int>(number);

			if ( number >= 0.0)
			{
				while(i)
				{
					operator+=(1);
					while (isHoliday(city) || isWeekend()) operator+=(1);
					i--;
				}


			}
			else
			{
				while(i)
				{
					operator-=(1);
					while (isHoliday(city) || isWeekend()) operator-=(1);
					i--;
				}
			}
			
*/			break;

		}
		case WEEK:
		{
			if ( number >= 0.0)
			{
				int i = 7*static_cast<unsigned int>(number);
				operator+=(i);
			}
			else
			{
				int i = 7*static_cast<unsigned int>(- number);
				operator-=(i);
			}
		break;
		}
		case MONTH:
		{
			if ( number >= 0.0)
			{
				int i = static_cast<unsigned int>(number);
				Month_ = (MonthsEnum)(Month_ + i);
				regenerateAfterMonthIncrement();
			}
			else
			{
				int i = static_cast<unsigned int>(- number);
				Month_ = (MonthsEnum)(Month_ - i);
				regenerateAfterMonthIncrement();
			}
		break;
		}
		case YEAR:
		{
			{
				//will allow to add fraction of years.
				//not allowed for months and weeks

				//member variables to be created for the new date

				int newday = getDayInMonth();
				int newmonth = getMonth();
				int newyear = getYear();

				//get integer number of years and sign

				int wholeyears = static_cast<int>(number);
				newyear += wholeyears;
				int direction = (wholeyears<0)? -1 : 1;

				//we needs to make sure that any year fraction that is a day or bigger is taken into account.
				// In a 365 days year, 1 day is a fraction equal to : 0.002739 and to 0.0027322 in a leap year

				double fraction = number - floor(number);
				if (direction == -1) fraction = number - ceil(number);


				double ayear = 366.0;

				if (!isLeap(newyear)) ayear = 365.0;
				int extradays = 0;
			
				if (fabs(fraction) > 0.00273) //i.e. we meed to add year fraction
				{
					//TODO: we might have some rounding issues here due to positine and negative increments. 

					extradays	= static_cast<int>(fraction*ayear);
				}
				//check for 29th of feb on non leap year
				if (!isLeap(newyear) && newmonth == 2 && newday ==29) newday = 28;
				
				*this = TPDate( TPDate(newday,newmonth,newyear).getExcelDate() + extradays);




			}
		break;
		}
		default:
		{
			THROW("Can not shift dates")
		}

	}


}

TPDate TPDate::moveForward(const TPDate& date, double number, TPPERIODTYPE type)
{
	TPDate tmp = date;
	tmp.moveForward(number, type);
	return tmp;
}

void TPDate::adjustBusinessDays(TPDate& date, const TPRolling Rol, TP_City city)
{

	switch(Rol)
	{
	case FOLLOWING:
		while (date.isHoliday(city) || date.isWeekend()) date++;
		break;
	case MODIFIEDFOLLOWING:
  case FRN:
		{
			int increment = 1;
			TPDate::MonthsEnum oldmonth = date.getMonth();
			while (date.isHoliday(city)|| date.isWeekend())
			{
				date += increment;
				if (date.getMonth() != oldmonth)
				{
					increment = -1;
					date -= 2;
				}
			}
			break;
		}
	case PREVIOUS:
		while (date.isHoliday(city) || date.isWeekend()) date--;
		break;
	case MODIFIEDPREVIOUS:
		{
			int increment = -1;
			TPDate::MonthsEnum oldmonth = date.getMonth();
			while (date.isHoliday(city)|| date.isWeekend())
			{
				date += increment;
				if (date.getMonth() != oldmonth)
				{
					increment = +1;
					date += 2;
				}
			}
			break;
		}
  case ENDOFMONTH:
    {
      int numberOfDayInMonth = getNumberOfDaysInMonth(date.getYear(), date.getMonth());
      TPDate tmp_date(numberOfDayInMonth, date.getMonth(), date.getYear());
  		while (tmp_date.isHoliday(city) || tmp_date.isWeekend()) tmp_date--;
      date = tmp_date;
    }
	case NOROLLING:
		break;
	case INVALIDROLLING:
	default:
		{
		THROW("Invalid rolling convention")
		}
	break;
	}
}


	


void TPDate::addBusinessDays(int numberofdays, TP_City city)
{

	int index = 0;
	while (index <fabs((double)numberofdays))	
	{
		if (numberofdays>0)
		{
			(*this) += TPPeriod(1,DAY);
		}
		else
		{
			(*this) -= TPPeriod(1,DAY);	
		}

		if (!isHoliday(city) && !isWeekend())
		{
			++index;
		}
	}
	
}	


int TPDate::gregorianDayNumber() const
{
    return getExcelDate() + DateOffsets::JULIAN;
}


///////////
//	Added 20/10/2004
////////////////////////////////
void TPDate::addBusinessDays(int numberofdays, TP_City city1, TP_City city2)
{

	int index = 0;
	while (index <fabs((double)numberofdays))	
	{
		if (numberofdays>0)
		{
			(*this) += TPPeriod(1,DAY);
		}
		else
		{
			(*this) -= TPPeriod(1,DAY);	
		}

		if (!isHoliday(city1) && !isHoliday(city2) && !isWeekend())
		{
			++index;
		}
	}
	
}	

TPDate TPDate::moveToTheNextTradingDay(const TPDate& date, TP_City city1, bool excludeStartDate)
{
    TPDate tmp = date;

    if (excludeStartDate)
    {
        tmp.addBusinessDays(1,city1);
    }

    if (tmp.isHoliday(city1))
    {
        tmp.addBusinessDays(1, city1);
    }
    return tmp;
}


void TPDate::dateToPeriod(const TPDate& date, const TPDate& asofdate, TPPeriod& period)
{
  long l_asof = asofdate.toLong();
  long l_date = date.toLong();

  double diff = (double)(l_date - l_asof);
  double yearfraction = asofdate.getFractionOfYear(date, TP_ACT_ACT);
  long yearno = (long)yearfraction;

  long month = 12 * yearno;

  double monthfraction = (yearfraction - yearno) * 12;

  long remainingmonth = (long)monthfraction;
  month += remainingmonth;

  if(monthfraction - remainingmonth > 0.5) ++month;

  period = TPPeriod(month, MONTH);
}

//TODO: this function needs to be checked out thoroughly

double	TPDate::getFractionOfYear(const TPDate& date1, const TPDate& date2, TPBasis basis)
{

	if (date2<date1)	
	{
		THROW("TPDate::getFractionOfYear: no term returned: date2 should be greater or equal to date1")
	}


	double ret = 0.0 ;
	double adjustedstartdate, adjustedenddate;

	switch(basis)
	{
		//lets start with the easy stuff;

		case TP_ACT_365:
		{
			ret = (double)(date2.getExcelDate() - date1.getExcelDate())/365.0;
			break;
		}
		case TP_ACT_360:
		{
			ret = (double)(date2.getExcelDate()-date1.getExcelDate())/360;
			break;
		}
		case TP_30_360:
		{

			int dd1 = date1.getDayInMonth(), dd2 = date2.getDayInMonth();
            int mm1 = date1.getMonth(), mm2 = date2.getMonth();
            int yy1 = date1.getYear(), yy2 = date2.getYear();
			if (dd2 == 31 && dd1 < 30) { dd2 = 1; mm2++; }

			int NofDays = 360*(yy2-yy1) + 30*(mm2-mm1-1) + std::max(0,30-dd1) + std::min(30,dd2);
            ret = (360*(yy2-yy1) + 30*(mm2-mm1-1) + std::max(0,30-dd1) + std::min(30,dd2))/360.;


			/*
			//get year and month diff
			ret = (double)(date2.getYear() - date1.getYear()) + double(date2.getMonth() - date1.getMonth())/12;
			//adjust day count
			
			adjustedstartdate = (date1.getDayInMonth()==31)? 30 : date1.getDayInMonth();
			adjustedenddate   = (date2.getDayInMonth()==31)? 30 : date2.getDayInMonth();
			if (date1.getDayInMonth() <30) adjustedenddate = 31;
	
			ret += (double)(adjustedenddate - adjustedstartdate)/360;
			*/
			break;
		}
		case TP_30E_360:
		{
			//get year and month diff
			ret = date2.getYear() - date1.getYear() + double(date2.getMonth() - date1.getMonth())/12;
			//adjust day count
			
			adjustedstartdate = (date1.getDayInMonth()==31)? 30 : date1.getDayInMonth();
			adjustedenddate   = (date2.getDayInMonth()==31)? 30 : date2.getDayInMonth();
	
			ret += (double)(adjustedenddate - adjustedstartdate)/360;	
			break;
		}
		case TP_ACT_ACT:
		{
			//fraction of days within same year, simply divide by number of days in year
			if (date1.getYear() == date2.getYear()) 
			ret = (double)(date2.getExcelDate() - date1.getExcelDate())/(double)(getDaysInYear(date1.getYear()));
			//else if none is leap
			else if (!isLeap(date1.getYear()) && !isLeap(date1.getYear())) 
			ret = (double)(date2.getExcelDate() - date1.getExcelDate())/365.0;
			//else one of them is, count number of days in each year fraction
			else
			{
				ret = (double)(TPDate(31,12,date1.getYear()).getExcelDate() - date1.getExcelDate())/
				(double)getDaysInYear(date1.getYear());
				ret += (double)(date2.getExcelDate()- TPDate(31,12,date2.getYear() - 1).getExcelDate())/
				(double)getDaysInYear(date2.getYear());

				ret += (double)(date2.getYear()-date1.getYear()) -1.0;
			}
			break;
		}
		case TP_CONT:
		{
			double NoOfDays = date2.getExcelDate() - date1.getExcelDate();
			/// Then simply divide by 365.25
			ret = NoOfDays / DAYS_IN_YEAR_STOCH;
			break;
		}
		default :
			THROW("unknown basis for day count calculations")
			break;
	}
	
	return ret;


}

double	TPDate::getFractionOfYear(const TPDate& date1, const TPDate& date2, const std::string basis)
{
	THROW("TPDate::getFractionOfYear(TPDate, TPDate, string): Not yet implemented");
	return 0.0;
}



double	TPDate::getFractionOfYear(const TPDate& date, TPBasis basis) const
{
	return getFractionOfYear(*this, date, basis);
}

double	TPDate::getFractionOfYear(const TPDate& date, const std::string basis) const
{
	THROW("TPDate::getFractionOfYear(TPDate, string): Not yet implemented");
	return 0.0;
}


TPDate	TPDate::getNextIMMDate(const TPDate& datein, TP_City city)
{
	bool processed = false;

	//first add two business days

	TPDate ret = datein;
	ret.addBusinessDays(2,city);
	

	//if Non Delivery Month or Delivery Month and Day less than 15
	//then skip to the right month
	if ((ret.getMonth() %3) || (ret.getDayInMonth()<15))
	{
		//skip to next delivery month
		int newmonth = ret.getMonth() + (2-(ret.getMonth()-1)%3);
		//re-assign close date to be adjusted to the right day later on
		ret = TPDate(15, newmonth, ret.getYear());

		processed = true;
	}

	if ((!processed) && (ret.getDayInMonth()>21))
	{
		//if del month, then skip to the next
		int newmonth = ret.getMonth() + (3-ret.getMonth()%3);
		//if december and over 21st, then skip to next year
		//else adjust as normal;
		if (newmonth ==15)
		{
			ret = TPDate(15,3,ret.getYear()+1);
		}
		else
		{
			ret = TPDate(15,newmonth,ret.getYear());
		}
		processed = true;
	}
	
	//Well by this time we are somewhere close to the right date
	if(!processed)
	{
		while (ret.getWeekDay() !=4)	ret++;
		//should neva be > 21
		if (ret.getDayInMonth() > 21) return getNextIMMDate(ret, city);
		else return ret;
	}

	while (ret.getWeekDay() !=4)	ret++;

	return ret;
}

//std::string TPDate::save_minimal(cereal::JSONOutputArchive const & archive) const
//{
//	return std::to_string(ExcelDate_);
//}
//
//void TPDate::load_minimal(cereal::JSONInputArchive const & archive, std::string const & dateStr)
//{
//	std::string::size_type sz;   // alias of size_t
//
//	ExcelDate_ = std::stol(dateStr, &sz);
//}

std::string ToString(TPRolling rolling)
{
	std::string ret;

	switch(rolling)
	{
	case FOLLOWING:	ret = "F";break;
	case MODIFIEDFOLLOWING:	ret = "MF";break;
	case FRN:	ret = "FRN";break;
	case PREVIOUS:			ret = "P";break;
	case MODIFIEDPREVIOUS:	ret = "MP";
	case NOROLLING:			ret = "NONE";
	case INVALIDROLLING:
	default:	ret = "INVALID ROLLING CONVENTION";break;
	}

	return ret;
}

TPRolling	TPDate::ToRolling(const std::string& rhs)
{

	TPRolling	ret	= INVALIDROLLING;

	if( rhs == "F") ret = FOLLOWING;
	else if( rhs == "MF") ret = MODIFIEDFOLLOWING;
	else if( rhs == "FRN") ret = FRN;
	else if( rhs == "EURODOLLAR") ret = FRN;
	else if( rhs == "E") ret = FRN;
	else if( rhs == "P") ret = PREVIOUS;
	else if( rhs == "MP") ret = MODIFIEDPREVIOUS;
	else if( rhs == "NONE") ret = NOROLLING;

	return ret;

}
std::string ToString(TPBasis basis)
{

	std::string ret;

	switch(basis)
	{
	case TP_30_360:			ret	=	"30/360"; break;
	case TP_30E_360:		ret =	"30/360E"; break;
	case TP_ACT_360:		ret =	"ACT/360";break;
	case TP_ACT_365:		ret =	"ACT/365";break;
	case TP_ACT_ACT:		ret =	"ACT/ACT";break;
	case TP_CONT:			ret =   "CONT";break;
	case INVALIDBASIS:
	default:	ret = "Invalid Basis"; break;
	}

	return ret;
}

TPBasis TPDate::ToBasis(const std::string& rhs)
{

	TPBasis ret = INVALIDBASIS;
	if (rhs == "30/360" || rhs == "30-360") ret = TP_30_360;
	if (rhs == "30/360E" || rhs == "30-360E" ) ret = TP_30E_360;
	if (rhs == "ACT/360" || rhs == "ACT-360") ret = TP_ACT_360;
	if (rhs == "ACT/365" || rhs == "ACT-365") ret = TP_ACT_365;
	if (rhs == "ACT/ACT" || rhs == "ACT-ACT") ret = TP_ACT_ACT;
	if (rhs == "CONT")	 ret = TP_CONT;

	return ret;
}


