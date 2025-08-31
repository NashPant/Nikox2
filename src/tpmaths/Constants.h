#ifndef TPMATHS_CONSTANTS_TP_68068068041680168
#define TPMATHS_CONSTANTS_TP_68068068041680168


class Constants
{
private:
	// No constructor as never instantiated
	Constants(void);

	// No destructor as never instantiated
	~Constants(void);

	// No copy constructor as never instantiated
	Constants(const Constants& rhs);

	// No assignment as never instantiated
	Constants& operator=(const Constants& rhs);

protected:
public:

	// ------------------------------ Pi -----------------------------------

	/// Pi
	inline static const double Pi(void)				{return(3.1415926535897932385);}

	/// Pi/2
	inline static const double PiOn2(void)			{return(1.5707963267948966192);}

	/// Pi/3
	inline static const double PiOn3(void)			{return(1.0471975511965977462);}

	/// Pi/4
	inline static const double PiOn4(void)			{return(0.78539816339744830962);}

	/// 2*Pi
	inline static const double TwoPi(void)			{return(6.2831853071795864769);}

	/// (2*Pi)^2
	inline static const double TwoPiSquared(void)	{return(39.478417604357434475);}

	/// (2*Pi)^3
	inline static const double TwoPiCubed(void)		{return(248.05021344239856140);}

	/// Sqrt[Pi]
	inline static const double SqrtPi(void)			{return(1.77245385090551602730);}

	/// Sqrt[2*Pi]
	inline static const double Sqrt2Pi(void)		{return(2.5066282746310005024);}

	/// Sqrt[Pi/2]
	inline static const double SqrtPiOn2(void)		{return(1.2533141373155002512);}

	/// Sqrt[Pi/3]
	inline static const double SqrtPiOn3(void)		{return(1.0233267079464884885);}

	/// 1/(Sqrt[Pi])
	inline static const double OneOnSqrtPi(void)	{return(0.56418958354775628695);}

	/// 1/(2*Sqrt[Pi])
	inline static const double OneOnTwoSqrtPi(void) {return(0.28209479177387814347);}

	/// 1/(Sqrt[2*Pi])
	inline static const double OneOnSqrtTwoPi(void) {return(0.39894228040143267794);}

	/// 1/(Sqrt[8*Pi])
	inline static const double OneOnSqrtEightPi(void) {return(0.19947114020071633897);}

	// ------------------------------ Powers --------------------------------

	/// Sqrt[2]
	inline static const double Sqrt2(void)		{return(1.4142135623730950488);}

	/// Sqrt[3]
	inline static const double Sqrt3(void)		{return(1.7320508075688772935);}

	/// Sqrt[5]
	inline static const double Sqrt5(void)		{return(2.2360679774997896964);}

	/// Sqrt[7]
	inline static const double Sqrt7(void)		{return(2.6457513110645905905);}

	/// Sqrt[10]
	inline static const double Sqrt10(void)		{return(3.1622776601683793320);}

	/// 1/Sqrt[2]
	inline static const double OneOnSqrt2(void)		{return(0.70710678118654752440);}

	/// 1/Sqrt[3]
	inline static const double OneOnSqrt3(void)		{return(0.57735026918962576451);}

	/// 1/Sqrt[5]
	inline static const double OneOnSqrt5(void)		{return(0.44721359549995793928);}

	/// 1/Sqrt[7]
	inline static const double OneOnSqrt7(void)		{return(0.37796447300922722721);}

	/// 1/Sqrt[10]
	inline static const double OneOnSqrt10(void)	{return(0.31622776601683793320);}

	// ------------------------------ e -------------------------------------

	/// e
	inline static const double e(void)			{return(2.7182818284590452354);}

	// ------------------------------ Error bounds ---------------------------

	/// 1.0e-6
	inline static const double epsilon06(void) {return(1.0e-6);}

	/// 1.0e-9
	inline static const double epsilon09(void) {return(1.0e-9);}

	/// 1.0e-12
	inline static const double epsilon12(void) {return(1.0e-12);}

	/// 1.0e-15
	inline static const double epsilon15(void) {return(1.0e-15);}


    // ---------------------------- Cut Off values  -----------------------------
    
    /// 7.8     sort of cut-off after/before which the normal dist. is assumed to be zero
    inline static const double  inverseNormCutOff(void) { return 7.8; }

    
    // ---------------------------- carried over from GFP Maths ----------------
    
    // 365.25   Divisor when computing years to exercise for options.  That is, tau = (exercise - today) / GPD_TAU_BASIS
    inline static const double  TauBasis(void)   {return 365.25; }

    // 100000.0 Definition of +infinity
    inline static const double  Aleph(void) {return 100000.0;   }

};


#endif

