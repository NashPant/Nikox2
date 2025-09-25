#include "TPPeriod.h"
#include <TPTools/Time/TPDate.h>
#include <map>

size_t TPPeriod::getFrequency() const
{
	if(myType == DAY)
  {
    // This should not really happen...
    return static_cast<size_t>(round((365.25 / (double)myNumber)));
  }
	else if(myType == MONTH)
	{
		return 12 / myNumber ;
	}
	else if(myType == YEAR) 
	{
		return myNumber;
	}
  else THROW("TPPeriod::getFrequency: Invalid type of period.")
}



TPPeriod::TPPeriod(const TPDate& date1, const TPDate& date2)
{
	//better safe than..

	if (date2<=date1) THROW("first date needs to be less than second date to construct a period")
	//convention adopted is as follows:
	//if days are equal then we have a valid period
	//if not then the difference needs to be less then seven or a
	//multiple of seven coz we dont want to create
	//periods of 333 days for instance.

	//note that this ctor does not give back a businessdays period.

	if (date1.getDayInMonth() == date2.getDayInMonth())
	{
		if (date1.getMonth() == date2.getMonth())
		{
			myNumber = date2.getYear() - date1.getYear();
			myType  = YEAR;
		}
		else
		{
			myNumber = (date2.getYear() - date1.getYear())*12;
			myNumber += date2.getMonth() - date1.getMonth();
			myType   = MONTH;
			converttoupperperiod();
		}
	}

	else if ((date2.getExcelDate() - date1.getExcelDate())<7)
	{
			myNumber = date2.getExcelDate() - date1.getExcelDate();
			myType   = DAY;
	}

	else if (((date2.getExcelDate() - date1.getExcelDate()) % 7 == 0))
	{
			myNumber = static_cast<int>((date2.getExcelDate() - date1.getExcelDate())/7);
			myType   = WEEK;
			converttoupperperiod();
	}

	else THROW("difference of two dates does not provide a valid period")

}

namespace
{
	std::map<TP::Core::PERIOD, TPPERIODTYPE> tpPeriodMap = {
	{TP::Core::PERIOD::ON ,OVERNIGHT},
	{TP::Core::PERIOD::TN , TN},
	{TP::Core::PERIOD::D ,DAY},
	{TP::Core::PERIOD::BD ,BUSINESSDAY},
	{TP::Core::PERIOD::W ,WEEK},
	{TP::Core::PERIOD::M ,MONTH},
	{TP::Core::PERIOD::Y ,YEAR}
	};
}
TPPeriod::TPPeriod(const TP::Core::TPTenor& tenor)
{
	myType = tpPeriodMap[tenor.getPeriod()];
	myNumber = tenor.getMultiplier();
}

TPPeriod::TPPeriod(const std::string& periodstring, bool convertToUpper)
{

	myNumber = atoi(periodstring.substr(0,periodstring.size()-1).data());
//	cout << periodstring.size()<<" "<<periodstring<<" "<<periodstring.substr(0,periodstring.size()-1)<<" "<<myNumber<<" ";
	std::string str = periodstring.substr(periodstring.size()-1,periodstring.size());
//	cout << str<<endl;

	//TODO TPGlobals::toUpper(str);

	if (str == "D" || str == "d") myType = DAY;
	else if (str == "B" || str == "b") myType = BUSINESSDAY;
	else if (str == "W" || str == "w") myType = WEEK;
	else if (str == "M" || str == "m") myType = MONTH;
    else if (str == "Y" || str == "y") myType = YEAR;
    else if (str == "ON" || str == "on") myType = OVERNIGHT;
    else if (str == "TN" || str == "tn") myType = TN;
	else THROW("Invalid Period Tag")

	if (convertToUpper)
		converttoupperperiod();	

	//cout <<"TPPeriod created"<<endl;
	//if (myType == DAY) cout <<"day type"<<endl;

}
	
std::string	TPPeriod::ToString(void)	const
{
	
	std::string ret = TPGlobals::LongToString(myNumber);


	switch(myType)
	{
	case DAY:	
	ret += "D";
	break;
	case BUSINESSDAY:	
	ret += "B";
	break;
	case WEEK:		ret += "W";
	break;
	case MONTH:		ret += "M";
	break;
	case YEAR:		ret += "Y";
	break;
    case OVERNIGHT:     ret +=  "ON";
    case TN:        ret += "TN";
	case INVALIDPERIOD:	ret ="Invalid Period";
	break;
	default:		ret ="unspecidied period";
	}


	return ret;

}


std::string ToString(const TPPeriod& period) { return period.ToString();}


const TPPeriod& TPPeriod::operator+=(const TPPeriod& rhs)
{
	TPPeriod p1 = rhs;
	p1.converttolowerperiod();
	converttolowerperiod();
	
	//I was just thinking
	//If u ever get such an error message on excel spread sheets
	// i.e. "Can not add periods", you wouldnt have a clue
	// on what that means, or what the error is from now
	//would you?
	//Exactly, thats my point. Errors need to be as explicit
	// as possible

	if (myType != p1.myType) THROW("Can not add periods")
	else
	{
		myNumber += p1.myNumber;
		converttoupperperiod();
	}

	return *this;	

}

//////////////////////////////////////////////////
// Number of periods between two given dates
/////////////////////////////////////////////////
size_t TPPeriod::NumberOfPeriods(TPDate& d1, TPDate& d2) const
{

	TPDate _D1(d1); 
	int j=0;
	while(_D1<d2)
	{
		_D1 += *this;
		j++;
	}
	
	return j;
}

const TPPeriod& TPPeriod::operator-=(const TPPeriod& rhs)
{
	TPPeriod p1 = rhs;
	p1.converttolowerperiod();
	converttolowerperiod();
	
	//I was just thinking
	//If u ever get such an error message on excel spread sheets
	// i.e. "Can not add periods", you wouldnt have a clue
	// on what that means, or what the error is from now
	//would you?
	//Exactly, thats my point. Errors need to be as explicit
	// as possible

	if (myType != p1.myType) THROW("Can not add periods")
	else
	{
		myNumber -= p1.myNumber;
		converttoupperperiod();
	}

	return *this;	

}

const TPPeriod& TPPeriod::operator*=(double rhs)
{
	converttolowerperiod();	
	double n = (double)myNumber * rhs;
	
	if (!(TPGlobals::isInt(n))) THROW("Invaid Period Multiplication Operation")
	
	myNumber = static_cast<int> (n);
	converttoupperperiod();	
	
	return *this;
}

const TPPeriod& TPPeriod::operator/=(double rhs)
{
	converttolowerperiod();	
	double n = (double)myNumber / rhs;
	
	if (!(TPGlobals::isInt(n))) THROW("Invaid Period Division Operation")
	
	myNumber = static_cast<int> (n);
	converttoupperperiod();	
	
	return *this;
}



TPPeriod operator+(const TPPeriod& p1, const TPPeriod& p2)
{
	TPPeriod tmp1 = p1;
	tmp1 += p2;
	return tmp1;
}


TPPeriod operator*(const double rhs, const TPPeriod& p)
{
	TPPeriod tmp = p;
	tmp *= rhs;
	return tmp;
}

template <class Archive>
 void TPPeriod::serialize(Archive& archive, uint32_t const /*version*/)
//void TPPeriod::serialize(Archive& archive)
{
	archive(cereal::make_nvp("Number", myNumber));
	archive(cereal::make_nvp("Type",myType));
}

//#include <TPTools\Serialisation\SerialMacros.h>
////NIKO_SERIALIZATION_IMPL_AND_REGISTER(TPPeriod, "TPPeriod", 1, TPNIKOPOLIS_IMPEXP);
//NIKO_SERIALIZATION_IMPL(TPPeriod,  TPNIKOPOLIS_IMPEXP)
