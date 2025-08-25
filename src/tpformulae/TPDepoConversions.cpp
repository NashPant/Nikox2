#include "TPDepoConversions.h"
#include "TPTools/TPException.h"
#include <cmath>

//Constructors
TPDepoConversions::TPDepoConversions(	const double dRate,			// (I) Value to be converted
									const double dDelDays,		// (I) Days until delivery
									const double dDaysInYear,	// (I) Days in year (depends on currency)
									const double dYieldYear):	// (I) Days in yield year (underlying assumption for conversion between yield and discount factor)
m_dDelDays(dDelDays),
m_dDaysInYear(dDaysInYear),
m_dYieldYear(dYieldYear)
{
	if(m_dDelDays == 0.0)
	{
		THROW("TPDepoConversions::TPDepoConversions.  m_dDelDays appears as denominator hence must be non-zero");
	}

	if(m_dDaysInYear == 0.0)
	{
		THROW("TPDepoConversions::TPDepoConversions..m_dDaysInYear appears as denominator hence must be non-zero");
	}

	if(m_dYieldYear == 0.0)
	{
		THROW("TPDepoConversions::TPDepoConversions. m_dYieldYear appears as denominator hence must be non-zero");
	}
}

TPDepoConversions::~TPDepoConversions()
{}

//Other Methods
//Convert a discount factor into corresponding yield
double TPDepoConversions::DFToYield(double dDF)
{
	if(dDF <= 0.0)
	{
		THROW("TPDepoConversions::DFToYield. Discount factors must be positive");
	}

	return -log(dDF)*m_dYieldYear/m_dDelDays;
}

//Convert yield into corresponding discount factor
double TPDepoConversions::YieldToDF(double dYield)
{
  return exp(-dYield*m_dDelDays/m_dYieldYear);
}

//Convert yield into corresponding depo
double TPDepoConversions::YieldToDepo(double dYield)
{
	if(m_dDelDays <= m_dDaysInYear)
	{
		return (exp(dYield*m_dDelDays/m_dYieldYear)-1.0)*m_dDaysInYear/m_dDelDays;
	}
	else
	{
		return exp(dYield*m_dDaysInYear/m_dYieldYear) - 1.0;
	}
}

//Convert depo into corresponding yield
double TPDepoConversions::DepoToYield(double dDepo)
{
	double dValue = 0.0;

	if(m_dDelDays <= m_dDaysInYear)
	{
		dValue = 1.0 + dDepo*m_dDelDays/m_dDaysInYear;
	}
	else
	{
		dValue = pow((1.0 + dDepo),(m_dDelDays/m_dDaysInYear));
	}

	return log(dValue)*m_dYieldYear/m_dDelDays;

}

//Convert a discount factor into corresponding depo
double TPDepoConversions::DFToDepo(double dDF)
{
	double dYield = DFToYield(dDF);
	return YieldToDepo(dYield);
}

//Convert depo into corresponding discount factor
double TPDepoConversions::DepoToDF(double dDepo)
{
	double dYield = DepoToYield(dDepo);
	return YieldToDF(dYield);
}
