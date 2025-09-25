#include "TPHolidays.h"
#include "../TPException.h"
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <cassert>
#include "../TPGlobals.h"

//TPDate transform_date(const std::string& str);

TPHolidays::cityList_t   TPHolidays::cityList_;
TPHolidays::holidayMap_t TPHolidays::holidayMap_;
TPHolidays::cityMap_t    TPHolidays::cityMap_;
bool TPHolidays::myHolidaysAreImported;

const size_t TPHolidays::maxDatesPerOutputRow = 250;

void
TPHolidays::setHoliday(const TPDate& date, TP_City city)
{

#ifdef TP_DEBUG
  cout << "setHoliday date=" << date << "\t" << "city=" << city << endl;
#endif

  holidayMap_t::iterator it = holidayMap_.find(date);
  if (it != holidayMap_.end()) {
    //the holiday is already in the map, we only need to add the city
    (*it).second.push_back(city);
    (*it).second.sort();
  } else {
    //we need to creat a list first and then insert into the hash_map,
    //because this is a new holiday
	  std::list<TP_City> myCityList;
    myCityList.push_back(city);


	//
    holidayMap_.insert(std::pair<const TPDate,cityList_t>(date, myCityList));
  }

  cityMap_t::iterator jt=cityMap_.find(city);
  if (jt != cityMap_.end())
    (*jt).second++;
  else

	//
    cityMap_.insert(std::pair<const TP_City,size_t>(city,1));

  cityList_t::const_iterator ct=find(cityList_.begin(), cityList_.end(), city);
  if (!binary_search(cityList_.begin(),cityList_.end(),city)) {
    cityList_.push_back(city);
    cityList_.unique();
    cityList_.sort();
  }
}

void 
TPHolidays::setHolidaysForCity(const dateList_t& holiday, TP_City city)
{
  for(dateList_t::const_iterator it=holiday.begin();
      it != holiday.end();
      it++) setHoliday(*it, city);
}

void 
TPHolidays::removeHoliday(const TPDate& date, TP_City city)
{
  holidayMap_t::iterator it = holidayMap_.find(date);
  bool is_holiday = it != holidayMap_.end();
  
  
  if (!is_holiday)
    return; //nothing to be deleted

  cityList_t::iterator jt = find((*it).second.begin(),(*it).second.end(), city);

  if (jt != (*it).second.end()) {
    //it is a holiday for city, then we need to delete it from the list
    (*it).second.erase(jt);

    //If the list is now empty remove it from the holidayMap_
    if ((*it).second.size() == 0)
      holidayMap_.erase(date);

    cityMap_[city]--;

#ifdef TP_DEBUG
    cout << "remove::date=" << date << "\tcity=" << city << "\t";
    cout << "remaining = " << cityMap_[city] << endl;
#endif

    //no holiday for this city anymore hence remove it from the cityList_
    if (cityMap_[city] == 0) {
      cityList_.remove(city);
#ifdef TP_DEBUG
      cout << "erasing" << endl << "left with the cities" << endl;
      for(jt=cityList_.begin(); jt != cityList_.end(); jt++)
	cout << *jt << endl;
#endif
    }
  }
}

void
TPHolidays::removeAllHolidaysForCity(TP_City city)
{
  //this could be code up more efficiently - but this will do (SME, 24.02.02)

  while (binary_search(cityList_.begin(),cityList_.end(),city)) {
    holidayMap_t::iterator it = holidayMap_.begin();
    bool isCityHoliday=isHoliday((*it).first,city);
    while ((!isCityHoliday) && (it != holidayMap_.end())) {
      it++;
      isCityHoliday=isHoliday((*it).first,city) && (it != holidayMap_.end());
    }
    if (isCityHoliday)
      removeHoliday((*it).first,city);
  }
}



bool
TPHolidays::isHoliday(const TPDate& date, TP_City city)
{
	if (!myHolidaysAreImported)
	{
		std::ifstream myFile(TPGlobals::getHolidaysFileHandle().c_str(), std::ios::in);

//to fix a bug whereby the library looped non stop when the holidays file
//passed in was non existent.

		if( myFile.bad() )
		{
			std::string hbError = "TPHolidays: Invalid or Inexistent Holiday file ( ";

			hbError += TPGlobals::getHolidaysFileHandle() + " ). ";

			THROW(hbError);
		}

		import(myFile);
	}

  holidayMap_t::const_iterator it= holidayMap_.find(date);

  bool is_holiday = it != holidayMap_.end();

  if (!is_holiday)
    return false;

  //in principle date is a holiday, but for the city?
  is_holiday = is_holiday && \
               binary_search((*it).second.begin(),
			     (*it).second.end(),
			     city);
  return is_holiday;
}

bool TPHolidays::isBusinessDay(const TPDate& date, TP_City city)
{ return !isHoliday(date, city); }

void
TPHolidays::getCityList(cityList_t& cities)
{ 
  cities.clear();
  copy(cityList_.begin(), cityList_.end(), cities.begin()); 
}

void
TPHolidays::getHolidaysForCity(TP_City city, dateList_t& holidays)
{
  holidays.clear();
  for(holidayMap_t::iterator it=holidayMap_.begin();
      it != holidayMap_.end();
      it++)
    if (isHoliday((*it).first, city))
      holidays.push_back((*it).first);
  holidays.sort();
}

void 
TPHolidays::import(std::ifstream& inFile, const char sep)
{
  try {

    //first remove all pervious holidays
    for(holidayMap_t::iterator ht = holidayMap_.begin();
	ht != holidayMap_.end();
	ht++)
      (*ht).second.clear();
    holidayMap_.clear();
    cityList_.clear();
    cityMap_.clear();

    //Start with the input
    checkLineSeparator(sep);

    char c_sep[]= "\t";
  

    while (!inFile.eof()) 
	{
		std::string line;
      getline(inFile, line);

      char *token = strtok(const_cast<char*>(line.c_str()),c_sep);

      if (token != NULL) 
	  {
		//first entry in the line is the city
		TP_City city = toCity(token);
      
		//next we loop until the token is NULL, which is the end of the line
		token = strtok(NULL, c_sep);
		//	if (token != NULL)
		//	  cityList_.push_back(city);
		while (token != NULL) 
		{
			try 
			{
				TPDate holiday(token);
				setHoliday(holiday, city);
			} 
			catch (TPException) 
			{}
			token = strtok(NULL, c_sep);
		}
      }
    }
    //keep a unique list of citys
    cityList_.unique();
    /*we sort the date associated list of cities
    for(holidayMap_t::iterator it=holidayMap_.begin();
	it != holidayMap_.end();
	it++) 
      (*it).second.sort();
    */
  } catch (...) {}

  myHolidaysAreImported = true;
}


void
TPHolidays::dump(std::ofstream& outFile, const char sep)
{
  bool needsNewLine;
  try {
    checkLineSeparator(sep);
    for(cityList_t::const_iterator it = cityList_.begin();
	it != cityList_.end();
	it++) {
#ifdef TP_DEBUG
      cout << "dumping holidays for " << *it << endl;
#endif
      outFile << *it << sep;
      dateList_t dateList;
      getHolidaysForCity(*it, dateList);
      
      size_t colCtr=0;
      size_t numOfDumpedDates=0;
      for(dateList_t::const_iterator jt=dateList.begin();
	  jt != dateList.end();
	  jt++) {
	outFile << (*jt).ToISOString();
	numOfDumpedDates++;
	
	colCtr++; 
	needsNewLine = (colCtr % maxDatesPerOutputRow) == 0;
	needsNewLine = needsNewLine && (numOfDumpedDates != dateList.size());

	dateList_t::const_iterator kt=jt;
	kt++;
	if ((kt != dateList.end()) && (!needsNewLine)) {
	  outFile << sep;
	}
	if (needsNewLine) {
	  outFile << std::endl << *it << sep;
	}
      }
      outFile << std::endl;
    }
  } catch (...) {}
}

void
TPHolidays::checkLineSeparator(const char sep)
{
  if (sep == '-') {
    THROW("Bad Line separator - ")
  }
}



