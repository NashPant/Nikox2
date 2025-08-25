#include "TPNumericalMethods\TPRootFinder.h"
#include "TPIRBlackAnalytics.h"
#include "TPTools/time/TPDateUtils.h"
#include "TPMaths/\randomnumbers/TPNumberGeneratorFactory.h"
#include "TPMaths/TPMathDefinitions.h"
#include "TPTools/TPMathException.h"
#include "TPMaths/Constants.h"
#include "TPMaths/Maths.h"

int		 ourMaxExpansionIteration = 6;





TPIRImpliedVolFinder_BIS::TPIRImpliedVolFinder_BIS(
    const double& underlying,
    const double& strike,
    const double& expiry,
    const TPIRBlackAnalytics::TPOptionType& optionType,
    const TPIRBlackAnalytics::TPModelType&    modeltype) :
    myDTau(expiry),
    myForward(underlying),
    myStrike(strike),
    myModelType(modeltype),
    myOptionType(optionType)
{
}

double TPIRImpliedVolFinder_BIS::operator () (const double& x) const
{
    double price;

    // constraint the volatility to stay above or equal to zero, 
    double x1 = std::max(0., x);
    TPIRBlackAnalytics::price(myForward, myStrike, x1, myDTau, 1.0, myOptionType, myModelType, price);

    return price*10000.;
}


double TPIRImpliedVolFinder_BIS::derivative(const double& x) const
{


    double price, delta, gamma, vega, theta;
    double x1 = std::max(0.001, x);
    // We are just interested in the vega
    TPIRBlackAnalytics::priceAndGreks(myForward, myStrike, x1, myDTau, 1.0, 1.0, myOptionType, myModelType, price, delta, gamma, theta, vega);

    return vega*10000.;
}


//----------------------------------------------------------------------------------
// For Root Finding .. Basically th same constructor except the premium..
ImpliedVolFinder::ImpliedVolFinder(const double& Underlying,
					   const double& strike,
					   const double& mypremium,
					   const double& Expiry,
					   const TPIRBlackAnalytics::TPOptionType& myOptionType,
					   const TPIRBlackAnalytics::TPModelType&   modeltype,	
					   const TP_GradientMethod& gradient_):ObjectiveFunction(gradient_),
									premium_(mypremium), myDTau(Expiry),
								   myForward(Underlying),myStrike(strike),
								   myoptionType_(myOptionType),mymodelType_(modeltype)
{
}

double ImpliedVolFinder::operator () (const double& x) const
{
  double price;

  // constraint the volatility to stay above or equal to zero, 
  double x1 = MAX(0.,x);
  TPIRBlackAnalytics::price(myForward,myStrike,x1,myDTau,1.0,myoptionType_,mymodelType_,price);

	return (price - premium_)*10000.;
}


double ImpliedVolFinder::derivative(const double& x) const
{

	if(myGradientMethod != TP_ANALYTIC)
	{
		return ObjectiveFunction::derivative(x);
	}
	else
	{
    double price, delta, gamma, vega, theta;
	double x1 = MAX(0.001,x);
    // We are just interested in the vega
    TPIRBlackAnalytics::priceAndGreks(myForward,myStrike,x1,myDTau,1.0,1.0,myoptionType_,mymodelType_,price,delta, gamma, theta, vega);

		return vega*10000.;
	}
}

//===============================
// IMPLIED STRIKE FROM DELTA
//================================
ImpliedStrikeDeltaFinder::ImpliedStrikeDeltaFinder(const double& Underlying,
	const double& delta,
	const double& Expiry,
	double vol,
	const TPIRBlackAnalytics::TPOptionType&	 myOptionType,
	const TPIRBlackAnalytics::TPModelType&   modeltype,
	const TP_GradientMethod& gradient_) :ObjectiveFunction(gradient_),
	fwd_(Underlying),
	delta_(delta),
	tau_(Expiry),
	vol_(vol),
	myoptionType_(myOptionType), mymodelType_(modeltype)
{
	// check the sign to make sure not dodgy numbers passed in
	if (myOptionType == TPIRBlackAnalytics::TPOptionType::TPCALL)
		TP_REQUIRE(delta_ > 0, "Expecting positive delta for call options");
	else if (myOptionType == TPIRBlackAnalytics::TPOptionType::TPPUT)
		TP_REQUIRE(delta_ < 0, "Expecting negative delta for call options");
}
double ImpliedStrikeDeltaFinder::operator () (const double& x) const
{

	// constraint the volatility to stay above or equal to zero, 
	double price, delta, gamma, vega, theta;
	TPIRBlackAnalytics::priceAndGreks(fwd_, x, vol_, tau_, 1.0, 1., myoptionType_, mymodelType_, price, delta, gamma, theta, vega);

	return (delta_ - delta)*100.;
}


//-----------------------------------------------------------------
//
//	SABR Implied Alpha Finder.
// Given Beta, rho, volvol and ATM Black Vol
//===============================================================
ImpliedSABRAlphaFinder::ImpliedSABRAlphaFinder(const double& Underlying,
					   const double& BlackVol,
					   const double& Expiry,
   					   const double& b,
					   const double& r,
					   const double& vlvl,
					   const TP_GradientMethod& gradient_):ObjectiveFunction(gradient_),
								   forward(Underlying),
								   tau(Expiry), m_vol_(BlackVol),
								   beta(b),rho(r),volvol(vlvl)						
{
}

double ImpliedSABRAlphaFinder::operator () (const double& x) const
{

  // rename the x (which is alpha in the model) to make it a bit more easier to read.

 // constraint the volatility to stay above or equal to zero, 
  double x1 = MAX(0.,x);

  double alpha = x1;
  double alpha2 = x1*x1;

 
  double ff2 = forward * forward;
  double oneMinusBeta = 1.0 - beta;
  double oneMinusBeta2 = oneMinusBeta * oneMinusBeta;
  double f_oneMinusBeta = pow(forward, oneMinusBeta);
  double f_oneMinusBeta2 = pow(ff2, oneMinusBeta);
  
  double f1 = oneMinusBeta2/24*alpha2/f_oneMinusBeta2;
  double f2 = rho * alpha * volvol * beta / 4.0 /f_oneMinusBeta ;
  double f3 = (2 - 3 * rho * rho) / 24.0 * volvol * volvol;

  double term = 1.0 + (f1 + f2 + f3)*tau;

  double sigma_atm = alpha/f_oneMinusBeta*term; 
  return (sigma_atm - m_vol_)*10000.;

}


double ImpliedSABRAlphaFinder::derivative(const double& x) const
{

	if(myGradientMethod != TP_ANALYTIC)
	{
		return ObjectiveFunction::derivative(x);
	}
	else
	{
		// Compute the gradient analytically from formula 2.18 in Hagan's paper.
		  double x1 = MAX(0.,x);

		  double alpha = x1;
		  double alpha2 = x1*x1;

 
		  double ff2 = forward * forward;
		  double oneMinusBeta = 1.0 - beta;
		  double oneMinusBeta2 = oneMinusBeta * oneMinusBeta;
		  double f_oneMinusBeta = pow(forward, oneMinusBeta);
		  double f_oneMinusBeta2 = pow(ff2, oneMinusBeta);
  
		  double f1 = oneMinusBeta2/24.*alpha2/f_oneMinusBeta2;
		  double f1grad = oneMinusBeta2/12.*alpha/f_oneMinusBeta2;
		  double f2 = rho * alpha * volvol * beta / 4.0 /f_oneMinusBeta ;
		  double f2grad = rho *  volvol * beta / 4.0 /f_oneMinusBeta ;	
		  double f3 = (2 - 3 * rho * rho) / 24.0 * volvol * volvol;

		  double term = 1.0 + (f1 + f2 + f3)*tau;

		  double grad = 1./f_oneMinusBeta*term +
						alpha/f_oneMinusBeta*(  f1grad + f2grad )*tau; 


		  return grad*10000;
	}
}





//----------------------------------------------------------------------------------
void TPIRBlackAnalytics::Black(const double& forward,
           const double& strike,
           const double& volatility,
           const double& dt,
           const double& multiplier,
           TPIRBlackAnalytics::TPOptionType  callput,
           double&       price,
           bool          doGreeks,
           const double& discount, // For the theta
           double*       delta,
           double*       gamma,
           double*       theta,
           double*       vega)
{
  if (dt <= 0.0 || volatility == 0.0 || strike <= 0.0)
  {
    double payoff =  0.0;
    if (callput == TPIRBlackAnalytics::TPCALL) payoff = MAX(forward - strike, 0.0) ;
    else if (callput == TPIRBlackAnalytics::TPPUT) payoff = MAX(strike - forward, 0.0) ;
    else if (callput == TPIRBlackAnalytics::TPSTRADDLE) payoff = MAX(strike - forward, forward - strike) ;
    
    price	= multiplier * payoff ;

    if(doGreeks)
    {
      if (callput == TPIRBlackAnalytics::TPCALL) *delta = (payoff > 0.0 ? 1.0 : 0.0) ;
      else if (callput == TPIRBlackAnalytics::TPPUT) *delta = (payoff > 0.0 ? -1.0 : 0.0) ;
      else if (callput == TPIRBlackAnalytics::TPSTRADDLE) *delta = (forward > strike ? 1.0 : -1.0 ) ;

      *delta	*= multiplier ;
	    *gamma	= 0.;	
	    *vega	= 0.;	  
	    *theta	= 0.;
    }
  }
  else
  {
    double sigma = volatility * sqrt(dt);
    double h1 = log(forward / strike) / sigma + 0.5 * sigma;
    double h2 = h1 - sigma;
    double n1 = TPIRBlackAnalytics::NormDist(h1);
    double n2 = TPIRBlackAnalytics::NormDist(h2);

   	double putcall = (callput == TPIRBlackAnalytics::TPCALL || callput == TPIRBlackAnalytics::TPSTRADDLE ? 0 : 1);

    double price1;
    price = multiplier * (forward * (n1 - putcall) - strike * (n2 - putcall));
    price1 = price;

    if(callput == TPIRBlackAnalytics::TPSTRADDLE)
    {
      putcall = 1;
      price += multiplier * (forward * (n1 - putcall) - strike * (n2 - putcall));
      putcall = 0;
    }

    if(doGreeks)
    {
      *delta = multiplier * (n1 - putcall);

      double exp_half_h1_square = exp(-0.5 * h1 * h1);
      double sqrt_2_pi = sqrt(2.0 * PI) ;

      *gamma = multiplier * exp_half_h1_square / (sqrt_2_pi* forward * sigma);
      *vega  = multiplier * forward * sqrt(dt) * exp_half_h1_square / sqrt_2_pi;

      double r = - log(discount) / dt;

      *theta =  r * price1 - multiplier * 0.5 * forward * volatility * exp_half_h1_square / sqrt_2_pi / sqrt(dt);

      if(callput == TPIRBlackAnalytics::TPSTRADDLE)
      {
        putcall = 1;
        *delta += multiplier * (n1 - putcall);
        *gamma += multiplier * exp_half_h1_square / (sqrt_2_pi* forward * sigma);
        *vega  += multiplier * forward * sqrt(dt) * exp_half_h1_square / sqrt_2_pi;
        *theta +=  r * (price - price1) - multiplier * 0.5 * forward * volatility * exp_half_h1_square / sqrt_2_pi / sqrt(dt);
      }
    }
  }
}

//----------------------------------------------------------------------------------
void TPIRBlackAnalytics::BlackExt(const double& forward,
           const double& strike,
           const double& volatility,
           const double& dt,
           const double& multiplier,
           TPIRBlackAnalytics::TPOptionType  callput,
           double&       price,
           bool          doGreeks,
           const double& discount, // For the theta
           double*       delta,
           double*       gamma,
           double*       theta,
           double*       vega,
		   double*		 vanna,
		   double*		 volga,
		   double*       speed)
{
	if (dt <= 0.0 || volatility == 0.0 || strike <= 0.0)
	{
		double payoff =  0.0;
		if (callput == TPIRBlackAnalytics::TPCALL) payoff = MAX(forward - strike, 0.0) ;
		else if (callput == TPIRBlackAnalytics::TPPUT) payoff = MAX(strike - forward, 0.0) ;
		else if (callput == TPIRBlackAnalytics::TPSTRADDLE) payoff = MAX(strike - forward, forward - strike) ;
    
		price	= multiplier * payoff ;

		if(doGreeks)
		{
			if (callput == TPIRBlackAnalytics::TPCALL) *delta = (payoff > 0.0 ? 1.0 : 0.0) ;
			else if (callput == TPIRBlackAnalytics::TPPUT) *delta = (payoff > 0.0 ? -1.0 : 0.0) ;
			else if (callput == TPIRBlackAnalytics::TPSTRADDLE) *delta = (forward > strike ? 1.0 : -1.0 ) ;

			*delta	*= multiplier ;
			*gamma	= 0.;	
			*vega	= 0.;	  
			*theta	= 0.;
			*vanna	= 0.;
			*volga	= 0.;
			*speed	= 0.;
		}
	}
	else
	{
		double sigma = volatility * sqrt(dt);
		double h1 = log(forward / strike) / sigma + 0.5 * sigma;
		double h2 = h1 - sigma;
		double n1 = TPIRBlackAnalytics::NormDist(h1);
		double n2 = TPIRBlackAnalytics::NormDist(h2);

   		double putcall = (callput == TPIRBlackAnalytics::TPCALL || callput == TPIRBlackAnalytics::TPSTRADDLE ? 0 : 1);

		double price1;
		price = multiplier * (forward * (n1 - putcall) - strike * (n2 - putcall));
		price1 = price;

		if(callput == TPIRBlackAnalytics::TPSTRADDLE)
		{
			putcall = 1;
			price += multiplier * (forward * (n1 - putcall) - strike * (n2 - putcall));
			putcall = 0;
		}

		if(doGreeks)
		{
			*delta = multiplier * (n1 - putcall);

			double exp_half_h1_square = exp(-0.5 * h1 * h1);
			double sqrt_2_pi = sqrt(2.0 * PI) ;

			*gamma = multiplier * exp_half_h1_square / (sqrt_2_pi* forward * sigma);
			*vega  = multiplier * forward * sqrt(dt) * exp_half_h1_square / sqrt_2_pi;

			double r = - log(discount) / dt;
			*theta =  r * price1 - multiplier * 0.5 * forward * volatility * exp_half_h1_square / sqrt_2_pi / sqrt(dt);

			if(callput == TPIRBlackAnalytics::TPSTRADDLE)
			{
				putcall = 1;
				*delta += multiplier * (n1 - putcall);
				*gamma += multiplier * exp_half_h1_square / (sqrt_2_pi* forward * sigma);
				*vega  += multiplier * forward * sqrt(dt) * exp_half_h1_square / sqrt_2_pi;
				*theta +=  r * (price - price1) - multiplier * 0.5 * forward * volatility * exp_half_h1_square / sqrt_2_pi / sqrt(dt);
			}

			*vanna = -*vega*h2/(forward*sigma);
			*volga = *vega*h1*h2/volatility;
			*speed = -(*gamma/forward)*(1.0+h1/sigma);
		}
	}
}


////////////
//	
//				Q - PRICE
//
/////////////////////////////////
//----------------------------------------------------------------------------------
void TPIRBlackAnalytics::QBlackScholesPrice(const double& forward,
                     const double& strike,
                     const double& volatility,
                     const double& dt,
                     const double& multiplier,
                     const double& q_param, // The q parameter.
                     TPOptionType  callput,
                     double&       price)
{
  if (dt <= 0.0 || volatility == 0.0 || strike <= 0.0)
  {
    double payoff =  0.0;
    if (callput == TPIRBlackAnalytics::TPCALL) payoff = MAX(forward - strike, 0.0) ;
    else if (callput == TPIRBlackAnalytics::TPPUT) payoff = MAX(strike - forward, 0.0) ;
    else if (callput == TPIRBlackAnalytics::TPSTRADDLE) payoff = MAX(strike - forward, forward - strike) ;
    
    price	= multiplier * payoff ;
  }
  else
  {
    double sigma = q_param*volatility * sqrt(dt);
    double h1 =- log( ( strike /forward -1)*q_param + 1 ) / sigma + 0.5 * sigma;
    double h2 = h1 - sigma;
    double n1 = TPIRBlackAnalytics::NormDist(h1);
    double n2 = TPIRBlackAnalytics::NormDist(h2);

   	double putcall = (callput == TPIRBlackAnalytics::TPCALL || callput == TPIRBlackAnalytics::TPSTRADDLE ? 0 : 1);

    double price1;
    price = multiplier * (forward *(n1/q_param +(1 - 1/q_param -  strike/forward) *n2) -putcall*(forward - strike) );
    price1 = price;

    if(callput == TPIRBlackAnalytics::TPSTRADDLE)
    {
      putcall = 1;
      price += multiplier * (forward *(n1/q_param +(1 - 1/q_param -  strike/forward) *n2) -putcall*(forward - strike) );
      putcall = 0;
    }

  }
}

////////////
//	
//				2Q - PRICE
//
/////////////////////////////////
//----------------------------------------------------------------------------------
void TPIRBlackAnalytics::Two_QBlackScholesPrice(const double& forward,
																					const double& strike,
																					 const double& volatility,
																					 const double& dt,
																					 const double& multiplier,
																					 const double& q_L,
																					 const double& q_R,
																					 const double& A,
																					 TPOptionType  callput,
																					 double&       price)
{
  if (dt <= 0.0 || volatility == 0.0 || strike <= 0.0)
  {
    double payoff =  0.0;
    if (callput == TPIRBlackAnalytics::TPCALL) payoff = MAX(forward - strike, 0.0) ;
    else if (callput == TPIRBlackAnalytics::TPPUT) payoff = MAX(strike - forward, 0.0) ;
    else if (callput == TPIRBlackAnalytics::TPSTRADDLE) payoff = MAX(strike - forward, forward - strike) ;
    
    price	= multiplier * payoff ;
  }
  else
  {
		TP_MATH_REQUIRE( ( q_L != 0) && (q_R != 0) , "TPIRBlackAnalytics::2QBlackScholesPrice. Q parameters cannot be zero",10,true);

		// Solve for the F_L and F_R parameters.
		double A_L1 = A - q_L*volatility * sqrt(dt);
		double A_R1 = A - q_R*volatility * sqrt(dt);
		double N_L = TPIRBlackAnalytics::NormDist(A_L1);
		double N_R = TPIRBlackAnalytics::NormDist(A_R1);
		double N_A= TPIRBlackAnalytics::NormDist(A);

		double P_L = N_A *(1. - 1./q_L) + N_L/q_L;
		double P_R = N_A *(1. - 1./q_R) + N_R/q_R;

		double A_L = 1+ (exp(volatility*q_L*sqrt(dt)*A- 0.5 *volatility*volatility*q_L*q_L*dt ) - 1.)/q_L;
		double A_R = 1+ (exp(volatility*q_R*sqrt(dt)*A- 0.5 *volatility*volatility*q_R*q_R*dt ) - 1.)/q_R;

		double F_L = forward / ( P_L + A_L/A_R *(1-P_R));
		double F_R = forward*A_L / ( P_L*A_R + A_L*(1-P_R));

		double sp1 = F_L*(1+ (exp(volatility*q_L*sqrt(dt)*A- 0.5 *volatility*volatility*q_L*q_L*dt ) - 1.)/q_L);
		double sp2 = F_R*(1+ (exp(volatility*q_R*sqrt(dt)*A- 0.5 *volatility*volatility*q_R*q_R*dt ) - 1.)/q_R);

		double sp3 = F_L*P_L + F_R * (1-P_R);
		double s2q_ofA = F_L * ( 1+ (exp(volatility*q_L*sqrt(dt)*A- 0.5 *volatility*volatility*q_L*q_L*dt ) - 1.)/q_L);
		if(s2q_ofA > strike) // we're top of the branch
		{
			double sigma = q_L*volatility * sqrt(dt);
			 double h1 =- log( ( strike /F_L -1)*q_L + 1 ) / sigma + 0.5 * sigma;
			 double h2 = h1 - sigma;
			double n1 = TPIRBlackAnalytics::NormDist(h1);
			double n2 = TPIRBlackAnalytics::NormDist(h2);

   			double putcall = (callput == TPIRBlackAnalytics::TPCALL || callput == TPIRBlackAnalytics::TPSTRADDLE ? 0 : 1);

			double price1;
			price = multiplier * (F_L *(n1/q_L +(1 - 1/q_L -  strike/forward) *n2) -putcall*(forward - strike) );
			price1 = price;
		}
		else // we're in the second branch
		{
			double sigma = q_R*volatility * sqrt(dt);
			 double h1 =- log( ( strike /F_R -1)*q_R + 1 ) / sigma + 0.5 * sigma;
			 double h2 = h1 - sigma;
			double n1 = TPIRBlackAnalytics::NormDist(h1);
			double n2 = TPIRBlackAnalytics::NormDist(h2);

   			double putcall = (callput == TPIRBlackAnalytics::TPCALL || callput == TPIRBlackAnalytics::TPSTRADDLE ? 0 : 1);

			double price1;
			price = multiplier * (F_R *(n1/q_R +(1 - 1/q_R -  strike/F_R) *n2) -putcall*(forward - strike) );
			price1 = price;
		}

  }

}


///=======================================================
//				DEM-FUNCTIONAL VOLATILITY MODEL
///===================================================
void TPIRBlackAnalytics::DEM_Price(const double& forward,
                     const double& strike,
                     const double& volatility,
                     const double& dt,
                     const double& multiplier,
                     const double& n,  // the atm level
					 const double& s,	// the skew parameter
					 const double& w, // the wing parameter
                     TPOptionType  callput,
                     double&       price)
{
  if (dt <= 0.0 || volatility == 0.0 || strike <= 0.0)
  {
    double payoff =  0.0;
    if (callput == TPIRBlackAnalytics::TPCALL) payoff = MAX(forward - strike, 0.0) ;
    else if (callput == TPIRBlackAnalytics::TPPUT) payoff = MAX(strike - forward, 0.0) ;
    else if (callput == TPIRBlackAnalytics::TPSTRADDLE) payoff = MAX(strike - forward, forward - strike) ;
    
    price	= multiplier * payoff ;
  }
  else
  {
	  // First we need to solve for Z_0;
	  //
	  Z0_finder  myZ0_finder(forward,strike,volatility,dt,n,s,w );
	  TP_RootFindingMethod myrootfindingmethod=TP_BISECTION;
	  TP1DRootFinder myfinder(myrootfindingmethod);
	  myfinder.setAccuracy(1.E-7);
  	  double xmin = -100.;
	  double xmax = 100.;

		double guess = 0.;
		// Find it..
		double Z_0 = myfinder.solve(myZ0_finder,guess,xmin,xmax);
		//
		double NofZ0 =  TPIRBlackAnalytics::NormDist(-Z_0);
		double normal_Z0 =1./(sqrt(2.0 * PI))* exp(-0.5 * Z_0 *Z_0);

		double I_1 =  normal_Z0;
		double I_2 = Z_0*I_1+ NofZ0;
		double I_3 = Z_0*Z_0*I_1+2*I_1;

		double  Sodash =forward/(1.+0.5*s*volatility*volatility*dt);

		double vol2 = volatility*volatility;
		double vol3 = vol2*volatility;
		double call_price = Sodash*(NofZ0+ n*volatility*sqrt(dt)*I_1 + 0.5*s*vol2*dt*I_2 + 1./6.*w*vol3*sqrt(dt*dt*dt)*I_3)-strike* NofZ0;

   	   double putcall = (callput == TPIRBlackAnalytics::TPCALL || callput == TPIRBlackAnalytics::TPSTRADDLE ? 0 : 1);

       price = multiplier * (call_price -putcall*(forward - strike) );


  }
}
//////////////////////////
//		Array of Options
///////////////////
void TPIRBlackAnalytics::priceOptionsVector(const double& forward,
											const std::vector<double>& strikes,
											 const std::vector<double>& volatilities,
											 const double& dt,
											 const std::vector<double>& percentages, 
											 TPOptionType  callput,
											 double&       price,	
											 double&       delta,	
											 double&       gamma,	
											 double&       theta,	
											 double&       vega)
{
	if ( (strikes.size() != percentages.size()) || (strikes.size() != volatilities.size()) )
		THROW("TPIRBlackAnalytics::priceOptionsVector. Not equal sizes")

	if (dt <= 0.0  )
	{
		double payoff =  0.0;
		for(int i=0; i < strikes.size(); i++)
		{
			if (callput == TPIRBlackAnalytics::TPCALL) payoff += percentages[i]*MAX(forward - strikes[i], 0.0) ;
			else if (callput == TPIRBlackAnalytics::TPPUT) payoff += percentages[i]*MAX(strikes[i] - forward, 0.0) ;
			else if (callput == TPIRBlackAnalytics::TPSTRADDLE) payoff += percentages[i]*MAX(strikes[i] - forward, forward - strikes[i]) ;

			if (callput == TPIRBlackAnalytics::TPCALL) delta += percentages[i]*(forward > strikes[i] ? 1.0 : 0.0) ;
			else if (callput == TPIRBlackAnalytics::TPPUT) delta += percentages[i]*(forward < strikes[i] ? -1.0 : 0.0) ;
			else if (callput == TPIRBlackAnalytics::TPSTRADDLE) delta += percentages[i]*(forward > strikes[i] ? 1.0 : -1.0 ) ;

		}
    
		price	=  payoff ;

   
		gamma	= 0.;	
		vega	= 0.;	  
		theta	= 0.;
	}
	else
	{
		double payoff =  0.0;
		double tmpd=0,tmpg=0.,tmpv=0.,tmpt=0.;
		double putcall = (callput == TPIRBlackAnalytics::TPCALL || callput == TPIRBlackAnalytics::TPSTRADDLE ? 0 : 1);
		

		for(int i=0; i < strikes.size(); i++)
		{
			double sigma = volatilities[i] * sqrt(dt);
			double h1 = log(forward / strikes[i]) / sigma + 0.5 * sigma;
			double h2 = h1 - sigma;
			double n1 = TPIRBlackAnalytics::NormDist(h1);
			double n2 = TPIRBlackAnalytics::NormDist(h2);

			payoff += percentages[i]*(forward * (n1 - putcall) - strikes[i] * (n2 - putcall));
			// Greeks
    	    tmpd +=  percentages[i]*(n1 - putcall);

		    double exp_half_h1_square = exp(-0.5 * h1 * h1);
			double sqrt_2_pi = sqrt(2.0 * PI) ;

			tmpg += percentages[i] * exp_half_h1_square / (sqrt_2_pi* forward * sigma);
			tmpv += percentages[i] * forward * sqrt(dt) * exp_half_h1_square / sqrt_2_pi;

			tmpt =  percentages[i]*( price -  0.5 * forward * volatilities[i] * exp_half_h1_square / sqrt_2_pi / sqrt(dt));


		}
		price = payoff;
		delta = tmpd;
		gamma = tmpg;
		vega = tmpv;
		theta = tmpt;
	}

}

//----------------------------------------------------------------------------------
void TPIRBlackAnalytics::White(const double& forward,
           const double& strike,
           const double& volatility,
           const double& dt,
           const double& multiplier,
           TPIRBlackAnalytics::TPOptionType  callput,
           double&       price,
           bool          doGreeks,
           const double& discount, // For the theta
           double*       delta,
           double*       gamma,
           double*       theta,
           double*       vega)
{
	if(volatility==0 || dt <= 0.0) // cases when option value is just intrinsic value
	{
    double payoff =  0.0;
    if (callput == TPIRBlackAnalytics::TPCALL) payoff = MAX(forward - strike, 0.0) ;
    else if (callput == TPIRBlackAnalytics::TPPUT) payoff = MAX(strike - forward, 0.0) ;
    else if (callput == TPIRBlackAnalytics::TPSTRADDLE) payoff = MAX(strike - forward, forward - strike) ;

    price	= multiplier * payoff ;

    if(doGreeks)
    {
      if (callput == TPIRBlackAnalytics::TPCALL) *delta = (payoff > 0.0 ? 1.0 : 0.0) ;
      else if (callput == TPIRBlackAnalytics::TPPUT) *delta = (payoff > 0.0 ? -1.0 : 0.0) ;
      else if (callput == TPIRBlackAnalytics::TPSTRADDLE) *delta = (forward > strike ? 1.0 : -1.0 ) ;

		*gamma	= 0.;	
	    *vega	= 0.;	 
	    *theta	= 0.;
    }
	}
	else
	{	
    double sigma = volatility * sqrt(dt);
    double intrinsic = forward - strike;
    double d = intrinsic / sigma;
    double n = TPIRBlackAnalytics::NormDist(d);

    double sqrt_2_pi = sqrt(2.0 * PI) ;
		double gauss = 1 / sqrt_2_pi * exp(- d * d / 2.0);

   	double putcall = (callput == TPIRBlackAnalytics::TPCALL || callput == TPIRBlackAnalytics::TPSTRADDLE ? 0 : 1);

    price = multiplier * ( intrinsic * ( n - putcall) + sigma * gauss);

    if(doGreeks)
    {
      *delta = multiplier * (n - putcall);
      *gamma = multiplier * gauss / sigma;
      *vega  = multiplier * sqrt(dt) * gauss;

      double r = - log(discount) / dt;

      *theta =  r * price - multiplier * volatility * 0.5 * gauss / sqrt(dt);

      if(callput == TPIRBlackAnalytics::TPSTRADDLE)
      {
        putcall = 1;
		price += multiplier * (intrinsic * (n - putcall) + sigma * gauss);
        *delta += multiplier * (n - putcall);
        *gamma += multiplier * gauss / sigma;
        *vega  += multiplier * sqrt(dt) * gauss;
        *theta +=  r * price - multiplier * 0.5 * gauss / sqrt(dt);
      }
    }
  }

/*
    
    if(callput == TPIRBlackAnalytics::TPSTRADDLE ) THROW("TPIRBlackAnalytics::White: Straddle not implemented yet.")

  	double dStdDev = volatility * sqrt(dt);

    double diff = (callput == TPIRBlackAnalytics::TPCALL ? forward - strike : strike - forward);
    double s = (callput == TPIRBlackAnalytics::TPCALL ? 1.0 : -1.0);
   	double putcall = (callput == TPIRBlackAnalytics::TPCALL || callput == TPIRBlackAnalytics::TPSTRADDLE ? 0 : 1);

		double dNorm = diff / dStdDev;
		double dExpTerm = 1 / sqrt(2.0 * PI) * exp(- dNorm * dNorm / 2.0);
		double dTerm1 = dStdDev * dExpTerm;
		double dTerm2 = s * forward * TPIRBlackAnalytics::NormDist(dNorm);
		double dTerm3 = s * strike * TPIRBlackAnalytics::NormDist(dNorm);

		price= multiplier * (dTerm1 + dTerm2 - dTerm3);

    if(doGreeks)
    {
		  *delta = multiplier * TPIRBlackAnalytics::NormDist((double)(s)*dNorm) - putcall;
		  *gamma = multiplier * dExpTerm / dStdDev;
		  *vega = multiplier * dExpTerm * sqrt(dt) / 10.;

      double r = - log(discount) / dt;
		  *theta= r * price + multiplier * 0.5 * volatility * dExpTerm / sqrt(dt);    // No yet implemented
    }
	} 
  */
}

//----------------------------------------------------------------------------------
void TPIRBlackAnalytics::WhiteExt(const double& forward,
           const double& strike,
           const double& volatility,
           const double& dt,
           const double& multiplier,
           TPIRBlackAnalytics::TPOptionType  callput,
           double&       price,
           bool          doGreeks,
           const double& discount, // For the theta
           double*       delta,
           double*       gamma,
           double*       theta,
           double*       vega,
		   double*       vanna,
		   double*       volga,
		   double*		 speed)
{
	if(volatility==0 || dt <= 0.0) // cases when option value is just intrinsic value
	{
		double payoff =  0.0;
		if (callput == TPIRBlackAnalytics::TPCALL) payoff = MAX(forward - strike, 0.0) ;
		else if (callput == TPIRBlackAnalytics::TPPUT) payoff = MAX(strike - forward, 0.0) ;
		else if (callput == TPIRBlackAnalytics::TPSTRADDLE) payoff = MAX(strike - forward, forward - strike) ;

		price	= multiplier * payoff ;

			if(doGreeks)
			{
			  if (callput == TPIRBlackAnalytics::TPCALL) *delta = (payoff > 0.0 ? 1.0 : 0.0) ;
			  else if (callput == TPIRBlackAnalytics::TPPUT) *delta = (payoff > 0.0 ? -1.0 : 0.0) ;
			  else if (callput == TPIRBlackAnalytics::TPSTRADDLE) *delta = (forward > strike ? 1.0 : -1.0 ) ;

				*gamma	= 0.;	
				*vega	= 0.;	 
				*theta	= 0.;
				*vanna	= 0.;
				*volga	= 0.;
				*speed	= 0.;
			}
	}
	else
	{	
		double sigma = volatility * sqrt(dt);
		double intrinsic = forward - strike;
		double d = intrinsic / sigma;
		double n = TPIRBlackAnalytics::NormDist(d);

		double sqrt_2_pi = sqrt(2.0 * PI) ;
		double gauss = 1 / sqrt_2_pi * exp(- d * d / 2.0);

   		double putcall = (callput == TPIRBlackAnalytics::TPCALL || callput == TPIRBlackAnalytics::TPSTRADDLE ? 0 : 1);

		price = multiplier * ( intrinsic * ( n - putcall) + sigma * gauss);

		if(callput == TPIRBlackAnalytics::TPSTRADDLE)
		{
			putcall = 1;
			price += multiplier * ( intrinsic * ( n - putcall) + sigma * gauss);
			putcall = 0;
		}

		if(doGreeks)
		{
			*delta = multiplier * (n - putcall);
			*gamma = multiplier * gauss / sigma;
			*vega  = multiplier * sqrt(dt) * gauss;

			double r = - log(discount) / dt;
			*theta =  r * multiplier * ( intrinsic * ( n - putcall) + sigma * gauss) - multiplier * 0.5 * volatility * gauss / sqrt(dt) + putcall*2.0*r*intrinsic;

			if(callput == TPIRBlackAnalytics::TPSTRADDLE)
			{
				putcall = 1;
				*delta += multiplier * (n - putcall);
				*gamma += multiplier * gauss / sigma;
				*vega  += multiplier * sqrt(dt) * gauss;
				*theta +=  r * multiplier * ( intrinsic * ( n - putcall) + sigma * gauss) - multiplier * 0.5 * volatility * gauss / sqrt(dt) + putcall*2.0*r*intrinsic;
			}	

			*vanna = -*vega*d/sigma;
			*volga = *vega*d*d/volatility;
			*speed = -(*gamma*d/sigma);
		}
	}
}

////////////////////////////////////////////////////////////
//
//	EXPECTATION OF ( Foward^m x ONE_{Forward^n > Strike^n} )
//
////////////////////////////////////////////////
void TPIRBlackAnalytics::ExpectationOf_Sm_times_ONE_Sn_Gt_Kn(const double& Fwd,
										   const double& strike,
										   const double& vol,
										   const double& dt,
										   const double& m_power,
										   const double& n_power,
										   TPIRBlackAnalytics::TPOptionType  callput,
										   double&       price,
										   const double& discount,
										   double*       delta ,
										   double*       gamma ,
										   double*       theta ,
										   double*       vega )
{
	// Code the call Lognormal case for the moment..
	
	// Check for zero Volatility first
	if(vol ==0 || dt <= 0.0) // cases when option value is just intrinsic value
	{
	    if (callput == TPIRBlackAnalytics::TPCALL) price = pow(Fwd,m_power) *MAX( pow(Fwd,n_power)-pow(strike,n_power) ,0.);
	    else if (callput == TPIRBlackAnalytics::TPPUT) price = pow(Fwd,m_power) *MAX( pow(strike,n_power)-pow(Fwd,n_power),0.);
		else	price = pow(Fwd,m_power); // Simple power case
	
	    if (callput == TPIRBlackAnalytics::TPCALL) *delta = (price > 0.0 ? 1.0 : 0.0) ;
        else if (callput == TPIRBlackAnalytics::TPPUT) *delta = (price > 0.0 ? -1.0 : 0.0) ;
		else	*delta = 1.;
     
		*gamma	= 0.;	
	    *vega	= 0.;	 
	    *theta	= 0.;
	}
	else  // Non-zero Vol Case
	{
		if(m_power <1 )
			THROW("TPIRBlackAnalytics::ExpectationOf_Sm_times_if_Sn_Gt_Kn. M_Power must be greater or equal to  1")

		if(n_power <1 )
			THROW("TPIRBlackAnalytics::ExpectationOf_Sm_times_if_Sn_Gt_Kn. Power must be greater or equal to  1")


		double dStdDev = vol * sqrt(dt);

		double d1 = ( log(Fwd/strike) + vol*vol*dt*(m_power-0.5) ) / dStdDev;  // The standard d1-part for the cumulative dist
		double Fwd0 = pow(Fwd,m_power)*exp(0.5*m_power*(m_power-1.)*vol*vol*dt);
		double Nd1 = TPIRBlackAnalytics::NormDist(d1);

		double putcall = (callput == TPIRBlackAnalytics::TPCALL || callput == TPIRBlackAnalytics::TPSTRADDLE ? 0 : 1);
		double s = (callput == TPIRBlackAnalytics::TPCALL ? 1.0 : -1.0);

		// 
		price = Fwd0 *(putcall +s*Nd1);

		double sqrt_2_pi = sqrt(2.0 * PI) ;
		double nd1 = 1/sqrt_2_pi * exp(- d1*d1/2.);
		
	
		/// Greeks computations..
//		*delta = pow(Fwd,m_power-1.)*exp(0.5*m_power*(m_power-1.)*vol*vol*dt) *
//				(m_power*(putcall +s*Nd1) + s*nd1);

//		*gamma = pow(Fwd,m_power-2.)*exp(0.5*m_power*(m_power-1.)*vol*vol*dt)* 
//				 (m_power*(m_power-1.)*Nd1 + (2*m_power-1.)*nd1 -d1 *nd1 );   // kai 0eos boh0os me auto edo.. ama einai sosto sfura mou!
//		*vega  = 0.;	// TODO
//		*theta = 0.;	// TODO.. 


	}
 


}


////////////////////////////////////////////////////////////
//
//	EXPECTATION OF (S_1(T) * 1_{S_2(T) > K) )
//
////////////////////////////////////////////////
void TPIRBlackAnalytics::ExpectationOf_S1_times_ONE_S2_Gt_K(const double& S_1,
															   const double& S_2,
															   const double& K,
															   const double& vol1,
															   const double& vol2,
															   const double& dt,
															   double& p)
{

	// Check a few trivial cases first.
	if(vol2 ==0 || dt <= 0)
	{
		if(S_2<=K)		p=0;
		else /// simply the expectation of s_p > g.t. K
		{
			p = S_1;
		}
		return;
	}
	// compute modifief d2 term
	double dStdDev = vol2 * sqrt(dt);
	double d2hat = ( log(S_2/K) + vol2*(vol1-0.5*vol2)*dt) / dStdDev;  // The standard d1-part for the cumulative dist
	double Nd2 = TPIRBlackAnalytics::NormDist(d2hat);
	//
	p = S_1*Nd2;


}

////////////////////////////////////////////////////////////
//
//	EXPECTATION OF (S_1(T) *S_2(T)* 1_{S_2(T) > K) )
//
////////////////////////////////////////////////
void TPIRBlackAnalytics::ExpectationOf_S1_S2_times_ONE_S2_Gt_K(const double& S_1,
															   const double& S_2,
															   const double& K,
															   const double& vol1,
															   const double& vol2,
															   const double& dt,
															   double& p)
{

	// Check a few trivial cases first.
	
	if(vol2 ==0 || dt <= 0)
	{
		if(S_2<=K)		p=0;
		else /// simply the expectation of s_p > g.t. K
		{
			p = S_1*S_2;
		}
		return;
	}
	// compute modifief d2 term
	double dStdDev = vol2 * sqrt(dt);
	double d1hat = ( log(S_2/K) + vol2*(vol1+0.5*vol2)*dt) / dStdDev;  // The standard d1-part for the cumulative dist
	double Nd1 = TPIRBlackAnalytics::NormDist(d1hat);
	//
	p = S_1*S_2*exp(vol1*vol2*dt)*Nd1;


}
//----------------------------------------------------------------------------------
TPIRBlackAnalytics::TPBarrierType TPIRBlackAnalytics::toBarrierType(const std::string& str)
{
	if(str == "UI" || str == "AUI" || str == "aui" || str == "ui" ) return TP_UPIN;
	if(str == "UO" || str == "AUO" || str == "auo" || str == "uo" ) return TP_UPOUT;
	if(str == "DO" || str == "ADO" || str == "ado" || str == "do" ) return TP_DOWNOUT;
	if(str == "DI" || str == "ADI" || str == "adi" || str == "di" ) return TP_DOWNIN;
	return TPINVALIDBARRIERTYPE;
}

//----------------------------------------------------------------------------------
TPIRBlackAnalytics::TPDoubleBarrierType TPIRBlackAnalytics::toDoubleBarrierType(const std::string& str)
{
	if(str == "II" || str == "ii" ) return TP_ININ;
	if(str == "OO" || str == "oo" ) return TP_OUTOUT;
	if(str == "IO" || str == "io" ) return TP_INOUT;
	if(str == "OI" || str == "oi" ) return TP_OUTIN;
	return TPINVALIDDOUBLEBARRIERTYPE;
}


//----------------------------------------------------------------------------------
TPIRBlackAnalytics::TPModelType TPIRBlackAnalytics::ToModelType(const std::string& str)
{
	if(str == "NORMAL" || str == "NOR" || str =="N" ) return TPNORMAL;
	if(str == "LOGNORMAL" || str == "LOG" || str == "L" ) return TPLOGNORMAL;
	return				TPINVALIDMODELTYPE;

}

//----------------------------------------------------------------------------------
TPIRBlackAnalytics::TPOptionType TPIRBlackAnalytics::ToOptionType(const std::string& str)
{
	if(str == "FLOOR" || str =="PUT" || str == "P") return TPPUT;
	if(str == "CAP"  ||  str=="CALL" || str == "C") return TPCALL;
	if(str == "STRADDLE"  ||  str=="S") return TPSTRADDLE;
	return				TPINVALIDOPTIONTYPE;

}

//----------------------------------------------------------------------------------
double TPIRBlackAnalytics::NormDist(const double& x)
{
	double CDFa=  0.2316419;
	double CDFb=  0.319381530;
	double CDFc= -0.356563782;
	double CDFd=  1.781477937;
	double CDFe= -1.821255978;
	double CDFf=  1.330274429;
	double CDFh=  0.3989422;   //1/sqrt(2PI)

	double k,temp;

	k=1./(1.+CDFa*fabs(x));
	if(x<-5.)
	{
		return 0.;
	}   
	else if(fabs(x)<EPSILON)
	{
		return 0.5;
	}
	else if(x>5.)
	{
		return 1.;
	}
	else
	{
		double g=k*k;
		temp=1.-CDFh*exp(-x*x/2)*(CDFb*k+CDFc*g+CDFd*g*k+CDFe*g*g+CDFf*g*g*k);
		if(x>0.)
		{
			return temp;
		}
		else
		{
			return (1.-temp);
		}
	}

}

double TPIRBlackAnalytics::NormDistNCR(const double& x, const double& mu, const double& sigma)
{
	return NormDist((x - mu) / sigma);
}

double TPIRBlackAnalytics::NormPdf(const double& x)
{
	const double p1 = Constants::OneOnSqrtTwoPi();
	const double p2 = -0.5*x*x;

	return(p1*Maths::exp(p2));
}

double TPIRBlackAnalytics::NormPdfNCR(const double& x, const double& mu, const double& sigma)
{
	if (sigma <= 0.0)
	{
		THROW("Cannot deal with negative volatilities");
	}

	const double onsigma = 1.0 / sigma;
	return (onsigma*NormPdf((x - mu)*onsigma));
}

//----------------------------------------------------------------------------------
void TPIRBlackAnalytics::price(const double& forward,
                               const double& strike,
                               const double& volatility,
                               const double& dt,
                               const double& multiplier,
                               TPOptionType  callput,
                               TPModelType   modelType,
                               double&       price)
{
	if(volatility < 0.0) 
	{
		THROW("TPIRBlackAnalytics::price(). Volatility is less than zero")
	}
	if(forward < 0.0 && modelType == TPLOGNORMAL) 
	{
		THROW("TPIRBlackAnalytics::price(). Underlying is less than zero and model is lognormal")
	}

	// Check the distribution and call apropriate function
	if(modelType == TPLOGNORMAL)
	{
		Black(forward,strike,volatility,dt,multiplier,callput,price);
	}
	else if(modelType == TPNORMAL)
	{
		White(forward,strike,volatility,dt,multiplier,callput,price);
	}
	else
	{
		THROW("TPIRBlackAnalytics::generate(). Diffusion should be normal or log-normal.")
	}
}

//----------------------------------------------------------------------------------
void TPIRBlackAnalytics::priceAndGreks(const double& forward,
                                       const double& strike,
                                       const double& volatility,
                                       const double& dt,
                                       const double& multiplier,
                                       const double& discount, // For the theta
                                       TPOptionType  callput,
                                       TPModelType   modelType,
                                       double&       price,
                                       double&       delta,
                                       double&       gamma,
                                       double&       theta,
                                       double&       vega)
{
	if(volatility < 0.0) 
	{
		THROW("TPIRBlackAnalytics::price(). Volatility is less than zero")
	}
	if(forward < 0.0 && modelType == TPLOGNORMAL) 
	{
		THROW("TPIRBlackAnalytics::price(). Underlying is less than zero")
	}

	// Check the distribution and call apropriate function
	if(modelType == TPLOGNORMAL)
	{
		Black(forward,strike,volatility,dt,multiplier,callput,price, true, discount, &delta, &gamma, &theta, &vega);
	}
	else if(modelType == TPNORMAL)
	{
		White(forward,strike,volatility,dt,multiplier,callput,price, true, discount, &delta, &gamma, &theta, &vega);
	}
	else
	{
		THROW("TPIRBlackAnalytics::generate(). Diffusion should be normal or log-normal.")
	}
}

void TPIRBlackAnalytics::priceAndGreksExt(const double& forward,
                                       const double& strike,
                                       const double& volatility,
                                       const double& dt,
                                       const double& multiplier,
                                       const double& discount, // For the theta
                                       TPOptionType  callput,
                                       TPModelType   modelType,
                                       double&       price,
                                       double&       delta,
                                       double&       gamma,
                                       double&       theta,
                                       double&       vega,
									   double&		 vanna,
									   double&       volga,
									   double&       speed)
{
	if(volatility < 0.0) 
	{
		THROW("TPIRBlackAnalytics::price(). Volatility is less than zero")
	}
	if(forward < 0.0 && modelType == TPLOGNORMAL) 
	{
		THROW("TPIRBlackAnalytics::price(). Underlying is less than zero")
	}

	// Check the distribution and call apropriate function
	if(modelType == TPLOGNORMAL)
	{
		BlackExt(forward,strike,volatility,dt,multiplier,callput,price, true, discount, &delta, &gamma, &theta, &vega, &vanna, &volga, &speed);
	}
	else if(modelType == TPNORMAL)
	{
		WhiteExt(forward,strike,volatility,dt,multiplier,callput,price, true, discount, &delta, &gamma, &theta, &vega, &vanna, &volga, &speed);
	}
	else
	{
		THROW("TPIRBlackAnalytics::generate(). Diffusion should be normal or log-normal.")
	}
}


//////////////////////////////////////////////////////////////////
//	
//		SAME FOR THE POWER STUFF...power to the people!!
//
/////////////////////////////////////////////////////////////////
void TPIRBlackAnalytics::PowerPrice(const double& forward,
                               const double& strike,
                               const double& volatility,
                               const double& dt,
                               const double& forwardpower,
                               TPOptionType  callput,
                               TPModelType   modelType,
                               double&       price,
							   double&       delta,
   							   double&       gamma,
							   double&       theta,
							   double&       vega)
{
	if(volatility < 0.0) 
	{
		THROW("TPIRBlackAnalytics::PowerPrice(). Volatility is less than zero")
	}
	if(forward < 0.0) 
	{
		THROW("TPIRBlackAnalytics::PowerPrice(). Underlying is less than zero")
	}

	// Check the distribution and call apropriate function
	if(modelType == TPLOGNORMAL)
	{
		PowerBlack(forward,strike,volatility,dt,forwardpower,callput,price,delta,gamma,theta,vega);
	}
	else if(modelType == TPNORMAL)
	{
		THROW("TPIRBlackAnalytics::PowerPrice. Normal Power model not yet implemented")
	}
	else
	{
		THROW("TPIRBlackAnalytics::PowerPrice(). Diffusion should be normal or log-normal.")
	}
}

///
/// Implementation of the Lognormal Black Power Model
//
void TPIRBlackAnalytics::PowerBlack(const double& forward,
								   const double& strike,
								   const double& volatility,
								   const double& dt,
								   const double& forwardpower,		// POwer for the Forward rate.
								   TPIRBlackAnalytics::TPOptionType  callput,
								   double&       price,
								   double&       delta ,
								   double&       gamma ,
								   double&       theta ,
								   double&       vega )
{
	//
	double powfwd		= pow(forward,forwardpower);
	double powstrike	= pow(strike, forwardpower);

	if(volatility==0 || dt <= 0.0) // cases when option value is just intrinsic value
	{

		double payoff =  0.0;

		if (callput == TPIRBlackAnalytics::TPCALL) payoff = MAX(powfwd - powstrike, 0.0) ;
		else if (callput == TPIRBlackAnalytics::TPPUT) payoff = MAX(powstrike - powfwd, 0.0) ;
		else if (callput == TPIRBlackAnalytics::TPSTRADDLE) payoff = MAX(powstrike - powfwd, powfwd - powstrike) ;

		price	= payoff ;

		if (callput == TPIRBlackAnalytics::TPCALL)			delta = (payoff > 0.0 ? 1.0 : 0.0) ;
		else if (callput == TPIRBlackAnalytics::TPPUT)		delta = (payoff > 0.0 ? -1.0 : 0.0) ;
		else if (callput == TPIRBlackAnalytics::TPSTRADDLE) delta = (powfwd > powstrike ? 1.0 : -1.0 ) ;

		gamma		= 0.;	
		vega		= 0.;	 
		theta		= 0.;
	
	}
	else
	{
		// Do some checks for the power.
		if(forwardpower <1. )
			THROW("TPIRBlackAnalytics::ExpectationOf_Sm_times_if_Sn_Gt_Kn. Power must be at least greater or equal to 1")

		double vol2	  = volatility * volatility ;
		// Expectation of F^n . Needed for put-call parity..
		double Fwd0 = powfwd*exp(0.5*forwardpower*(forwardpower-1.)*vol2*dt);
		//
		double sqrt_2_pi = sqrt(2.0 * PI) ;
		// put 1 if put..
   		double putcall = (callput == TPIRBlackAnalytics::TPCALL || callput == TPIRBlackAnalytics::TPSTRADDLE ? 0 : 1);

		double dStdDev = volatility * sqrt(dt);
		

		double d1 = ( log(forward/strike) + vol2*dt*(forwardpower-0.5) ) / dStdDev;  // The standard d1-part for the cumulative dist
		double d2 = ( log(forward/strike) - 0.5*vol2*dt ) / dStdDev;
		double Nd1 = TPIRBlackAnalytics::NormDist(d1);
		double Nd2 = TPIRBlackAnalytics::NormDist(d2);
		double nd1 = 1./sqrt_2_pi *exp(-(d1*d1)/2.);


		/// Use put-call parity, i.e. 
		//	Call - Put = E[F] -K . Solve for Put price to obtain:
		price = (Fwd0 * (Nd1 - putcall) - powstrike * (Nd2 - putcall));

		// if straddle add the put price to the previously computed call price
		if(callput == TPIRBlackAnalytics::TPSTRADDLE)
		{
		  putcall = 1;
		  price += (Fwd0 * (Nd1 - putcall) - powstrike * (Nd2 - putcall));
		}

		
       delta = forwardpower*pow(forward,forwardpower-1.)*exp(0.5*forwardpower*(forwardpower-1.)*vol2*dt)*(Nd1 - putcall);

       gamma = forwardpower*pow(forward,forwardpower-2.)*exp(0.5*forwardpower*(forwardpower-1.)*vol2*dt)*
			    ( exp(-d1*d1/2.) / (volatility*sqrt_2_pi) + (forwardpower-1)*Nd1);

       vega  = forwardpower*(forwardpower-1.)*Fwd0*volatility*dt*Nd1 +Fwd0*forwardpower*sqrt(dt)*nd1;
	   theta = 0.;
	   if(callput == TPIRBlackAnalytics::TPSTRADDLE)
       {
			putcall = 1;
			delta += forwardpower*pow(forward,forwardpower-1.)*exp(0.5*forwardpower*(forwardpower-1.)*vol2*dt)*(Nd1 - putcall);
			gamma *=2. ; // this is wrong
			vega  *= 2.; // this is wrong
			theta *= 2.; // this is wrong
       }

	}
}

//----------------------------------------------------------------------------------
double TPIRBlackAnalytics::getImpliedVol(const double& forward,
                               const double& strike,
                               const double& dt,
                               TPOptionType  callput,
                               TPModelType   modelType,
                               const double& premium, 
                               double guess,
                               const TP_RootFindingMethod& myrootfindingmethod) 
{
	ImpliedVolFinder	myvolfinder(forward,strike,premium,dt,callput,modelType,TP_ANALYTIC);
	TP1DRootFinder myfinder(myrootfindingmethod);

	double xmin = 0.0001;
	double xmax = 1000.;

	// Find it..
	return myfinder.solve(myvolfinder,guess,xmin,xmax);
}


//----------------------------------------------------------------------------------
double TPIRBlackAnalytics::getImpliedVolBoost(const double& forward,
	const double& strike,
	const double& dt,
	TPOptionType  callput,
	TPModelType   modelType,
	const double& premium,
	double guess,
	const TP_RootFindingMethod& myrootfindingmethod)
{
	ImpliedVolFinder	myvolfinder(forward, strike, premium, dt, callput, modelType, TP_ANALYTIC);
	//TP1DRootFinder myfinder(myrootfindingmethod);

	double vol_f;
	if (myrootfindingmethod == TP_RootFindingMethod::TP_BISECTION)
	{
		 vol_f = TP::Core::solveWithTOMS748(myvolfinder, premium*1., 0., 1000.);
	}
	else
	{
		const auto digits = static_cast<int>(std::numeric_limits<double>::digits * 0.8);
		const uintmax_t maxIteration = 30;
		auto iteration = maxIteration;
		 vol_f = boost::math::tools::newton_raphson_iterate([=](const double vol)
		{
			double price, delta, gamma, theta, vega;
			TPIRBlackAnalytics::priceAndGreks(forward,strike, vol, dt, 1., 1., callput, modelType, price, delta, gamma, theta, vega);
			const auto error = price - premium;
			const auto slope = vega;
			return std::make_pair(error, slope);
		}, 0.5, 0.0, 10.0*premium, digits, iteration);

	}
	return vol_f;
}

//----------------------------------------------------------------------------------
double TPIRBlackAnalytics::equivalentVolatility(
                const double&  forward,         //! forward   
                const double&  strike,          //! strike
                const double&  timeToExpiry,    //! time to expiry in fraction of year
                const double&  alpha,           //! initial alpha-volatility value in SABR model
                const double&  beta,            //! power of the stock diffusion
                const double&  voVol,           //! volatility of the alpha-vol
                TPModelType    volatilityType,  //! indicates the type of volatility which must be returned
                const double& rho)
{

  // No initial alpha-volatility - there is no diffusion specified
  if(alpha == 0.0) return 0.0;

  // The formula can be decomposed in big blocks as:
  // sigma = alpha * numerator / denominator * xsiFactor(xsi) * (1 + (f1 + f2 + f3) * timeToExpiry)
  // some of the terms are common to the normal and log-normal case:
  //        xsiFactor, denominator, f2 and f3
  // some of them are specific:
  //        xsi, numerator and f1

  // Common terms
  double fK = forward * strike;
  double oneMinusBeta = 1.0 - beta;
  double oneMinusBeta2 = oneMinusBeta * oneMinusBeta;
  double oneMinusBeta4 = oneMinusBeta2 * oneMinusBeta2;
  double fK_oneMinusBeta = pow(fK, oneMinusBeta);
  
  double log_f_K = log(forward/strike);
  double log_f_K2 = log_f_K * log_f_K;
  double log_f_K4 = log_f_K2 * log_f_K2;

  double f2 = rho * alpha * voVol * beta / 4.0 / sqrt(fK_oneMinusBeta);
  double f3 = (2 - 3 * rho * rho) / 24.0 * voVol * voVol;

  double denominator = 1.0 + oneMinusBeta2 / 24.0 * log_f_K2
                           + oneMinusBeta4 / 1920.0 * log_f_K4;

  double xsi = 0.0;
  double numerator = 1.0;
  double f1_factor = 1.0;

  switch(volatilityType)
  {
  case TPLOGNORMAL:
    xsi = voVol / alpha * sqrt(fK_oneMinusBeta) * log_f_K;
    numerator = 1.0 / sqrt(fK_oneMinusBeta);

    break;
  case TPNORMAL:
    {
      double fK_beta_over_2 = pow(fK, (beta / 2.0));
      xsi = voVol / alpha * (forward - strike) / fK_beta_over_2;
      numerator = 1 + log_f_K2 / 24.0 + log_f_K4 / 1920.0;
      numerator *= fK_beta_over_2;
      f1_factor = 0.0;
    }
 
    break;
  default:
    THROW("TPIRBlackAnalytics::getEquivalentBlackVolatility: Invalid Volatility Type")
  }

  // inthe case voVol == 0.0
  // It is obtained by a taylor expansion of 
  // log( (sqrt(1 - 2 * correl * xsi + xsi^2) + xsi - correl ) / (1 - correl) )
  double xsiFactor = 1.0;
  if(fabs(xsi)> 1.E-12)// this is to correct some fucked up numerics..
  {
    double spy = sqrt(1 - 2.0 * rho * xsi + xsi * xsi) + xsi - rho;
    double spy2 = spy / (1.0 - rho);
    
    if(spy2 <= 0.0 || spy2 == 1.0) return 0.0;
    
    xsiFactor = xsi / log(spy2);
  }
  
  double f1 = (f1_factor - beta * (2.0 - beta)) * alpha * alpha / 24.0 / fK_oneMinusBeta;

  double result = numerator / denominator * xsiFactor * (1.0 + (f1 + f2 + f3) * timeToExpiry);
  
  return alpha * result ; 

}

// Array of SABR Vols
std::vector<double> TPIRBlackAnalytics::SABRVols(
                const double&  forward,         //! forward   
                const std::vector<double>&  strikes,  // ! array of strikes
                const double&  timeToExpiry,    //! time to expiry in fraction of year
                const double&  alpha,           //! initial alpha-volatility value in SABR model
                const double&  beta,            //! power of the stock diffusion
                const double&  voVol,           //! volatility of the alpha-vol
                TPModelType    volatilityType,  //! indicates the type of volatility which must be returned
                const double& rho)
{

	std::vector<double> res; res.resize(strikes.size());
  // No initial alpha-volatility - there is no diffusion specified
  if(alpha == 0.0) return std::vector<double>(0.0);

  // Common terms
  double oneMinusBeta = 1.0 - beta;
  double oneMinusBeta2 = oneMinusBeta * oneMinusBeta;
  double oneMinusBeta4 = oneMinusBeta2 * oneMinusBeta2;
  for(int i=0;i<strikes.size();i++)
  { 
		   double strike = strikes[i];	 
		  double fK = forward * strike;
		  double fK_oneMinusBeta = pow(fK, oneMinusBeta);
  

		  double log_f_K = log(forward/strike);
		  double log_f_K2 = log_f_K * log_f_K;
		  double log_f_K4 = log_f_K2 * log_f_K2;

		  double f2 = rho * alpha * voVol * beta / 4.0 / sqrt(fK_oneMinusBeta);
		  double f3 = (2 - 3 * rho * rho) / 24.0 * voVol * voVol;

		  double denominator = 1.0 + oneMinusBeta2 / 24.0 * log_f_K2
								   + oneMinusBeta4 / 1920.0 * log_f_K4;

		  double xsi = 0.0;
		  double numerator = 1.0;
		  double f1_factor = 1.0;

		  switch(volatilityType)
		  {
		  case TPLOGNORMAL:
			xsi = voVol / alpha * sqrt(fK_oneMinusBeta) * log_f_K;
			numerator = 1.0 / sqrt(fK_oneMinusBeta);

			break;
		  case TPNORMAL:
			{
			  double fK_beta_over_2 = pow(fK, (beta / 2.0));
			  xsi = voVol / alpha * (forward - strike) / fK_beta_over_2;
			  numerator = 1 + log_f_K2 / 24.0 + log_f_K4 / 1920.0;
			  numerator *= fK_beta_over_2;
			  f1_factor = 0.0;
			}
 
			break;
		  default:
			THROW("TPIRBlackAnalytics::getEquivalentBlackVolatility: Invalid Volatility Type")
		  }

		  // inthe case voVol == 0.0
		  // It is obtained by a taylor expansion of 
		  // log( (sqrt(1 - 2 * correl * xsi + xsi^2) + xsi - correl ) / (1 - correl) )
		  double xsiFactor = 1.0;
		  if(fabs(xsi)> 1.E-12)// this is to correct some fucked up numerics..
		  {
			double spy = sqrt(1 - 2.0 * rho * xsi + xsi * xsi) + xsi - rho;
			double spy2 = spy / (1.0 - rho);
    
			if(spy2 <= 0.0 || spy2 == 1.0) return std::vector<double>(0.0);
    
			xsiFactor = xsi / log(spy2);
		  }
  
		  double f1 = (f1_factor - beta * (2.0 - beta)) * alpha * alpha / 24.0 / fK_oneMinusBeta;

		  double result = numerator / denominator * xsiFactor * (1.0 + (f1 + f2 + f3) * timeToExpiry);
  
		  res[i]=(alpha * result) ; 
  }
  return res;
}


///============================================================
//	
//  Calculate the SABR density given an array of strikes
//
//==================================================================
std::vector<double>	TPIRBlackAnalytics::SABRdensity(
			                const double&  forward,         //! forward   
							const double&  bsvol,
							const std::vector<double>& strike,          //! strike
							const double&  timeToExpiry,    //! time to expiry in fraction of year
							const double&  voVol,           //! voVol
							const double&  beta,            //! power of the stock diffusion
							const double& rho)
{
	//Compute the alpha given the black vol
	double alpha = ImplySABRalpha(forward,timeToExpiry,bsvol,beta,rho,voVol);
	//=====================================================================
	// Now loop through the array of strikes and compute the density.
	//====================================================================
	std::vector<double> densities;
	int n = strike.size();
	densities.resize(n);

	TPModelType	mytype = TPIRBlackAnalytics::TPLOGNORMAL;
	TPIRBlackAnalytics::TPOptionType  callput = TPIRBlackAnalytics::TPCALL;
	double dK = 1./10000. ; // dK 0.1 of a basis point
	double vol_1, vol_0,vol_2;
	double c_1, c_0,c_2;
	for(int i=0; i< n; ++i)
	{
		// 
		
		double K = strike[i];
		vol_1 = equivalentVolatility(forward,K-dK,timeToExpiry,alpha,beta,voVol,mytype,rho);
		vol_0 = equivalentVolatility(forward,K,timeToExpiry,alpha,beta,voVol,mytype,rho);
		vol_2 = equivalentVolatility(forward,K+dK,timeToExpiry,alpha,beta,voVol,mytype,rho);
		// now compute the BS price
		price(forward,K-dK,vol_1, timeToExpiry,1.,callput, mytype, c_1);
		price(forward,K,vol_0, timeToExpiry,1.,callput, mytype, c_0);
		price(forward,K+dK,vol_2, timeToExpiry,1.,callput, mytype, c_2);
		
		//
		densities[i]= (c_1 + c_2 - 2* c_0)/(dK*dK);

	}



	return densities;
}

//===================================================
//
//  Gradient of the ATM SABR vol with respect to the forward. Note that the 
//  naive way of doing things, i.e. s_atm = alpha/ forward^(1-beta) can be less accurate, 
///	especially for long dated swaptions
//==================================================
 double	TPIRBlackAnalytics::dSABR_atmvol_dforward(
                const double&  f,                  
                const double&  tau,    
                const double&  volvol,          
                const double&  beta,           
                const double&  alpha,          
                const double& rho)
{
	double Aterm =  alpha * pow(f,beta-1.);
	double termm2 = pow(1.-beta ,2.)/24. * pow(alpha,2.) *pow(f,2.*beta-2.);
	double termm3 = (1./4.) *rho*beta*alpha*volvol*pow(f,beta-1.);
	double termm4 = (2.-3.*pow(rho,2.))/24. * pow(volvol,2.);
	double Bterm = 1+ (termm2+termm3+termm4)*tau;
	//
	double dAtermdf = alpha * (beta - 1.)*pow(f,beta-2.);
	
	double dBtermdf = ( pow(1.-beta ,2.)/24. * pow(alpha,2.) *(2.*beta-2.)* pow(f,2.*beta-3.) + 
						(1./4.) *rho*beta*alpha*volvol*(beta-1.)*pow(f,beta-2.))*tau;
	double anal_gradient =dAtermdf * Bterm +  Aterm* dBtermdf;

	return anal_gradient;

}

//================================================
//	 Given the ATM BS vol, the forward rate and the chagne in the forward rate
//   we can compute the change in the ATM vol in SABR framework given the
//   SABR parameters.
//
//==============================================
 double	TPIRBlackAnalytics::SABR_DVolDRate(
                 const double&  f, 
				 const double&  bsvol,
				 const double& df,
                const double&  tau,    
                const double&  volvol,          
                const double&  beta,                    
                const double& rho)
{
	double alpha = ImplySABRalpha(f,tau,bsvol,beta,rho,volvol);
	//
	//
	double grad = dSABR_atmvol_dforward(f, tau, volvol, beta,alpha, rho);
	//
	double newvol = bsvol + grad*df;
	//
	return newvol;
}
//=================================================
//
//	Calculate the alpha in the SABR model in order to hit the ATM black volatility
//
//========================================
double TPIRBlackAnalytics::ImplySABRalpha(
                const double& forward,
                const double& timeToExpiry,
                const double& BlackVol,
				const double& beta,
				const double& rho,
				const double& volvol,
				const std::string& root_method,
				const double& accuracy)
{
	TP_RootFindingMethod  myrootfindingmethod = ToRootFindingMethod(root_method);
	TP_GradientMethod mymethod=TP_ANALYTIC;
	/// Define the Root finding functor
	ImpliedSABRAlphaFinder	myalphafinder(forward,BlackVol,timeToExpiry,beta,rho,volvol,mymethod);

	TP1DRootFinder myfinder(myrootfindingmethod);

	double xmin = 0.0001;
	double xmax = 1.;

	double guess = 0.1;
	// Find it..
	myfinder.setAccuracy(accuracy);
	double alpha = myfinder.solve(myalphafinder,guess,xmin,xmax);

	return alpha;
}
// --------------------------------------------------------------------------------
double TPIRBlackAnalytics::normalToLogNormal(
        const double& forward,
        const double& strike,
        const double& timeToExpiry,
        const double& normVolatility)
{
  double m = 0.5 * (forward + strike ) ;
  double m2 = m * m;
  double m3 = m2 * m;
  double m4 = m3 * m;
  double d = (forward - strike);
  double d2 = d * d;
  double d3 = d * d2;
  double d4 = d * d3;

  double var = normVolatility * normVolatility * timeToExpiry;
  double var2 = var * var;

  double result = 1.0 
                + var / m2 / 24.0 
                + d2 / m2 / 12.0 
                + (7.0 / 1920.0 * var2 + 29.0 / 2880.0 * d2 * var 
                +  72.0 / 5760.0 * d4 ) / m4;

  return normVolatility / m * result;
}

// --------------------------------------------------------------------------------
double TPIRBlackAnalytics::logNormalToNormal(
        const double& forward,
        const double& strike,
        const double& timeToExpiry,
        const double& logVolatility)
{
#pragma message ("TODO: Check this formula TPIRBlackAnalytics::logNormalToNormal")
  double dt = timeToExpiry;
  double dt2 = dt * dt;
  double m = 0.5 * ( forward + strike ) ;
  double m2 = m * m;
  double m3 = m2 * m;
  double m4 = m3 * m;

  double d = ( forward - strike );
  double d2 = d * d;

  double a = 1.0 + d2 * ( 1.0 / 12.0 + 72.0 / 5760.0 * d2 / m2) / m2;
  double b = timeToExpiry * (1.0 / 24.0 + 29.0 / 2880.0 * d2  / m2) / m2;

  double c =  7.0 / 1920.0 * dt2  / m4;
  double dN = logVolatility * m;

  double dNormalVolatility = dN / a;

  for ( int i = 0; i < ourMaxExpansionIteration ; i ++ )
  {
    dNormalVolatility = dN / (a + dNormalVolatility * dNormalVolatility *
                    (b + c * dNormalVolatility * dNormalVolatility));
  }

  //  return dNormalVolatility;
  return dNormalVolatility ;
}


//--------------------------------------------------------------------------------
//-------------------- AMERICAN BARRIER OPTION -----------------------------------
void TPIRBlackAnalytics::priceBarrier(
  const double& spot,
  const double& forward,
  const double& strike,
  const double& barrier,
  const double& volatility,
  const double& dt,
  const double& multiplier,
  TPOptionType  callput,
  TPBarrierType barrierType,
  TPModelType   modelType,
  double&       price)
{
	if(volatility < 0.0) 
		THROW("TPIRBlackAnalytics::priceBarrier(). Volatility is less than zero")

  if(forward < 0.0) 
		THROW("TPIRBlackAnalytics::priceBarrier(). Underlying is less than zero")

  double mu = log(forward / spot) / dt - volatility * volatility / 2;
  double B, K;

	if(modelType == TPLOGNORMAL)
	{
    B = log(barrier / spot);
    K = log(strike / spot);
	}
	else if(modelType == TPNORMAL)
	{
    B = barrier;
    K = strike;
	}
	else
		THROW("TPIRBlackAnalytics::priceBarrier(). Diffusion should be normal or log-normal.")

  if(callput == TPPUT && barrierType == TP_UPOUT)
  {
    if(spot > barrier) price = 0.0; // barrier has been touched
    else price = multiplier * 
        ( strike * probaCappedMaxSpot(mu, volatility, K, B, dt) 
        - forward * probaCappedMaxSpot(mu + volatility * volatility, volatility, K, B, dt) );
  }
  else if(callput == TPCALL && barrierType == TP_UPOUT)
  {
    if(spot > barrier) price =  0.0; // barrier has been touched
    else price = multiplier *
        ( forward * probaFlooredMaxSpot(mu + volatility * volatility, volatility, K, B, dt) 
        - strike * probaFlooredMaxSpot(mu, volatility, K, B, dt) );
  }
  else if(barrierType == TP_UPIN)
  {
    double euro_option;
    TPIRBlackAnalytics::price(forward, strike, volatility, dt, 
                              multiplier, callput, modelType, euro_option);

    if(forward > barrier) price = euro_option; // barrier has been touched
    else 
    {
      double upout_option;
      priceBarrier(spot, forward, strike, barrier, volatility, dt, multiplier, callput, TP_UPOUT, modelType, upout_option);
      price = euro_option - upout_option; 
    }
  }
  else if(callput == TPPUT && barrierType == TP_DOWNOUT)
  {
    if(forward < barrier) price = 0.0; // barrier has been touched
    else price =multiplier * 
        ( strike * probaFlooredMinSpot(mu, volatility, K, B, dt)
        - forward * probaFlooredMinSpot(mu + volatility * volatility, volatility, K, B, dt) );
  }
  else if(callput == TPCALL && barrierType == TP_DOWNOUT)
  {
    if(forward < barrier) price =0.0; // barrier has been touched
    else price = multiplier *
        ( forward * probaCappedMinSpot(mu + volatility * volatility, volatility, K, B, dt) 
        - strike * probaCappedMinSpot(mu, volatility, K, B, dt) );
  }
  else if(barrierType == TP_DOWNIN)
  {
    double euro_option;
    TPIRBlackAnalytics::price(forward, strike, volatility, 
                              dt, multiplier, callput, modelType, euro_option);

    if(forward < barrier) price = euro_option; // barrier has been touched
    else
    {
      double downout_option;
      priceBarrier(spot, forward, strike, barrier, volatility, dt, multiplier, callput, TP_DOWNOUT, modelType, downout_option);
      price = euro_option - downout_option; 
    }
  }
  else
  {
		THROW("TPIRBlackAnalytics::priceBarrier(). Wrong type of barrier.")
  }
}

//--------------------------------------------------------------------------------
//-------------------- AMERICAN BARRIER OPTION -----------------------------------
void TPIRBlackAnalytics::priceBarrierAndGreks(
  const double& forward,
  const double& strike,
  const double& barrier,
  const double& volatility,
  const double& dt,
  const double& multiplier,
  TPOptionType  callput,
  TPModelType   modelType,
  TPBarrierType barrierType,
  double&       price,
  double&       delta,
  double&       gamma,
  double&       theta,
  double&       vega)
{
  THROW("TPIRBlackAnalytics::priceBarrierAndGreks(). Greeks on American Barrier not implemented yet.")
}

void TPIRBlackAnalytics::priceDoubleBarrier(
  const double& spot,
  const double& forward,
  const double& strike,
  const double& lower,
  const double& upper,
  const double& volatility,
  const double& dt,
  const double& multiplier,
  TPOptionType  callput,
  TPDoubleBarrierType barrierType,
  TPModelType   modelType,
  double&       price,
  const double& error)
{
	if (lower > upper)
	{
		THROW("TPIRBlackAnalytics::priceDoubleBarrier: Lower barrier above upper barrier");
	}
	
  price = 0.0;
	if (callput == TPCALL) 
	{
		if(barrierType == TP_ININ)
  	{
      if(spot<=lower || spot >= upper)
      {
        // the option has already knocked in
	      TPIRBlackAnalytics::price(forward, strike, volatility, dt, multiplier, TPCALL, modelType, price);
      }
      else
      {
        //using infinite series of single barrier options
        double spy = 0.0;
	      priceBarrier( spot, forward, strike, upper, volatility, dt, multiplier, TPCALL, TP_UPIN, modelType, spy);
        price = spy;
        priceBarrier( spot, forward, strike, lower, volatility, dt, multiplier, TPCALL, TP_DOWNIN, modelType, spy);
        price += spy;

        double additionalterm = 10.0;
        int i = 1;

			  while (fabs(additionalterm) > error)
			  {				
          double Li_1 = pow(lower, i-1);
          double Li = pow(lower, i);
          double L2_i_1 = pow(lower, 2*(i-1));
          double L2i_1 = pow(lower, 2*i-1);
          double L2i = pow(lower, 2*i);
          double L2i_p_1 = pow(lower, 2*i+1);
          double Ui = pow(upper,i);
          double Ui_1 = pow(upper,i-1);
          double U2i = pow(upper,2*i);
          double U2_i_1 = pow(upper,2*(i-1));
          double U2i_1 = pow(upper,2*i-1);
          double U2i_p_1 = pow(upper, 2*i+1);

          double put1 = 0.0;
          double put2 = 0.0;
          double call1 = 0.0;
          double call2 = 0.0;

          priceBarrier(spot, forward, U2i / L2_i_1 / strike, U2i / L2i_1, volatility, dt, multiplier, TPPUT, TP_UPIN, modelType, put1);
          priceBarrier(spot, forward, L2i / U2_i_1 / strike, L2i / U2i_1, volatility, dt, multiplier, TPPUT, TP_DOWNIN, modelType, put2);
          priceBarrier(spot, forward, L2i * strike / U2i , L2i_p_1 / U2i, volatility, dt, multiplier, TPCALL, TP_DOWNIN, modelType, call1);
          priceBarrier(spot, forward, U2i * strike / L2i, U2i_p_1 / L2i, volatility, dt, multiplier, TPCALL, TP_UPIN, modelType, call2);

				  additionalterm = - (Li_1 * strike / Ui * put1
						       + Ui_1 * strike / Li * put2)
					         + Ui / Li * call1
					         + Li / Ui * call2;

          price += additionalterm;
          ++i;
 			  }

        double euro_option = 0.0;
        TPIRBlackAnalytics::price(forward, strike, volatility, dt, multiplier, callput, modelType, euro_option);

        price = std::min(price, euro_option);
      }
		}
		else if (barrierType == TP_OUTOUT)
		{
      if(spot<lower || spot > upper)
      {
        // the option has already knocked out
	      price = 0.0;
      }
      else
      {
        double euro_option = 0.0;
        double barrier = 0.0;

        TPIRBlackAnalytics::price(forward, strike, volatility, dt, multiplier, TPCALL, modelType, euro_option);
        priceDoubleBarrier(spot, forward, strike, lower, upper, volatility, dt, multiplier, TPCALL, TP_ININ, modelType, barrier, error);

			  price = euro_option - barrier;
      }
		}
		else	
		{
			THROW("TPIRBlackAnalytics::priceDoubleBarrier: Only out and in barriers priced");
		}
		
	}
	else if (callput == TPPUT) 
	{	
		if(barrierType == TP_ININ)
		{
      if(spot<=lower || spot >= upper)
      {
        // the option has already knocked in
	      TPIRBlackAnalytics::price(forward, strike, volatility, dt, multiplier, TPPUT, modelType, price);
      }
      else
      {

        //using infinite series of single barrier options
        double spy = 0.0;
	      priceBarrier( spot, forward, strike, upper, volatility, dt, multiplier, TPPUT, TP_UPIN, modelType, spy);
        price = spy;
        priceBarrier( spot, forward, strike, lower, volatility, dt, multiplier, TPPUT, TP_DOWNIN, modelType, spy);
        price += spy;

        double additionalterm = 10.0;
        int i = 1;

			  while (fabs(additionalterm) > error)
			  {				
          double Li_1 = pow(lower, i-1);
          double Li = pow(lower, i);
          double L2_i_1 = pow(lower, 2*(i-1));
          double L2i_1 = pow(lower, 2*i-1);
          double L2i = pow(lower, 2*i);
          double L2i_p_1 = pow(lower, 2*i+1);
          double Ui = pow(upper,i);
          double Ui_1 = pow(upper,i-1);
          double U2i = pow(upper,2*i);
          double U2_i_1 = pow(upper,2*(i-1));
          double U2i_1 = pow(upper,2*i-1);
          double U2i_p_1 = pow(upper, 2*i+1);

          double put1 = 0.0;
          double put2 = 0.0;
          double call1 = 0.0;
          double call2 = 0.0;

          priceBarrier(spot, forward, U2i / L2_i_1 / strike, U2i / L2i_1, volatility, dt, multiplier, TPCALL, TP_UPIN, modelType, call1);
          priceBarrier(spot, forward, L2i / U2_i_1 / strike, L2i / U2i_1, volatility, dt, multiplier, TPCALL, TP_DOWNIN, modelType, call2);
          priceBarrier(spot, forward, L2i * strike / U2i , L2i_p_1 / U2i, volatility, dt, multiplier, TPPUT, TP_DOWNIN, modelType, put1);
          priceBarrier(spot, forward, U2i * strike / L2i, U2i_p_1 / L2i, volatility, dt, multiplier, TPPUT, TP_UPIN, modelType, put2);

				  additionalterm = - (Li_1 * strike / Ui * call1
						       + Ui_1 * strike / Li * call2)
					         + Ui / Li * put1
					         + Li / Ui * put2;

          price += additionalterm;
          ++i;
			  }

        double euro_option = 0.0;
        TPIRBlackAnalytics::price(forward, strike, volatility, dt, multiplier, callput, modelType, euro_option);

        price = std::min(price, euro_option);
      }
		}
		else if (barrierType == TP_OUTOUT)
		{
      if(spot<lower || spot > upper)
      {
        // the option has already knocked out
	      price = 0.0;
      }
      else
      {
        double euro_option = 0.0;
        double barrier = 0.0;

        TPIRBlackAnalytics::price(forward, strike, volatility, dt, multiplier, TPPUT, modelType, euro_option);
        priceDoubleBarrier(spot, forward, strike, lower, upper, volatility, dt, multiplier, TPPUT, TP_ININ, modelType, barrier, error);

			  price = euro_option - barrier;
      }
		}
		else	
		{
			THROW("TPIRBlackAnalytics::priceDoubleBarrier: Only out and in barriers priced");
		}
	}
	else
	{	
			THROW("TPIRBlackAnalytics::priceDoubleBarrier: Option must be call or put.");
	}	
}

void TPIRBlackAnalytics::priceDoubleBarrierAndGreks(
  const double& forward,
  const double& strike,
  const double& lower,
  const double& upper,
  const double& volatility,
  const double& dt,
  const double& multiplier,
  TPOptionType  callput,
  TPModelType   modelType,
  TPDoubleBarrierType barrierType,
  double&       price,
  double&       delta,
  double&       gamma,
  double&       theta,
  double&       vega)
{
  THROW("TPIRBlackAnalytics::priceDoubleBarrierAndGreks(). Greeks on American Barrier not implemented yet.")
}



// --------------------------------------------------------------------------------
// P(X < x, max(X) < b) 
double TPIRBlackAnalytics::probaCappedMaxSpot(const double& mu,
                                              const double& sigma,
                                              const double& x,
                                              const double& b,
                                              const double& T)
{
  if(b < EPS) return 1.0; // In the reflexion principal, the barrier is positive
  double vol = sigma * sqrt(T);
  
  if(vol < EPS) return 0.0;

  double y = std::min(x, b); 

  double h1 = ( y - mu * T) / vol;
  double h2 = ( y - 2.0 * b - mu * T) / vol;

  double spy = exp(2.0 * mu * b / ( sigma * sigma ) );

double tmp = TPIRBlackAnalytics::NormDist(h1) - spy * TPIRBlackAnalytics::NormDist(h2);
  
  return TPIRBlackAnalytics::NormDist(h1) - spy * TPIRBlackAnalytics::NormDist(h2);
}

// --------------------------------------------------------------------------------
// P(max(X) < b) 
double TPIRBlackAnalytics::probaCappedMax(const double& mu,
                                          const double& sigma,
                                          const double& b,
                                          const double& T)
{
  return probaCappedMaxSpot(mu, sigma, b, b, T);
}


// --------------------------------------------------------------------------------
// P(X < x, max(X) > b)
double TPIRBlackAnalytics::probaFlooredMaxSpot(const double& mu,
                                               const double& sigma,
                                               const double& x,
                                               const double& b,
                                               const double& T)
{
  return probaCappedMax(mu, sigma, b, T) - probaCappedMaxSpot(mu, sigma, x, b, T);
}

// --------------------------------------------------------------------------------
// P(X < x, TPMin(X) > b)
double TPIRBlackAnalytics::probaFlooredMinSpot(const double& mu,
                                               const double& sigma,
                                               const double& x,
                                               const double& b,
                                               const double& T)
{
double tmp = probaCappedMax(-mu, sigma, -b, T) - probaCappedMaxSpot(-mu, sigma, -x, -b, T);
  return probaCappedMax(-mu, sigma, -b, T) - probaCappedMaxSpot(-mu, sigma, -x, -b, T);
}

// --------------------------------------------------------------------------------
// P(TPMin(X) > b)
double TPIRBlackAnalytics::probaFlooredMinSpot(const double& mu,
                                               const double& sigma,
                                               const double& b,
                                               const double& T)
{
double tmp = probaCappedMax(-mu, sigma, -b, T);

  return probaCappedMax(-mu, sigma, -b, T);
}


// --------------------------------------------------------------------------------
// P(X < x, TPMin(X) < b)
double TPIRBlackAnalytics::probaCappedMinSpot(const double& mu,
                                              const double& sigma,
                                              const double& x,
                                              const double& b,
                                              const double& T)
{
  return probaFlooredMinSpot(mu, sigma, b, T) 
    - probaFlooredMinSpot(mu, sigma, x, b, T);
}


 // E[ TPMin(tau,T) ]
double TPIRBlackAnalytics::ExpectOfMintauAndT(
		const double& lamda,	
		const double& sigma,	
		const double& b,		
		const double& T)
{
	// 	adjusted barrier
	double bs = b/sigma;
	double ll = lamda /sigma;
	// 
	double h1 = bs/sqrt(T) - ll *sqrt(T);
	double h2 = -bs/sqrt(T) - ll *sqrt(T);

	return (bs/ll) - exp(2*ll*bs)*(T+bs/ll)*TPIRBlackAnalytics::NormDist(h2) +
			  (T-bs/ll)*TPIRBlackAnalytics::NormDist(h1) ;
}


  // E[ tau | tau <T ]
double TPIRBlackAnalytics::ConditionalExpectedHittingTime(
		const double& lamda,	
		const double& sigma,	
		const double& b,		
		const double& T)
{
	// 	adjusted barrier
	double bs = b/sigma;
	double ll = lamda /sigma;
	// 
	double h1 = - bs/sqrt(T) + ll *sqrt(T);  // basicall h1 = -h1 of the above calculation in ExpectOfMintauAndT
	double h2 = -bs/sqrt(T) - ll*sqrt(T);

	double nh1 = TPIRBlackAnalytics::NormDist(h1);
	double nh2 = TPIRBlackAnalytics::NormDist(h2);

	return (bs/ll) * ( nh1 - exp(2*ll*bs)*nh2   ) / (nh1+  exp(2*ll*bs)*nh2);

}


//------------------- ASIAN OPTION 2/3 moments approximation -----------------------
void TPIRBlackAnalytics::priceAsianFixedStrike(
                                               const std::vector<double>& forwards,
                                               const std::vector<double>& vols,
                                               const std::vector<double>& fixingsdates,
                                               const double& strike,
                                               const double& dt,
                                               const double& multiplier,
                                               TPOptionType  callput,
                                               TPAsianModelType   modelType,
                                               double&       price)
{
  if (dt <= 0.0 )
  {
    THROW("TPIRBlackAnalytics::priceAsianFixedStrike: Negative maturity passed in...")
  }
  double epsilon = (callput == TPPUT? -1.0 : 1.0);

  int i = 0;
  double avsf = 0.0;
  double nbfixings = (double)fixingsdates.size();

  while(i < fixingsdates.size() && fixingsdates[i] <= 0.0)
  {
    avsf += forwards[i] / nbfixings;
    ++i;
  }

  double m1 = 0.0;
  double m2 = 0.0;
  double m3third = 0.0;
  double m3last = 0.0;
  double m3 = 0.0;
  int start = i;

  for(i = fixingsdates.size() -1 ; i >= start  ; --i)
  {
    double e_sigma_square = exp(vols[i] * vols[i] * fixingsdates[i]);
    double spy = forwards[i] / nbfixings;
    m1 += spy;
    m2 += spy *  e_sigma_square * ( 2*m1 - spy);

    double m3first = e_sigma_square * spy * spy;
    double m3second = e_sigma_square * spy * m1;
    m3third += m3first;
    m3last += spy * e_sigma_square * m1;

    m3 += spy * e_sigma_square * e_sigma_square * (m3first - 3.0*m3second - 3.0*m3third + 6.0*m3last);
  }

  double mu2 = m2 - m1*m1;
  double mu1 = m1;
  
  double y1 ;
  if(modelType == TPTWOMOMENTS) y1 = m1;
  else if(modelType == TPTHREEMOMENTS) 
  {
    double mu3 = m3 - 3.0 * mu1 * mu2 - mu1*mu1*mu1;
    double z = pow( ((mu3 + sqrt(mu3*mu3+4.0*mu2*mu2*mu2))/2) , 1./3.);
    y1 = mu2 / (z-mu2/z);;
  }


  double delta = mu1-y1;
  double y11 = mu2 + y1*y1;
  double d1 = log(sqrt(y11)/(strike-avsf-delta)) / sqrt(log(y11/y1/y1));
  double d0 = d1 - sqrt(log(y11/y1/y1));

  price = multiplier * epsilon*(y1*TPIRBlackAnalytics::NormDist(epsilon*d1)
                   -(strike-avsf-delta)*TPIRBlackAnalytics::NormDist(epsilon*d0));

}

void TPIRBlackAnalytics::priceAsianFixedStrikeHarmonic(
                                               const std::vector<double>& forwards,
                                               const std::vector<double>& vols,
                                               const std::vector<double>& fixingsdates,
                                               const double& strike,
                                               const double& dt,
                                               const double& multiplier,
                                               TPOptionType  callput,
                                               TPAsianModelType   modelType,
                                               double&       price,
                                               TP_IntegrationMethod integ,
                                               long nbPoints)
{
  if (dt <= 0.0 )
  {
    THROW("TPIRBlackAnalytics::priceAsianFixedStrike: Negative maturity passed in...")
  }
  double epsilon = (callput == TPPUT? -1.0 : 1.0);

  int i = 0;
  double avsf = 0.0;
  double nbfixings = (double)fixingsdates.size();

  while(i < fixingsdates.size() && fixingsdates[i] <= 0.0)
  {
    avsf += 1.0 / forwards[i] / nbfixings;
    ++i;
  }

  double m1 = 0.0;
  double m2 = 0.0;
  double m3third = 0.0;
  double m3last = 0.0;
  double m3 = 0.0;
  int start = i;

  for(i = fixingsdates.size() -1 ; i >= start  ; --i)
  {
    double e_sigma_square = exp(vols[i] * vols[i] * fixingsdates[i]);
    double spy = e_sigma_square / forwards[i] / nbfixings;
    m1 += spy;
    m2 += spy *  e_sigma_square * ( 2*m1 - spy);

    double m3first = e_sigma_square * spy * spy;
    double m3second = e_sigma_square * spy * m1;
    m3third += m3first;
    m3last += spy * e_sigma_square * m1;

    m3 += spy * e_sigma_square * e_sigma_square * (m3first - 3.0*m3second - 3.0*m3third + 6.0*m3last);
  }

  double mu2 = m2 - m1*m1;
  double mu1 = m1;
  
  double y1 ;
  if(modelType == TPTWOMOMENTS) y1 = m1;
  else if(modelType == TPTHREEMOMENTS) 
  {
    double mu3 = m3 - 3.0 * mu1 * mu2 - mu1*mu1*mu1;
    double z = pow( ((mu3 + sqrt(mu3*mu3+4.0*mu2*mu2*mu2))/2) , 1./3.);
    y1 = mu2 / (z-mu2/z);;
  }


  double delta = mu1-y1;
  double y11 = mu2 + y1*y1;

  double v1 = sqrt(log(y11/y1/y1));
  double eta1 = avsf + delta;
  double eta2 = y1*y1/sqrt(y11);
  
  double xsi = log( (1.0/strike - eta1) / eta2) / v1;

  if(eta1 == 0.0)
  {
    double spy = epsilon * strike * TPIRBlackAnalytics::NormDist(epsilon*xsi);
    price = epsilon / eta2 * exp(0.5*v1*v1) * TPIRBlackAnalytics::NormDist(epsilon*(v1+xsi)) 
      - spy; 
  }
  else if(strike < 1.0/eta1)
  {
    AsianIntegral2 asianf(eta1, eta2, -epsilon*v1);
  	TP1DIntegration integrator(integ, nbPoints);
    double lowerbound = -epsilon * xsi;
    double upperbound = lowerbound + 2.0 * nbPoints;
    price = epsilon * integrator.integrate(asianf, lowerbound, upperbound);
    double spy = epsilon * strike * TPIRBlackAnalytics::NormDist(epsilon*xsi);
    price -= spy; 
  }
  else if(strike >= 1.0/eta1 && epsilon == 1)
  {
    price = 0.0;
  }
  else if(strike >= 1.0/eta1 && epsilon == -1)
  {
    AsianIntegral2 asianf(eta1, eta2, v1);
  	TP1DIntegration integrator(integ, nbPoints);
    price = strike - integrator.integrate(asianf, -nbPoints, nbPoints);;
  }
  else
  {
    price = 0.0;
  }
}

 void TPIRBlackAnalytics::priceAsianFloatingStrike(const std::vector<double>& forwards,
                                                                        const std::vector<double>& vols,
                                                                        const std::vector<double>& fixingsdates,
                                                                        const double& forward,
                                                                        const double& vol,
                                                                        const double& dt,
                                                                        const double& multiplier,
                                                                        TPOptionType  callput,
                                                                        TPAsianModelType   modelType,
                                                                        double&       price,
                                                                        TP_IntegrationMethod integ,
                                                                        long nbPoints)
{
  if (dt <= 0.0 )
  {
    THROW("TPIRBlackAnalytics::priceAsianFixedStrike: Negative maturity passed in...")
  }
  double epsilon = (callput == TPPUT? -1.0 : 1.0);

  int i = 0;
  double avsf = 0.0;
  double nbfixings = (double)fixingsdates.size();

  while(i < fixingsdates.size() && fixingsdates[i] <= 0.0)
  {
    avsf += forwards[i] / nbfixings;
    ++i;
  }

  double m1 = 0.0;
  double m2 = 0.0;
  double m3third = 0.0;
  double m3last = 0.0;
  double m3 = 0.0;
  int start = i;
  double x12 = 0.0;

  for(i = fixingsdates.size() -1 ; i >= start  ; --i)
  {
    double e_sigma_square = exp(vols[i] * vols[i] * fixingsdates[i]);
    double spy = forwards[i] / nbfixings;
    m1 += spy;
    m2 += spy *  e_sigma_square * ( 2*m1 - spy);

    double m3first = e_sigma_square * spy * spy;
    double m3second = e_sigma_square * spy * m1;
    m3third += m3first;
    m3last += spy * e_sigma_square * m1;

    m3 += spy * e_sigma_square * e_sigma_square * (m3first - 3.0*m3second - 3.0*m3third + 6.0*m3last);

    x12 += spy * e_sigma_square;
  }

  double mu2 = m2 - m1*m1;
  double mu1 = m1;
  
  double y1 ;
  if(modelType == TPTWOMOMENTS) y1 = m1;
  else if(modelType == TPTHREEMOMENTS) 
  {
    double mu3 = m3 - 3.0 * mu1 * mu2 - mu1*mu1*mu1;
    double z = pow( ((mu3 + sqrt(mu3*mu3+4.0*mu2*mu2*mu2))/2) , 1./3.);
    y1 = mu2 / (z-mu2/z);;
  }

  double delta = mu1-y1;

  double y11 = mu2 + y1*y1;

  double x1 = forward;
  double x11 = exp(vol*vol*dt) * forward * forward;
  double x2 = y1;
  double x22 = y11;
  x12 *= x1;
  x12 -= delta * x1;
  double strike = avsf + delta;
  double beta1 = sqrt(log(x22/x2/x2));
  double beta2 = log(x12/x1/x2)/beta1;
  double gamma = epsilon / sqrt(log(x11/x1/x1)-beta2*beta2);


  double a1 = sqrt(x11);
  double a2 = x1*x12/x2/sqrt(x11);
  double a3 = x1*x1/sqrt(x11);
  double b = beta2;
  double c = strike;
  double d1 = x2*x12/x1/sqrt(x22);
  double d2 = sqrt(x22);
  double d3 = x2*x2/sqrt(x22);
  double f = beta1;

  AsianIntegral asianf1(a1, b, c, d1, f, gamma);
  AsianIntegral asianf2(a2, b, c, d2, f, gamma);
  AsianIntegral asianf3(a3, b, c, d3, f, gamma);
	TP1DIntegration integrator(integ, nbPoints);

  double I1 = integrator.integrate(asianf1, -nbPoints, nbPoints);
  double I2 = integrator.integrate(asianf2, -nbPoints, nbPoints);
  double I3 = integrator.integrate(asianf3, -nbPoints, nbPoints);

  price = multiplier * epsilon*(x1*I1-x2*I2-strike*I3);
}


 void TPIRBlackAnalytics::priceAsianFloatingStrikeHarmonic(
  const std::vector<double>& forwards,
  const std::vector<double>& vols,
  const std::vector<double>& fixingsdates,
  const double& forward,
  const double& vol,
  const double& dt,
  const double& multiplier,
  TPOptionType  callput,
  TPAsianModelType   modelType,
  double&       price,
  TP_IntegrationMethod integ,
  long nbPoints)
{
  if (dt <= 0.0 )
  {
    THROW("TPIRBlackAnalytics::priceAsianFixedStrike: Negative maturity passed in...")
  }
  double epsilon = (callput == TPPUT? -1.0 : 1.0);

  int i = 0;
  double avsf = 0.0;
  double nbfixings = (double)fixingsdates.size();

  while(i < fixingsdates.size() && fixingsdates[i] <= 0.0)
  {
    avsf += 1.0 / forwards[i] / nbfixings;
    ++i;
  }

  double m1 = 0.0;
  double m1_bis = 0.0;
  double m2 = 0.0;
  double m3third = 0.0;
  double m3last = 0.0;
  double m3 = 0.0;
  int start = i;

  for(i = fixingsdates.size() -1 ; i >= start  ; --i)
  {
    double e_sigma_square = exp(vols[i] * vols[i] * fixingsdates[i]);
    double spy = e_sigma_square / forwards[i] / nbfixings;
    m1 += spy;
    m1_bis += 1.0 / forwards[i] / nbfixings;
    m2 += spy *  e_sigma_square * ( 2*m1 - spy);

    double m3first = e_sigma_square * spy * spy;
    double m3second = e_sigma_square * spy * m1;
    m3third += m3first;
    m3last += spy * e_sigma_square * m1;

    m3 += spy * e_sigma_square * e_sigma_square * (m3first - 3.0*m3second - 3.0*m3third + 6.0*m3last);
  }

  double mu2 = m2 - m1*m1;
  double mu1 = m1;
  
  double y1 ;
  if(modelType == TPTWOMOMENTS) y1 = m1;
  else if(modelType == TPTHREEMOMENTS) 
  {
    double mu3 = m3 - 3.0 * mu1 * mu2 - mu1*mu1*mu1;
    double z = pow( ((mu3 + sqrt(mu3*mu3+4.0*mu2*mu2*mu2))/2) , 1./3.);
    y1 = mu2 / (z-mu2/z);;
  }


  double delta = mu1-y1;
  double y11 = mu2 + y1*y1;

  double v1 = sqrt(log(y11/y1/y1));
  double eta1 = avsf + delta;
  double eta2 = y1*y1/sqrt(y11);
  
  double v2 = vol * sqrt(dt);
  double xeta = log( (m1_bis - delta) / y1) / v1 / v2;
  double xetaprime = sqrt(1-xeta*xeta);

  double eta3 = forward * exp(-0.5*v2*v2);

  if(0 && eta1 == 0.0)
  {
    double logeta23 = log(eta2 * eta3);
    double sqroot = sqrt(v1*v1 + v2*v2 + 2.0 * xeta * v1 * v2);
    double xetav12 = xeta * v1 * v2;

    double firstlogarg = epsilon * logeta23 + epsilon * (xetav12 + v2*v2) / sqroot;
    double secondlogarg = epsilon * logeta23 - epsilon * (xetav12 + v1*v1) / sqroot;

    double firstguy = epsilon * eta3 * exp(0.5*v2*v2) * TPIRBlackAnalytics::NormDist(firstlogarg);
    double secondguy = epsilon / eta2 * exp(0.5*v1*v1) * TPIRBlackAnalytics::NormDist(secondlogarg);

    price = firstguy - secondguy;
  }
  else if(1 || eta1 != 0.0)
  {
    AsianIntegral3 asianf(eta1, eta2, eta3, v1, v2, xetaprime, xeta, epsilon);
  	TP1DIntegration integrator(integ, nbPoints);

    double lowerbound = -nbPoints;
    double upperbound = nbPoints;
    price = epsilon * integrator.integrate(asianf, lowerbound, upperbound);
  }
  else
  {
    price = 0.0;
  }
}

const TPIRBlackAnalytics::TPShiftType TPIRBlackAnalytics::toShiftType(const size_t& type)
{
	TPShiftType invalid = TPINVALIDSHIFTTYPE;
	if ( type >= static_cast<size_t>(invalid) ) 
	{
		return TPINVALIDSHIFTTYPE;
	}
	return static_cast<TPShiftType> (type);
}



