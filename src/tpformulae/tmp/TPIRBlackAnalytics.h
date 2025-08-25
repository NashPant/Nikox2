
/*! \namespace	 TPIRBlackAnalytics 
 *
 *	\brief   Implements the Black and Scholes formula for futures. The are two available options \n
 *			 for the underlying model. This option is selected by the enumeration type TPModelType \n
 *
 *		- Normal Model for the futures price process. (enum TPNORMAL): dS = vol dW
 *		- LogNormal Model (enum TPLOGNORMAL): dS = vol S dW
 *			
 *	
			Added implementation of Black Equivalent of a CEV (alpha, beta, sigma) vol \n
			For more details, please refer to "Managing Smile Risk", P Hagan et al. \n
 *
 *		
 *  \note	  The price and the greeks are computed during construction of the object by using the internal \n
 *			  function generate().
 *
 */


#if !defined TP_IRBLACKANALYTICS_TP
#define TP_IRBLACKANALYTICS_TP


#include "TPMaths/TPMathsEnums.h"
#include <TPNumericalMethods\RootFinding\TP1DRootFinder.h>

#include "TPMaths/integration/TP1DIntegration.h"

#include <string>
#include <cmath>

namespace TPIRBlackAnalytics  
{
	//! Simple enumeration to choose 2 moments or 3 moments approximation for asian
//	MATHSIMPEXP enum TPAsianModelType
     enum TPAsianModelType
  {
		TPTWOMOMENTS,
		TPTHREEMOMENTS,
		TPINVALIDASIANMODELTYPE
	};

	//! Simple enumeration to choose arithmetic or harmonic average for asian
//	MATHSIMPEXP enum TPAverageType
     enum TPAverageType
  {
		TP_ARITHMETIC_AVERAGE,
		TP_HARMONIC_AVERAGE,
		TPINVALIDAVERAGEMODELTYPE
	};

  //! Simple enumeration to choose distribution
	//MATHSIMPEXP enum TPModelType
     enum TPModelType
  {
		TPNORMAL,
		TPLOGNORMAL,
		TPINVALIDMODELTYPE
	};

	//! Option Type --> Put = Receiver's, CALL = PAYERS
	//MATHSIMPEXP enum TPOptionType
     enum TPOptionType
  {
		TPPUT,
		TPCALL,
    TPSTRADDLE,
		TPINVALIDOPTIONTYPE
	};

	//! Barrier Type
//	MATHSIMPEXP enum TPBarrierType
    enum TPBarrierType
  {
    TP_NO_BARRIER,
		TP_UPIN,
		TP_UPOUT,
    TP_DOWNIN,
    TP_DOWNOUT,
		TPINVALIDBARRIERTYPE
	};

	//! Double Barrier Type
	//MATHSIMPEXP enum TPDoubleBarrierType
    enum TPDoubleBarrierType
  {
		TP_ININ,
		TP_OUTOUT,
    TP_OUTIN,
    TP_INOUT,
		TPINVALIDDOUBLEBARRIERTYPE
	};


	//! Shift types
	//MATHSIMPEXP enum TPShiftType
    enum TPShiftType
  {
		TP_UP_SHIFT,
		TP_DOWN_SHIFT,
		TP_TWO_SIDED_SHIFT,
		TP_DEFAULT_SHIFT,
		TPINVALIDSHIFTTYPE
	};

	//! Converts from size_t to shift type
  TPNIKOPOLIS_IMPEXP const TPShiftType toShiftType(const size_t& type);

  //! Converts from string to model type
  TPNIKOPOLIS_IMPEXP TPModelType ToModelType(const std::string&);
	
  //! Converts from std::string to asian model type
  TPNIKOPOLIS_IMPEXP TPAsianModelType ToAsianModelType(const std::string&);

  //! Converts from std::string to average type
  TPNIKOPOLIS_IMPEXP TPAsianModelType ToAverageModelType(const std::string&);

  //! Converts from std::string to option type
	TPNIKOPOLIS_IMPEXP TPOptionType ToOptionType(const std::string&);

	//! Converts from std::string to barrier type
  TPNIKOPOLIS_IMPEXP TPBarrierType toBarrierType(const std::string&);

	//! Converts from std::string to double barrier type
  TPNIKOPOLIS_IMPEXP TPDoubleBarrierType toDoubleBarrierType(const std::string&);

  //! Imply myVolatility From Premium.. Bisection or Newton Raphson..
	TPNIKOPOLIS_IMPEXP double getImpliedVol(const double& forward,
                       const double& strike,
                       const double& dt,
                       TPOptionType  callput,
                       TPModelType   modelType,
                       const double& Premium, 
                       double guess = 0.1, 
		  			    const TP_RootFindingMethod& myrootfindingmethod = TP_NEWTONRAPHSON) ;
	// USe boost solvers directly..
	TPNIKOPOLIS_IMPEXP double getImpliedVolBoost(const double& forward,
		const double& strike,
		const double& dt,
		TPOptionType  callput,
		TPModelType   modelType,
		const double& Premium,
		double guess = 0.1,
		const TP_RootFindingMethod& myrootfindingmethod = TP_NEWTONRAPHSON);


	//! P. Hagan stuff
	//! Given a beta, alpha, inital voVol, we get our equivalent blackvol back, the one
	//! we will be using in our Black Pricing.
  //! The passed parameters correspond to the following diffusion:
  //! dS(t) = alpha(t) * S(t)^(beta) dW1(t)
  //! dalpha(t) = voVol * dW2(t)
  //! alpha(0) = alpha
  //! The formula is as stated in p.102 Wilmott of P.Hagan paper
	TPNIKOPOLIS_IMPEXP double	equivalentVolatility(
                const double&  forward,         //! forward   
                const double&  strike,          //! strike
                const double&  timeToExpiry,    //! time to expiry in fraction of year
                const double&  alpha,           //! voVol
                const double&  beta,            //! power of the stock diffusion
                const double&  voVol,           //! initial alpha-volatility value in SABR mofel
                TPModelType    volatilityType,  //! indicates the type of volatility which must be returned
                const double& rho);
	//!
	//! Return an Array of vols given an array of strikes
 TPNIKOPOLIS_IMPEXP std::vector<double>	SABRVols(
                const double&  forward,         //! forward   
                const std::vector<double>&  strikes,          //! strike
                const double&  timeToExpiry,    //! time to expiry in fraction of year
                const double&  alpha,           //! voVol
                const double&  beta,            //! power of the stock diffusion
                const double&  voVol,           //! initial alpha-volatility value in SABR mofel
                TPModelType    volatilityType,  //! indicates the type of volatility which must be returned
                const double& rho);


  // Compute the gradient of the ATM volatility with respect to the forward .
  // This will be used to estimate the changes of the ATM volatility when the forward is 
  // changing..
   TPNIKOPOLIS_IMPEXP double	dSABR_atmvol_dforward(
                const double&  forward,         //! forward   
                const double&  timeToExpiry,    //! time to expiry in fraction of year
                const double&  voVol,           //! voVol
                const double&  beta,            //! power of the stock diffusion
                const double&  alpha,           //! initial alpha-volatility value in SABR mofel
                const double& rho);
   //
   // This function will compute the new ATM vol given the vol, forward and the change in the
   //  forward.
   TPNIKOPOLIS_IMPEXP double	SABR_DVolDRate(
                const double&  forward,         //! forward   
				const double&  bsvol,			// ! ATM Bs VOL
				const double&  df,				// Change in the forward
                const double&  timeToExpiry,    //! time to expiry in fraction of year
                const double&  voVol,           //! voVol
                const double&  beta,            //! power of the stock diffusion
                const double& rho);

  // Compute the implied distribution from the SABR model, by using the well-known
  // approximation \partial^2 Call(K) / \partial K^2
  // Given an array of strikes it returns the density at these points.
   TPNIKOPOLIS_IMPEXP std::vector<double>	SABRdensity(
                const double&  forward,         //! forward   
				const double&  bsvol,
                const std::vector<double>& strike,          //! strike
                const double&  timeToExpiry,    //! time to expiry in fraction of year
                const double&  voVol,           //! voVol
                const double&  beta,            //! power of the stock diffusion
                const double& rho);

  //! Returns the lognormal volatility equivalent to an input normal volatility 
  //! Accuracy to the 4th order
  //! From the P. Hagan paper: Equivalent Black volatility
  //! Mid = 1/2 * ( K + F )
  //! Var = VolNormal^2 * timeToExpiry
  //! VolBlack = VolNormal / Mid * (1 + 1/24 Var / Mid^2 + 1/12 (f-K)^2 / Mid^2 + 1/Mid^4 ( 7 / 1920 * Var^2 + 29 / 2880 * Var + 72/5760 * (f-K)^2 ) )
	TPNIKOPOLIS_IMPEXP double normalToLogNormal(
                const double& forward,
                const double& strike,
                const double& timeToExpiry,
                const double& normVolatility);

  //! Imply the Alpha in the SABR model to match the ATM Black Volatility
  TPNIKOPOLIS_IMPEXP double ImplySABRalpha(
                const double& forward,
                const double& timeToExpiry,
                const double& BlackVol,
				const double& beta,
				const double& rho,
				const double& volvol,
				const std::string& root_method=std::string("NEWTON"),
				const double& accuracy= 1.E-7);
	
  //! Returns the lognormal volatility equivalent to an input normal volatility 
  //! Accuracy to the 4th order
  //! This function is calculated as the inversed (Taylor expansion) of the previous one
	TPNIKOPOLIS_IMPEXP double logNormalToNormal(
                const double& forward,
                const double& strike,
                const double& timeToExpiry,
                const double& logVolatility);

	TPNIKOPOLIS_IMPEXP double NormDist(const double&);
	TPNIKOPOLIS_IMPEXP double NormDistNCR(const double& x, const double& mu, const double& sigma);
	TPNIKOPOLIS_IMPEXP double NormPdf(const double&);
	TPNIKOPOLIS_IMPEXP double NormPdfNCR(const double& x, const double& mu, const double& sigma);

	TPNIKOPOLIS_IMPEXP void price(const double& forward,
             const double& strike,
             const double& volatility,
             const double& dt,
             const double& multiplier,
             TPOptionType  callput,
             TPModelType   modelType,
             double&       price);

	TPNIKOPOLIS_IMPEXP void priceAndGreks(const double& forward,
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
                     double&       vega);

	TPNIKOPOLIS_IMPEXP void priceAndGreksExt(const double& forward,
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
					 double&	   vanna,
					 double&       volga,
					 double&       speed);

	//////
	//
	//	Q-MODEL. THIS IS BASICALLY A MIXTURE OF A NORMAL AND LOGNORMAL MODEL, CONTROLLED  BY THE
	//  PARAMETER Q.
	//   q = 0 =========> NORMAL MODEL
	//   q = 1 =========> LOGNORMAL MODEL.
	//
	//////////////////////////////////////////////////////////
	TPNIKOPOLIS_IMPEXP void QBlackScholesPrice(const double& forward,
                     const double& strike,
                     const double& volatility,
                     const double& dt,
                     const double& multiplier,
                     const double& q_param, // The q parameter.
                     TPOptionType  callput,
                     double&       price);

	//////
	//
	//	2Q-MODEL. A MORE SOPHISTICATED VERSION OF THE Q-MODEL , WITH TWO Q-PARAMETERS AND 
	//  WHICH COTROL THE SKEW TO THE LEFT (q_L) AND THE SKEW TO THE RIGHT (q_R)
	//////////////////////////////////////////////////////////
	TPNIKOPOLIS_IMPEXP void Two_QBlackScholesPrice(const double& forward,
                     const double& strike,
                     const double& volatility,
                     const double& dt,
                     const double& multiplier,
                     const double& q_L,
					 const double& q_R,
					 const double& A, // The acnhor value..
                     TPOptionType  callput,
                     double&       price);

	//========================================================//
	//
	//  DISTORTED EXPONENTIAL MODEL ..
	// 
	///================================================================
	TPNIKOPOLIS_IMPEXP void DEM_Price(const double& forward,
                     const double& strike,
                     const double& volatility,
                     const double& dt,
                     const double& multiplier,
                     const double& n,  // the atm level
					 const double& s,	// the skew parameter
					 const double& w, // the wing parameter
                     TPOptionType  callput,
                     double&       price);

	// Version to price many options together, required to speed up the replication pricing for the CMS's
	// LogNormal Pricing is assumed here
	TPNIKOPOLIS_IMPEXP void priceOptionsVector(const double& forward,
                     const std::vector<double>& strikes,
                     const std::vector<double>& volatilities,
                     const double& dt,
                     const std::vector<double>& percentages, // Relative Weighting of each option
                     TPOptionType  callput,
                     double&       price,	// We add all the prices together
                     double&       delta,	///.... same for the deltas
                     double&       gamma,	// ... and the gamma's
                     double&       theta,	// ... and the theta's;
                     double&       vega);	/// ... 

	/// 
	/// Power Black Future analytic Formula
	/// We'll solve the following case:	max(F^n - K^2,0) for calls, or max(K^n-F^n, 0) for puts.
	/// F is assumed to be a standard Lognormal or Normal martingale with constant Vol, i.e.
	///	  dF = \sigma F dWt  or
	///	  dF = \sigma dWt
	//  NOTE:: The Formula needs adjustment for cash-options as in the usual Black&Scholes case and will
	///			not work for power Stock optios for example.

	TPNIKOPOLIS_IMPEXP	void  PowerPrice(const double& forward,
											 const double& strike,
											 const double& volatility,
											 const double& dt,
											 const double& forwardpower, // This is the power of the forward
											 TPOptionType  callput,
											 TPModelType   modelType,
											 double&       price,
											 double&       delta,
											 double&       gamma,
											 double&       theta,
											 double&       vega);


	///////////////////////////////////////////////////////////////////////////////////

  TPNIKOPOLIS_IMPEXP void Black(const double& forward,
           const double& strike,
           const double& volatility,
           const double& dt,
           const double& multiplier,
           TPIRBlackAnalytics::TPOptionType  callput,
           double&       price,
           bool          doGreeks = false,
           const double& discount = 1.0, // For the theta
           double*       delta = 0,
           double*       gamma = 0,
           double*       theta = 0,
           double*       vega = 0);


  TPNIKOPOLIS_IMPEXP void BlackExt(const double& forward,
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
		   double*		 vanna=0,
		   double*		 volga=0,
		   double*		 speed=0);

  void White(const double& forward,
           const double& strike,
           const double& volatility,
           const double& dt,
           const double& multiplier,
           TPIRBlackAnalytics::TPOptionType  callput,
           double&       price,
           bool          doGreeks = false,
           const double& discount = 1.0, // For the theta
           double*       delta = 0,
           double*       gamma = 0,
           double*       theta = 0,
           double*       vega = 0);

  TPNIKOPOLIS_IMPEXP void WhiteExt(const double& forward,
           const double& strike,
           const double& volatility,
           const double& dt,
           const double& multiplier,
           TPIRBlackAnalytics::TPOptionType  callput,
           double&       price,
           bool          doGreeks = false,
           const double& discount = 1.0, // For the theta
           double*       delta = 0,
           double*       gamma = 0,
           double*       theta = 0,
           double*       vega = 0,
		   double*       vanna = 0,
		   double*       volga = 0,
		   double*		 speed = 0);



//------------------- AMERICAN BARRIER OPTION -----------------------------------
	TPNIKOPOLIS_IMPEXP void priceBarrier(
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
    double&       price);

	TPNIKOPOLIS_IMPEXP void priceBarrierAndGreks(
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
    double&       vega);

	TPNIKOPOLIS_IMPEXP void priceDoubleBarrier(
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
    const double& error = 0.01);

	TPNIKOPOLIS_IMPEXP void priceDoubleBarrierAndGreks(
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
    double&       vega);


  //! Reflexion Principle, normal case, handles the case where x > y  and y < x
  //! The underlying diffusion is normal with drift:
  //! dXt = mu * dt + sigma * dWt
  //! P(Xt < x, max(Xt) > b) = exp(2 * mu * b / sigma^2) N((x - 2*b - mu*t)/sigma / sqrt(t) )
  double probaFlooredMaxSpot(
    const double& mu,
    const double& sigma,
    const double& x,
    const double& b,
    const double& T);

  //! P(Xt < x, max(Xt) < b) = N( (x - mu * t) / sigma / sqrt(t) ) - probaMaxFlooredSpot 
  double probaCappedMaxSpot(
    const double& mu,
    const double& sigma,
    const double& x,
    const double& b,
    const double& T);

  //! P(Xt < x, min(Xt) > b)  
  double probaFlooredMinSpot(
    const double& mu,
    const double& sigma,
    const double& x,
    const double& b,
    const double& T);

  //! P(Xt < x, min(Xt) < b)  
  double probaCappedMinSpot(
    const double& mu,
    const double& sigma,
    const double& x,
    const double& b,
    const double& T);

  //! P(min(Xt) > b)
  TPNIKOPOLIS_IMPEXP double probaFlooredMinSpot(
    const double& mu,
    const double& sigma,
    const double& b,
    const double& T);

  //! P(max(Xt) < b)  
  TPNIKOPOLIS_IMPEXP double probaCappedMax(
    const double& mu,
    const double& sigma,
    const double& b,
    const double& T);

  /// 
  // Expected hittinh Time calculations of a Brownian motion with drift, i.e.
  // 
  //   Xt = \lambda t + \sigma Wt

   // E[ min(tau,T) ]
	TPNIKOPOLIS_IMPEXP double ExpectOfMintauAndT(
		const double& lamda,	// drift
		const double& sigma,	// volatility
		const double& b,		// barrier
		const double& T);		//expiry

  // E[ tau | tau <T ]
	TPNIKOPOLIS_IMPEXP double ConditionalExpectedHittingTime(
		const double& lamda,	// drift
		const double& sigma,	// volatility
		const double& b,		// barrier
		const double& T);		//expiry

  /// Similarly for the Power Black Stuff.
  void PowerBlack(const double& forward,
           const double& strike,
           const double& volatility,
           const double& dt,
           const double& forwardpower,		// POwer for the Forward rate.
           TPIRBlackAnalytics::TPOptionType  callput,
           double&       price,
           double&       delta ,
           double&       gamma ,
           double&       theta ,
           double&       vega );


  /// 
  // This simply computes the following expectation:
  //  E[ S^m *1_{S^n > K^n) ] or E[ S^m *1_{S^n < K^n) ]  
  //   First Case corresponds to CALL option, SEcond one to PUT option
  // where:
  //	S: is a standard Normal or Lognormal martingale
  //	m,n: integers 
  //	S^m : m-th power of S
  //  Note that m and n can be in general be different. I don't know why someone would want to use a payoff like
  //  this, but the case where m=n+1 appears for the power caplet in arrears case.
  TPNIKOPOLIS_IMPEXP void ExpectationOf_Sm_times_ONE_Sn_Gt_Kn(const double& forward,
															   const double& strike,
															   const double& volatility,
															   const double& dt,
															   const double& m_power,
															   const double& n_power,
															   TPIRBlackAnalytics::TPOptionType  callput,
															   double&       price,
															   const double& discount = 1.0, // For the theta
															   double*       delta = 0,
															   double*       gamma = 0,
															   double*       theta = 0,
															   double*       vega = 0);

  /////////////////////
  //
  //	Two Assets. Perfectlly correlated Brownian martingales, i.e.
  //	d S_1 = \sigma_1 S_1 d W(t)
  //
  //	d S_2 = \sigma_2 S_2 d W(t)
  //
  ///////////////////////////////////////////////////////////////////////

  // First compute E[ S_1(T) * 1_{S_2(T) > K) ]
  TPNIKOPOLIS_IMPEXP void ExpectationOf_S1_times_ONE_S2_Gt_K(const double& S1,
															   const double& S_2,
															   const double& strike,
															   const double& vol1,
															   const double& vol2,
															   const double& dt,
															   double&  p);

  // First compute E[ S_1(T) * S_2(T)*1_{S_2(T) > K) ]
  TPNIKOPOLIS_IMPEXP void ExpectationOf_S1_S2_times_ONE_S2_Gt_K(const double& S1,
															   const double& S_2,
															   const double& strike,
															   const double& vol1,
															   const double& vol2,
															   const double& dt,
															   double&  p);

//------------------- ASIAN OPTION 2/3 moments approximation -----------------------
  // Returns N(Gamma . log(a exp(bx) / ( c + d exp(fx) ) ) ) . N'(x)
  class AsianIntegral : public ObjectiveFunction
  {
  public:
    AsianIntegral () {}
    AsianIntegral (const double& a, const double& b, const double& c, const double& d, const double& f, const double& g) 
      : myA(a), myB(b), myC(c), myD(d), myF(f), myGamma(g) {}

    double operator()(const double& x) const ;

  protected:
    double myA;
    double myB;
    double myC;
    double myD;
    double myF;
	double myGamma;
  };

  inline double AsianIntegral::operator()(const double& x) const 
  {
    double logarg = myA * exp(myB * x) / (myC + myD * exp(myF * x));
    double N = TPIRBlackAnalytics::NormDist(myGamma * log(logarg));
    double Nder = 1.0 / sqrt(2.0 * PI) * exp(-x*x/2.0);
    return N*Nder;
  }
  
  class AsianIntegral2 : public ObjectiveFunction
  {
  public:
    AsianIntegral2 () {}
    AsianIntegral2 (const double& a, const double& b, const double& c) 
      : myEta1(a), myEta2(b), myV1(c) {}

    double operator()(const double& x) const ;

  protected:
    double myEta1;
    double myEta2;
    double myV1;
  };

  inline double AsianIntegral2::operator()(const double& x) const 
  {
    double cnte = 1.0/(myEta1 + myEta2 * exp(myV1 * x) );
    double Nder = 1.0 / sqrt(2.0 * PI) * exp(-x*x/2.0);
    return cnte*Nder;
  }

  class AsianIntegral3 : public ObjectiveFunction
  {
  public:
    AsianIntegral3 () {}
    AsianIntegral3 (const double& eta1, const double& eta2, const double& eta3, const double& v1, 
      const double& v2, const double& xetaprime, const double& xeta, const double& eps) 
      : myEta1(eta1), myEta2(eta2), myEta3(eta3), myV1(v1), myV2(v2), myXetaPrime(xetaprime),
    myXeta(xeta), myEpsilon(eps) {}

    double operator()(const double& x) const ;

  protected:
    double myEta1;
    double myEta2;
    double myEta3;
    double myV1;
    double myV2;
    double myXetaPrime;
    double myXeta;
    double myEpsilon;
  };

  inline double AsianIntegral3::operator()(const double& x) const 
  {
    double average = myEta1 + myEta2 * exp(myV1 * x);
    double phi = myXeta * myV2 * x + log(average) + log(myEta3);
    phi /= (myXetaPrime * myV2);

    double secondguy = TPIRBlackAnalytics::NormDist(myEpsilon * phi);
    secondguy /= average;

    double firstguy = TPIRBlackAnalytics::NormDist(myEpsilon * myV2 * myXetaPrime + myEpsilon * phi);
    firstguy *= myEta3;
    firstguy *= exp(0.5 * myV2 * myV2 * myXetaPrime * myXetaPrime + myV2 * myXeta * x);

    double Nder = 1.0 / sqrt(2.0 * PI) * exp(-x*x/2.0);

    double result = Nder * (firstguy - secondguy);

    return result;
  }

  TPNIKOPOLIS_IMPEXP void priceAsianFixedStrike(
    const std::vector<double>& forwards,
    const std::vector<double>& vols,
    const std::vector<double>& fixingsdates,
    const double& strike,
    const double& dt,
    const double& multiplier,
    TPOptionType  callput,
    TPAsianModelType   modelType,
    double&       price);

  TPNIKOPOLIS_IMPEXP void priceAsianFixedStrikeHarmonic(
    const std::vector<double>& forwards,
    const std::vector<double>& vols,
    const std::vector<double>& fixingsdates,
    const double& strike,
    const double& dt,
    const double& multiplier,
    TPOptionType  callput,
    TPAsianModelType   modelType,
    double&       price,
    TP_IntegrationMethod integ = TP_INTEGRATION_ROMBERG,
    long nbPoints = 100);

  TPNIKOPOLIS_IMPEXP void priceAsianFloatingStrike(
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
    TP_IntegrationMethod integ = TP_INTEGRATION_ROMBERG,
    long nbPoints = 100);

  TPNIKOPOLIS_IMPEXP void priceAsianFloatingStrikeHarmonic(
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
    long nbPoints);



  /////================
  //	Functor for Finding Z_0 in the DEM Functional model
  //==============================================
  class Z0_finder : public ObjectiveFunction
  {
  public:
    Z0_finder () {}
    Z0_finder (const double& F, const double& strike, const double& vol, const double& T, 
		const double& n, const double& s, const double& w) : S_0_(F),K_(strike),vol_(vol), T_(T),n_(n),s_(s),w_(w){}

    double operator()(const double& x) const ;

  protected:
	 double S_0_; // forward
	 double K_;
	 double vol_;// the volatility
	 double T_; // time to expiry.
     double  n_; // nu parameter
     double s_;  // skew parameter
     double w_; //  wing
  };

  inline double Z0_finder::operator () (const double& x) const
	{
	  

	  // constraint the volatility to stay above or equal to zero, 
	  double vol2= vol_*vol_;
	  double vol3 = vol2*vol_;
	  double T3= T_*T_*T_;
	  double S0tilde = S_0_/(1+0.5*s_*vol2*T_);
	  double spy1 = (1+n_*vol_*sqrt(T_)*x + 0.5*s_*vol2*T_*x*x+ 1./6.*w_*vol3*sqrt(T3)*x*x*x);
	  double y = S0tilde*spy1;

		return (y - K_)*10000.;
	}



  /////=============================================================================
  //	Functor for Finding the nu level to hit the atm Black&Scholes price  in the DEM Functional model
  //==============================================
  class Nu_finder : public ObjectiveFunction
  {
  public:
    Nu_finder () {}
    Nu_finder (const double& F, const double& strike, const double& vol, const double& T, 
		  const double& s, const double& w) : S_0_(F), K_(strike),vol_(vol), T_(T),s_(s),w_(w)
	{

		// compute the ATM black &Scholes price.
		TPIRBlackAnalytics::price(S_0_,K_,vol_,T_,1.0,TPIRBlackAnalytics::TPCALL,
			TPIRBlackAnalytics::TPLOGNORMAL, price_);
	}

    double operator()(const double& x) const ;

  protected:
	 double S_0_; // forward
	 double K_;
	 double vol_;// the volatility
	 double T_; // time to expiry.
     double s_;  // skew parameter
     double w_; //  wing
	  double price_;
  };

  inline double Nu_finder::operator () (const double& x) const
	{
	  
	  double y;
	  TPIRBlackAnalytics::DEM_Price(S_0_,K_,vol_,T_,1,x,s_,w_,TPIRBlackAnalytics::TPCALL,y);

	  // 
		return (y - price_)*10000.;
	}

};


//----------------------------------------------------------------------------------
//  IMPLIED VOLATILITY FINDER.. GIVEN A PREMIUM FIND THE VOL
class TPNIKOPOLIS_IMPEXP ImpliedVolFinder : public ObjectiveFunction
{
public:

	ImpliedVolFinder(const double& Underlying,
					   const double& strike,
					   const double& Premium,
					   const double& Expiry,
					   const TPIRBlackAnalytics::TPOptionType&	 mycallput,
					   const TPIRBlackAnalytics::TPModelType&   modeltype,	
					   const TP_GradientMethod& gradient_);

	ImpliedVolFinder(const ImpliedVolFinder& rhs) = default;

	~ImpliedVolFinder(){}

	virtual double operator()(const double& x) const ;

	virtual double derivative(const double& x) const ;

private:
	double				premium_;				// Volatility in %
	double				myDTau;			   //  Expiry in year fraction
	double				myForward;			  //   Forward myPrice
	//int					  myCallPut;		 //    1 = Put, 0 = Call
	double				myStrike;			//	   myStrike

///  Model Type / Option Type...
	TPIRBlackAnalytics::TPModelType		mymodelType_;
	TPIRBlackAnalytics::TPOptionType	myoptionType_;
};

//----------------------------------------------------------------------------------
//  IMPLIED STRIKE FINDER.. GIVEN A DELTA FIND THE STRIKE
class TPNIKOPOLIS_IMPEXP ImpliedStrikeDeltaFinder : public ObjectiveFunction
{
public:

	ImpliedStrikeDeltaFinder(const double& Underlying,
		const double& delta,
		const double& Expiry,
		double vol,
		const TPIRBlackAnalytics::TPOptionType&	 mycallput,
		const TPIRBlackAnalytics::TPModelType&   modeltype,
		const TP_GradientMethod& gradient_);

	ImpliedStrikeDeltaFinder(const ImpliedStrikeDeltaFinder& rhs) {}
	~ImpliedStrikeDeltaFinder() {}

	virtual double operator()(const double& x) const;


private:
	double				fwd_;			  //   Forward myPrice
	double				delta_;				// Delta in %
	double				tau_;			   //  Expiry in year fraction
	double              vol_;
	int					  yCallPut;		 //    1 = Put, 0 = Call

///  Model Type / Option Type...
	TPIRBlackAnalytics::TPModelType		mymodelType_;
	TPIRBlackAnalytics::TPOptionType	myoptionType_;
};

///-------------------------------------
//	 Alpha finder (or initial vol of vol in the SABR model) to hit a given ATM Black Volatility
//
//===================================================
//----------------------------------------------------------------------------------
//  IMPLIED VOLATILITY FINDER.. GIVEN A PREMIUM FIND THE VOL
class TPNIKOPOLIS_IMPEXP ImpliedSABRAlphaFinder : public ObjectiveFunction
{
public:

	ImpliedSABRAlphaFinder(const double& Underlying,
					   const double& BlackVol,
					   const double& Expiry,
					   const double& b,
					   const double& r,
					   const double& vlvl,	
					   const TP_GradientMethod& gradient_= TP_ANALYTIC);

	ImpliedSABRAlphaFinder(const ImpliedSABRAlphaFinder& rhs){}
	~ImpliedSABRAlphaFinder(){}

	virtual double operator()(const double& x) const ; // we pass alpha and compute the ATM Black Vol

	virtual double derivative(const double& x) const ;

private:
	double				forward;			   // Forward
	double				m_vol_;				// ATM B&S Volatility in %
	double				tau;				// Expiry of the optiion
	// SABR parameters..
	double				beta;
	double				rho;
	double				volvol;
};


// IMPLENTATION FOR boost solvers.
class TPNIKOPOLIS_IMPEXP TPIRImpliedVolFinder_BIS
{
public:

    TPIRImpliedVolFinder_BIS(const double& Underlying,
        const double& strike,
        const double& Expiry,
        const TPIRBlackAnalytics::TPOptionType&	 mycallput,
        const TPIRBlackAnalytics::TPModelType&   modeltype);

    //BMIRImpliedVolFinder(const BMIRImpliedVolFinder& rhs) {}
    ~TPIRImpliedVolFinder_BIS() {}

    double operator()(const double& x) const;

    double derivative(const double& x) const;

private:
    double				myDTau;			   //  Expiry in year fraction
    double				myForward;			  //   Forward myPrice
    int					  myCallPut;		 //    1 = Put, 0 = Call
    double				myStrike;			//	   myStrike

                                            ///  Model Type / Option Type...
    TPIRBlackAnalytics::TPModelType	     myModelType;
    TPIRBlackAnalytics::TPOptionType	myOptionType;
};

#endif 
