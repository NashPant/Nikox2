
// DOXYGEN CLASS DOCUMENTATION
/*! \class		TPDepoConversions
 *
 *	\brief	    Conversions between discount factors, yield and depo

	 

   <P>

  
	@param		
 
 *

 *	\version   0.0.0
 *
 *	\date    31/08/2009
 *
 *  \note	
 *
 */

#ifndef TPDepoConversions_347659823764589273465834_
#define TPDepoConversions_347659823764589273465834_

//#include "TPAnalyticUtils.h"
#include <vector>
#include <TPNikopolisUtils.h>

class TPNIKOPOLIS_IMPEXP TPDepoConversions
{
public:
	//Constructors
	TPDepoConversions(const double dRate,				// (I) Value to be converted
					const double dDelDays,			// (I) Days until delivery 
					const double dDaysInYear,		// (I) Days in year (depends on currency)
					const double dYieldYear=365.0);	// (I) Days in yield year (underlying assumption for conversion between yield and discount factor)

	virtual ~TPDepoConversions();

	//Other Methods
	virtual double DFToYield(double dDF);			//Takes a discount factor and returns corresponding yield
	virtual double YieldToDF(double dYield);		//Takes yield and returns corresponding discount factor
	virtual double YieldToDepo(double dYield);		//Takes yield and returns corresponding depo
	virtual double DepoToYield(double dDepo);		//Takes depo and returns corresponding yield
	virtual double DFToDepo(double dDF);			//Takes a discount factor and returns corresponding depo
	virtual double DepoToDF(double dDepo);			//Takes depo and returns corresponding discount factor
	
protected:
	double  m_dDelDays;								//Days until delivery 
	double  m_dDaysInYear;							//Days in year (depends on currency)
	double  m_dYieldYear;							//Days in yield year (underlying assumption for conversion between yield and discount factor)
};

#endif