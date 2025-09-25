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

#ifndef TP_HOLIDAYS
#define TP_HOLIDAYS

#include <TPNikopolisUtils.h>
#include <TPTools/Time/TPDate.h>

#ifdef TPSTATIC

#include <sstream>
#include <fstream>

#else

#ifndef WIN32
#include <fstream>
#else
//Michele #include <fstream.h>
#include <fstream>
#endif

#endif // TPSTATIC


#include <list>
#include <map>

#ifndef TPSTATIC
//#include <hash_map>
#endif // TPSTATIC

#include <string>

#include "TPDateUtils.h"
#include <unordered_map>//Michele: instead of hash_map

class TPNIKOPOLIS_IMPEXP TPHolidays
{
 public:


  typedef std::list<TP_City>        cityList_t;

  //counts the number of holidays for each city
  typedef std::map<TP_City, size_t> cityMap_t;

  /*
#ifndef TPSTATIC

#ifdef __INTEL_COMPILER
  //It uses the wrapper _Hash_compare found in <xhash> to wrap the traditional STL to Intel's
  typedef hash_map<TPDate,cityList_t, _Hash_compare<TPDate, equal_to<TPDate> > > holidayMap_t;
#else
*/
  typedef std::unordered_map<TPDate, cityList_t> holidayMap_t;
//#endif //__INTEL_COMPILER

//#else
 // typedef map<TPDate, cityList_t> holidayMap_t;
//#endif

  typedef std::list<TPDate>  dateList_t;

  TPHolidays() {}
  ~TPHolidays() {}


  //remove a holiday for a city
  static void removeHoliday(const TPDate& date, TP_City city);

  //remove all holidays for a city
  static void removeAllHolidaysForCity(TP_City city);

  //sets
  static void setHoliday(const TPDate& date, TP_City city);
  static void setHolidaysForCity(const dateList_t& holiday, TP_City city);

  //gets
  static void getCityList(cityList_t& cities);
  static void getHolidaysForCity(TP_City city, dateList_t& holidays);

  //major method: returns true, if date is a holiday in city.
  //              returns false, if date is a working date
  //REMARK: weekends etc. have to be imported before this make much sense
  static bool isHoliday(const TPDate& date, TP_City city);
  static bool isBusinessDay(const TPDate& date, TP_City city);

  //I/O
  //get the holidays from a file stream
  //The file is a file of the following format, where each line has the following format
  //<CityName1>,<Holiday1-Iso Date String:YYYY-MM-DD>,<Holiday2>,...,<HolidayN>
  //<CityName1>,<HolidayN+1>,...,<HolidayN>
  //<CityName2>,....
  //etc.
  //where  sep=',' was taken for simplicity
  //Different separator strings may be used. Should not use "-", because
  //the tokenizer gets confused otherwise
  //THROWS, if sep='-'
  //REMARK: A new import destroys all pre existing data for a given city
  static void import(std::ifstream& inFile, const char sep='\t');

  //dump using sep as separate between dates in a line
  //"-" is not allowd as sep
  //THROWS, if sep='-'
  static void dump(std::ofstream& outFile, const char sep='\t');

  // check if holidays are imported
  static bool hasHolidays() { return myHolidaysAreImported; }

 private:

  static cityList_t    cityList_;
  static holidayMap_t  holidayMap_;
  static cityMap_t     cityMap_; 
  static bool			myHolidaysAreImported;

  static const size_t maxDatesPerOutputRow; //to keep Excel happy in the output

  //THROWS
  static void checkLineSeparator(const char sep);
};


#endif
