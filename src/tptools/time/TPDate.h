////////////////////////////////////////////////////////////////////////////
//
//  This program is the CONFIDENTIAL and PROPRIETARY property of'
//   Athanasios Pantas. Unauthorized use and reproduction'
//  of this program is strictly prohibited.
//
// Copyright (C) 2003  - Nikopolis
//  Copyright (C) 2003 - Athanasios Pantas.
//  ALL RIGHTS RESERVED.
/////////////////////////////////////////////////////////////////////////////

#ifndef TP_DATE
#define TP_DATE


#include <TPNikopolisUtils.h>
#include "TPPeriod.h"
#include "TPTenor.h"
#include <string>

#ifndef TPSTATIC
#define _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS 1 //Michele
#include <hash_map>
#endif // TPSTATIC

#include "TPDateUtils.h"
//#include <cereal\archives\json.hpp>
#include "TPTenor.h"
//#include "boost\optional.hpp"
#include <optional>


//#include "TPAny.h"
//Rolling
enum TPRolling
{
  FOLLOWING = 0,
  MODIFIEDFOLLOWING = 1,
  PREVIOUS =2 ,
  MODIFIEDPREVIOUS = 3,
  ENDOFMONTH = 4,
  FRN = 5, // For the FRN or Eurodollar convention
  NOROLLING = 6,
  INVALIDROLLING = 7
};

//Basis
enum TPBasis
{
  TP_30_360	=	0,
  TP_30E_360	=	1,
  TP_ACT_360	=	2,
  TP_ACT_365	=	3,
  TP_ACT_ACT	=	4,
  TP_CONT		=   5,
  INVALIDBASIS	=	6
};

namespace TP::date_time
{
	class Date;
}

class TPDate;
using OptionalTPDate = std::optional<TPDate>;

class TPNIKOPOLIS_IMPEXP TPDate
{
 
public:
  
  //Days
  enum DaysEnum
  {
    SUNDAY          =       1,
    MONDAY          =       2,
    TUESDAY         =       3,
    WEDNESDAY       =       4,
    THURSDAY        =       5,
    FRIDAY          =       6,
    SATURDAY        =       7
  };
  
  //Months
  
  enum MonthsEnum
  {
    JANUARY         =       1,
    FEBRUARY        =       2,
    MARCH           =       3,
    APRIL           =       4,
    MAY             =       5,
    JUNE            =       6,
    JULY            =       7,
    AUGUST          =       8,
    SEPTEMBER       =       9,
    OCTOBRE         =       10,
    NOVEMBRE        =       11,
    DECEMBRE        =       12
  };

  //Excel or RW Julian based dates
  //We treat summit format seperatly as we are unlikely to use them
  explicit TPDate(const long& date);
  
  //copy constructor
  TPDate(const TPDate& rhs);
  // from Date
  TPDate(const TP::date_time::Date& d);
  //ISO Date string format ctor YYYY-MM-DD
  TPDate(const std::string& date);
  
  // canonical ctors
  TPDate(const unsigned day, const unsigned month, const unsigned year);
  TPDate(const unsigned day, const MonthsEnum, const unsigned year);
    //
  static bool fromString(const std::string& delimitedString, TPDate& result, const std::locale& locale);
  static bool fromString(const std::string& delimitedString, TPDate& result);

  using Year = unsigned short;
  using Day = unsigned short;
  
  //dumb ctor
  //
  TPDate();
  
  //From Summit Date Format...from Summit to Excel so to speak
  //      const TPDate&   FromSummitToOurDateFormat(const long&);
  
  //dtor
  virtual ~TPDate(){}
  
  
//#ifndef TPSTATIC
  size_t hash() {return static_cast<size_t>(ExcelDate_);}
//#endif  
  
  //Offsets
  
  enum    DateOffsets
  {
    EXCEL   =       0,                      //not necessary since we code up our date as exceldate anyway
    JULIAN  =       2415019,
    SUMMIT  =       604000         //TODO: Dunno yet should be something around 604000
  };                      //besides, summit year starts at 1980-01-01
  
  //Acessors
  
  bool    isLeapYear(void)                const;
  bool    isWeekend(void)                 const;
  bool    isMonthEnd(void)                const;
  bool    isInvalid(void)                 const { return *this == InvalidDate_;}
  bool    isNull()                        const { return isValid(); }
  bool isValid() const                   { return !isInvalid(); }
  bool 	isHoliday(TP_City city)		const;
  
  int             getNumberOfDaysInThisMonth()    const;
  long    getExcelDate()                  const;
  long    getJulianDate()                 const;
  //      long    toSummitDate()                  const;
  int             getDayInMonth()                 const;
  int			  day() const { return getDayInMonth(); }
  MonthsEnum      getMonth()                      const;
  DaysEnum        getWeekDay()            const;
  unsigned            getYear()                               const;

  unsigned year() const {	  return getYear();  }
  unsigned month() const { return getMonth(); }
  //unsigned year() const { return getYear(); }

  TPDate  eom() const;
  // 
  Day dayOfMonthEnd() const ;

  // to Match Boost gregorian data (which follows Julian convention above)
  static TPDate fromGregorianDayNumber(const long gregorianDayNumber);
  int  gregorianDayNumber() const; // to conform with some boost stuff

    std::string  toString(void)                 const
    {
        return ToISOString();
    }
	std::string  ToISOString(void)               const;  //ISO Date format       YYYY-MM-DD
	std::string  ToEURString(void)               const;  //EU Date Format        DD-MM-YYYY
	std::string  ToUSAString(void)               const;  //US Date format        MM-DD-YYY       cant we all just get along...
	std::string  ToYYYYMMDD(void)               const; // YYYYMMDD
  //
  //static members
  //
  
 static TPDate MinimalDate() { return TPDate(1, 1, 1901); }
 static TPDate MaximalDate() { return TPDate(31, 12, 2200); }

  //              today's date.
  static TPDate   AsOfDate(void);
  
  static  bool             isLeap(const int y);
  //Total number of days in a given month
  static  int             getNumberOfDaysInMonth(const int& year, const int& month);
  //cumulative number of days from start of year till month end
  static  int             getDaysToMonthEnd(const int& year, const int& month);
  //total number of days in a given year
  static  int             getDaysInYear(const int& year);
  //returns our invalid date
  static const TPDate&    getInvalidDate(void) { return InvalidDate_;}
  //given a date and a city, says whether the date is a public holiday
  //TODO does this include weekends?
  static  bool		isHoliday(const TPDate&, TP_City city);
  //given a date and a period, shifts the date forward
  static TPDate		moveForward(const TPDate& date, double number,TPPERIODTYPE type);
  //given a date, a rolling convention and a city
  //adjust the date if its a holiday according to the convention.
  static void 		adjustBusinessDays( TPDate&, const TPRolling, TP_City city);	
  //fraction of year
  //given two dates and a basis, returns the fraction of year between the two dates
  static double		getFractionOfYear(const TPDate&, const TPDate&, TPBasis);
  static double		getFractionOfYear(const TPDate&, const TPDate&, const std::string);	
  
  static void dateToPeriod(const TPDate& date, const TPDate& asofdate, TPPeriod& period);

    // compatibility
  void getDMY(unsigned short& day, unsigned short& month, unsigned short& year) const;
  //operators
  
  TPDate& operator= (const long& rhs);
  bool operator== (const TPDate& rhs)             const;
  bool operator!= (const TPDate& rhs)             const;
  bool operator>= (const TPDate& rhs)             const;
  bool operator<= (const TPDate& rhs)             const;
  bool operator> (const TPDate& rhs)              const;
  bool operator< (const TPDate& rhs)              const;
  
  // from Tenor
  void addTenor(const TP::Core::TPTenor& tenor, const bool preventsnaptomonthend = false);
  void subtractTenor(const TP::Core::TPTenor& tenor, const bool preventsnaptomonthend = false);

  TPDate& operator+=(const TP::Core::TPTenor& tenor);
  TPDate& operator-=(const TP::Core::TPTenor& tenor);


  TPDate& operator+=(int numdays);
  TPDate& operator-=(int numdays);
  TPDate& operator+=(const TPPeriod& period);
  TPDate& operator-=(const TPPeriod& period);
  TPDate& operator++();
  TPDate	operator++(int);	
  TPDate& operator--();
  TPDate	operator--(int);
  
  friend TPDate   operator+ (const TPDate& date, int numdays);
  friend TPDate   operator- (const TPDate& date, int numdays);
  
  friend TPDate   operator+ (const TPDate& date, const TPPeriod& period);
  friend TPDate   operator- (const TPDate& date, const TPPeriod& period);
  
  friend TPDate   operator+ (const TPDate& date, const TP::Core::TPTenor& period);
  friend TPDate   operator- (const TPDate& date, const TP::Core::TPTenor& period);

  friend long   operator- (const TPDate& date1, const TPDate& date2);

  //conversion to a long value - used for day counting etc.
  const long& toLong() const { return ExcelDate_; }
  
  
  //periods arithmatic
  
  //this function rolls the date to the next business date, following a
  //rolling convention and given a city calendar
  void adjustBusinessDays(const TPRolling rol, TP_City city) {  adjustBusinessDays(*this, rol, city);}
  
  //This does addition AND substraction as well.
  void addBusinessDays(int, TP_City city);
 //
 // Added 20/10/2004. Check for holidays in two cities together..
	void addBusinessDays(int, TP_City city1, TP_City city2);
    /*
    * if excludeStartDate = true -> move one day forward no matter what
    * if excludeStartDate = false -> if start date is a trading day, stay there
    */
    static TPDate moveToTheNextTradingDay(const TPDate& date, TP_City city1,  bool excludeStartDate = false);
  //main function when moving forward n periods
  //can move backward as well.
  
  void moveForward(double, TPPERIODTYPE);
  void addYears(const size_t&n)
  {
	  moveForward(static_cast<double>(n), YEAR);
  }
  void addMonths(const size_t&n)
  {
	  moveForward(static_cast<double>(n), MONTH);
  }
  void addẀeeks(const size_t&n)
  {
	  moveForward(static_cast<double>(n),WEEK);
  }

  void addDays(const size_t&n)
  {
	  moveForward(static_cast<double>(n), DAY);
  }
  
  //fraction of year
  //given a date and a basis, gives back fraction of year between today and the given date
  
  double		getFractionOfYear(const TPDate&, TPBasis) const;
  double		getFractionOfYear(const TPDate&, const std::string) const;
  
  //Futures and Listed Options Delivery Rules
  //TODO Needs to be parametrised somehow over a delivery/trading convention
  
  //Gives back next IMM date (i.e. next wednesday of 3rd week of MAR/JUN/SEP/DEC)
  static TPDate	getNextIMMDate(const TPDate& datein, TP_City city);
  
  //Misc
  
  
  
  static	TPBasis		ToBasis(const std::string&);
  static	TPRolling	ToRolling(const std::string&);
  
  // Serilisation
  //friend class cereal::access;
  //std::string save_minimal(cereal::JSONOutputArchive const & archive) const;
  //void load_minimal(cereal::JSONInputArchive const & archive, std::string const & dateStr);

//  uint32_t save_minimal(cereal::PortableBinaryOutputArchive const & archive) const;
 // void load_minimal(cereal::PortableBinaryInputArchive const & archive, uint32_t const & rep);
  
private:
  
  //THE date
  long          ExcelDate_;
  
  //to be returned as invalid date
  static TPDate InvalidDate_;
  
  //days in month and accumaled days in year at each month end;
  
  static  int   DaysInMonth_[2][13];
  static  int   AccumaltiveDaysInYear_[2][13];
  
  
  //Internal
  unsigned int  Day_;
  DaysEnum      WeekDay_;
  MonthsEnum    Month_;
  unsigned int  Year_;
  
  //regenerating the above member whenever we manipulate our ExcelDate
  void regenerate();
  
  //regenerates dates when adding or substracting months
  void regenerateAfterMonthIncrement();
  
  
  //string format date for differemt ISO/EU/US formats
  std::string ToString(int) const;
  
};

TPNIKOPOLIS_IMPEXP std::string	ToString(TPBasis);
TPNIKOPOLIS_IMPEXP std::string	ToString(TPRolling);

inline bool TPDate::isLeap(const int year)       
{
  return( ((year % 400 == 0) || (year % 100 != 0)) && (year % 4 == 0) );
}

inline bool TPDate::isLeapYear()        const
{
  return (isLeap(Year_));
}

inline  TPDate& TPDate::operator=(const long& rhs)
{
  *this = TPDate(rhs);
  return *this;
}

inline bool TPDate::operator==(const TPDate& rhs)       const
{
  return (ExcelDate_ == rhs.ExcelDate_);
}

inline bool TPDate::operator!=(const TPDate& rhs)       const
{
  return (ExcelDate_ != rhs.ExcelDate_);
}

inline bool TPDate::operator>=(const TPDate& rhs)       const
{
  return (ExcelDate_ >= rhs.ExcelDate_);
}

inline bool TPDate::operator<=(const TPDate& rhs)       const
{
  return (ExcelDate_ <= rhs.ExcelDate_);
}

inline bool TPDate::operator>(const TPDate& rhs)        const
{
  return (ExcelDate_ > rhs.ExcelDate_);
}

inline bool TPDate::operator<(const TPDate& rhs)        const
{
  return (ExcelDate_ < rhs.ExcelDate_);
}


inline long TPDate::getJulianDate()                     const
{
  return ExcelDate_ + JULIAN;
}

inline long TPDate::getExcelDate()                      const
{
  return ExcelDate_ ;
}

inline int              TPDate::getDayInMonth()                 const
{
  return Day_;
}

inline TPDate::MonthsEnum       TPDate::getMonth()                      const
{
  return Month_;
}

inline TPDate::DaysEnum TPDate::getWeekDay()            const
{
  return WeekDay_;
}
inline unsigned              TPDate::getYear()                               const
{
  return Year_;
}


inline std::ostream& operator<<(std::ostream& o, const TPDate& date)
{
  if (date.isInvalid()) return o<< "Invalid Date";
  
  return o<<date.getDayInMonth() << "/" << date.getMonth() << "/" << date.getYear();
}

inline std::string ToString(const TPDate& date)
{
  return date.ToISOString();
}

inline TPDate&  TPDate::operator+=(int numdays)
{
  ExcelDate_      += numdays;
  regenerate();
  
  return *this;
  
}

inline TPDate&  TPDate::operator++()
{
  *this +=1; 
  return *this;
  
}


inline TPDate	TPDate::operator++(int)
{
  TPDate oldate = *this;
  ++(*this);
  return oldate;
}

inline TPDate	TPDate::operator--(int)
{
  TPDate oldate = *this;
  --(*this);
  return oldate;
}


inline TPDate&  TPDate::operator--()
{
  *this -=1; 
  return *this;
  
}

inline
TPDate   operator+ (const TPDate& date, const TPPeriod& period)
{
	return TPDate::moveForward(date, period.getNumber(), period.getType());


}

inline
TPDate   operator- (const TPDate& date, const TPPeriod& period)
{
	return TPDate::moveForward(date, -period.getNumber(), period.getType());
	
}	

inline
long   operator- (const TPDate& date1, const TPDate& date2)
{
	return date1.getExcelDate() - date2.getExcelDate();

}
inline  TPDate   operator+ (const TPDate& date, const TP::Core::TPTenor& period)
{
	TPDate res = date;
	res.addYears(period.yearCount());
	res.addMonths(period.monthCount());
	res.addẀeeks(period.weekCount());
	res.addDays(period.dayCount());
	return res;

}
inline TPDate   operator- (const TPDate& date, const TP::Core::TPTenor& period)
{

}

inline TPDate   operator+ (const TPDate& date, int numdays)
{
  
  TPDate  tmp = date;
  
  tmp.ExcelDate_ += numdays;
  tmp.regenerate();
  
  return tmp;
}

inline TPDate&  TPDate::operator-=(int numdays)
{
  ExcelDate_      -= numdays;
  regenerate();
  
  return *this;
  
}

inline TPDate   operator- (const TPDate& date, int numdays)
{
  
  TPDate  tmp = date;
  
  tmp.ExcelDate_ -= numdays;
  tmp.regenerate();
  
  return tmp;
}

inline TPDate& TPDate::operator+=(const TPPeriod& period)
{
  
  moveForward(period.getNumber(), period.getType());
  return *this;
}
inline TPDate& TPDate::operator-=(const TPPeriod& period)
{
  
  moveForward(-period.getNumber(), period.getType());
  return *this;
}





//STLport was complaining about passing a reference to the operators below


namespace std {
    template<>
    struct hash<TPDate> {
        size_t operator()(const TPDate& date) const {
            return static_cast<size_t>(date.getExcelDate());
        }
    };

    template<>
    struct equal_to<TPDate> {
        bool operator()(const TPDate& date1, const TPDate& date2) const {
            return date1 == date2;
        }
    };
}

//STL function functors
//#ifndef TPSTATIC
//
//#ifndef WIN32
//template<>
//#else
//template<>//Michele
//#endif
//struct std::hash<TPDate>
//{
//  size_t operator()(TPDate date) const
//  {
//    return date.hash();
//  }
//};
//
//#ifndef WIN32
//template<>
//#else
//template<>//Michele
//#endif
//struct std::equal_to<TPDate >
//{
//  bool operator()(TPDate date1, TPDate date2) const
//  {
//    return date1 == date2 ;
//  }
//};
//#endif // TPSTATIC


//Added a wrap for adjusting business days
//To be used in STL Algorithms
class TPBusinessDayRule
{
 public:
  TPBusinessDayRule(const TPRolling rol, TP_City city) : rol_(rol),city_(city)
    {}

  TPDate operator()(const TPDate& date)
    { 
      TPDate adjustedDate(date);
      adjustedDate.adjustBusinessDays(rol_,city_); 
#ifdef TP_DEBUG
      cout << "adjusting " << date << " -> " << adjustedDate << endl;
#endif
      return adjustedDate;
    }

 private:
  TPRolling rol_;
  TP_City city_;
};

class TPAddPeriodToDate
{
 public:
  TPAddPeriodToDate(const TPPeriod& period) : period_(period)
    {}

  TPDate operator()(const TPDate& date)
    { TPDate added(date); added += period_; return added; }

 private:
  TPPeriod period_;
};

class TPSubtractPeriodFromDate
{
 public:
  TPSubtractPeriodFromDate(const TPPeriod& period) : period_(period)
    {}

  TPDate operator()(const TPDate& date)
    { TPDate subtracted(date); subtracted -= period_; return subtracted; }

 private:
  TPPeriod period_;
};

class TPAddBusinessDaysToDate
{
 public:
  TPAddBusinessDaysToDate(int numberofdays, TP_City city) : numberofdays_(numberofdays), city_(city)
    {}

  TPDate operator()(const TPDate& date)
    { TPDate added(date); added.addBusinessDays(numberofdays_,city_); return added; }

 private:
  int numberofdays_;
  TP_City city_;
};


//======
// bariemai na grapso comment.. koita ton kodika gia leptomereies..
//===========================================
//#include "TPAny.h"
//#include "TPTools/QUANTLIB/Utilities/HashHelpers.h"
//TP_HASH_IMPLEMENT_USING_METHOD(TPDate)
/*
double AnyExpiryTodouble(const TPAny& 	expiry_any, const TPDate& asOfDate)
{
	double res;
    switch(expiry_any.getType())
	{
		// If string, then a period is given
	case TPAny::STRING:
		{
			TPPeriod expiry_period(ToString(expiry_any));
			TPDate Expirydate(asOfDate); Expirydate += expiry_period;
			Expirydate.adjustBusinessDays(MODIFIEDFOLLOWING,TP_LON);
			res= (Expirydate.getExcelDate() - asOfDate.getExcelDate())/365.;
			break;
		}
	case TPAny::DOUBLE:
	case TPAny::LONG:
		{
		double tau = expiry_any.toDouble();
		if (tau > 500.) // in this case the supplied number is a date, have to convert it to year fraction
		{
			res= (tau -asOfDate.getExcelDate())/365.;
		}
		else
			res = expiry_any;
		}
		break;
	default:
		THROW("AnyExpiryTodouble: Invalid Expiry Type ")


	}

	return res;
}
*/

#endif
