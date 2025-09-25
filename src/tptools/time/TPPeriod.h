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

#ifndef TP_PERIOD
#define TP_PERIOD

#include <TPNikopolisUtils.h>
#include "../TPGlobals.h"
#include "../TPException.h"
//
#include <string>
#include <cereal\cereal.hpp>
#include "TPTenor.h"


class TPDate;
class TPPeriod;



enum TPPERIODTYPE {
	DAY		=	0,
	BUSINESSDAY	=	1,
	WEEK		=	2,
	MONTH		=	3,
	YEAR		=	4,
    OVERNIGHT   = 5,
    TN          =6,
	INVALIDPERIOD	=	7
	};



class TPNIKOPOLIS_IMPEXP TPPeriod
{

public:
	//Accessors

	int 		      getNumber() const {return myNumber;}
	TPPERIODTYPE	getType() const {return myType;}
    size_t         getFrequency() const;

	//ctors

	const TPPeriod& operator=(const TPPeriod& rhs);
	TPPeriod(int num, TPPERIODTYPE type):myNumber(num), myType(type){}
	TPPeriod(const TPDate& date1, const TPDate& date2);
	TPPeriod(const std::string& periodstring, bool convertToUpper = true);
	TPPeriod(const TPPeriod& rhs):myNumber(rhs.myNumber),myType(rhs.myType){}
	TPPeriod(const TP::Core::TPTenor& tenor);
	TPPeriod():myNumber(0),myType(INVALIDPERIOD){}


	//dtor
	~TPPeriod(){}

	//boolean operators
	bool isInvalid() const { return (myType == INVALIDPERIOD);}
	bool operator==(const TPPeriod& rhs)	const;
	bool operator!=(const TPPeriod& rhs)	const;
	bool operator>=(const TPPeriod& rhs)	const;
	bool operator<=(const TPPeriod& rhs)	const;
	bool operator>(const TPPeriod& rhs)	const;
	bool operator<(const TPPeriod& rhs)	const;

	//arithmatic operations

	const TPPeriod& operator+=(const TPPeriod& rhs);
	const TPPeriod& operator-=(const TPPeriod& rhs);

		//we need to be able to multiply 6M by 0.5 and get 3M period.
	const TPPeriod& operator*=(double rhs);
	const TPPeriod& operator/=(double rhs);


		// careful here, this returns a double and doesnt care whether
		// you are dividing 5Y by 3M (which makes sense) or
		// 2W by 5D (which clearly doesnt)
		// TPGlobals::isInt() check needs to be done on the output
		// if ur expecting an integer
		//
		// I warned you..

	friend double operator/(const TPPeriod& p1, const TPPeriod& p2);
	friend TPNIKOPOLIS_IMPEXP TPPeriod operator+(const TPPeriod& p1, const TPPeriod& p2);
	friend TPNIKOPOLIS_IMPEXP TPPeriod operator-(const TPPeriod& p1, const TPPeriod& p2);
	friend TPNIKOPOLIS_IMPEXP TPPeriod operator*(const double rhs, const TPPeriod& p);
	
	//miscellaneous

	//for some reason this does not work
	//ya ne znayo potechemu eto tak...
	std::string	ToString(void)	const;

	//TODO: Get rid of this.

	// Get the number of periods given two dates
	size_t NumberOfPeriods(TPDate& d1, TPDate& d2) const;

	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& archive, uint32_t const /*version*/);

private:
	//converts years into months
	//and weeks into days.
	void converttolowerperiod();

	//checks whether days can be upgraded into weeks
	//and months into years

	void converttoupperperiod();
	int 		myNumber;
	TPPERIODTYPE	myType;

};

//inline functions

inline const TPPeriod& TPPeriod::operator=(const TPPeriod& rhs)
{
	if (this != &rhs)
	{
		myNumber = rhs.myNumber;
		myType   = rhs.myType;
	}

	return *this;
}
	
//binary comparison

//I cant ganarantee that these are error free as 
//I simply copied and pasted the first operator
//5 times.

inline bool TPPeriod::operator==(const TPPeriod& rhs) const
{
	if (myType == rhs.myType) return (myNumber == rhs.myNumber);

		//this is necessary since this operator is const.
		//not a big waste of memort anyway..

		TPPeriod p1 = *this, p2 = rhs;
		p1.converttolowerperiod();
		p2.converttolowerperiod();

	if (p1.myType == p2.myType) return (p1.myNumber == p2.myNumber);
	else return false;
}

inline bool TPPeriod::operator!=(const TPPeriod& rhs) const
{
  return !(*this == rhs);
}

inline bool TPPeriod::operator>=(const TPPeriod& rhs) const
{
  return !(*this < rhs);
}

inline bool TPPeriod::operator<=(const TPPeriod& rhs) const
{
  return !(*this>rhs);
}

inline bool TPPeriod::operator>(const TPPeriod& rhs) const
{
  return (*this!=rhs) && !(*this<rhs);
}

inline bool TPPeriod::operator<(const TPPeriod& rhs) const
{
	if (myType == rhs.myType) return (myNumber < rhs.myNumber);
		//this is necessary since this operator is const.
		//not a big waste of memort anyway..

		TPPeriod p1 = *this, p2 = rhs;
		p1.converttolowerperiod();
		p2.converttolowerperiod();

	if (p1.myType == p2.myType) return (p1.myNumber < p2.myNumber);
	else 
  {
    // This can be the case where we try to compare "2D" and "1M"
    if(p1.myType == DAY && p2.myType == MONTH)
    {
      if(p1.myNumber < 30) 
      {
        return true;
      }
      else
      {
        THROW("Can not compare periods")
      }
    }
    else if(p2.myType == DAY && p1.myType == MONTH)
    {
      if(p2.myNumber <= 30) 
      {
        return false;
      }
      else
      {
        THROW("Can not compare periods")
      }
    }
    THROW("Can not compare periods")
  }
}
//end of binary comparison

//global ToString



inline void TPPeriod::converttolowerperiod()
{

	//Now the convention is the following
	//MONTHS and YEARS can somehow be equivalent.
	//WEEKS and DAYS can also be equivalent, somehow.
	//All other possibilties are not allowed.
	switch(myType)
	{
	case YEAR:
	{
		myNumber *= 12;
		myType   = MONTH;
	}
		break;
	case WEEK:
	{
		myNumber *= 7;
		myType   = DAY;
	}
		break;
	default:
		break;
	}
}

inline void TPPeriod::converttoupperperiod()
{
	switch(myType)
	{
	case MONTH:
	{
		if(!(myNumber % 12))
		{
		myNumber = static_cast<int>(myNumber / 12);
		myType   = YEAR;
		}
	}
		break;
	case DAY:
	{
		if (!(myNumber % 7))
		{
		myNumber = static_cast<int>(myNumber / 7);
		myType   = WEEK;
		}
	}
		break;
	default:
		break;
	}
}

inline double operator/(const TPPeriod& p1, const TPPeriod& p2)
{
	TPPeriod tmp1 = p1, tmp2 = p2;
	tmp1.converttolowerperiod();	
	tmp2.converttolowerperiod();		

	if (tmp1.myType != tmp2.myType) THROW("Inavlid Period Division Operation")
	else return static_cast<double>(tmp1.myNumber) / static_cast<double>(tmp2.myNumber);
}
	

inline
TPPeriod operator-(const TPPeriod& p1, const TPPeriod& p2)
{
	TPPeriod tmp1 = p1;
	tmp1 -= p2;
	return tmp1;
}


TPNIKOPOLIS_IMPEXP std::string ToString(const TPPeriod& period) ;



#endif
