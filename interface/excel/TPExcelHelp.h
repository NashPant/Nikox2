#ifndef TP_EXCEL_HELP_H
#define TP_EXCEL_HELP_H


//#include "TPTools/time/TPDate.h"
//#include "TPTools/TPAny.h"
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//  This class stores help strings for standard variable names used in 
//  the Excel interface.
//
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class TPExcelHelp
{
public:
    // Constructor and destructor.
    ~TPExcelHelp() {}
    TPExcelHelp() {}

    // Dates
    static const char* SpotLag()        { return " A string specifiyng the Spot Lag Period."; }  
    static const char* FixingLag()      { return " A string specifiyng the Fixing Lag Period."; }  
    static const char* PayLag()         { return " A string specifiyng the Payment Lag Period."; }  
    static const char* ResetLag()       { return " A string specifiyng the Reset Lag Period."; }  
    static const char* SettlementLag()  { return " A string specifying the number of (business) days to settlement."; }
    static const char* RollingConv()    { return " A string the rolling convention."; }
    static const char* Calendar()       { return " A string specifying the holiday calendar to be used."; }
    static const char* Basis()            { return " A string specifying the basis convention."; }

    static const char* ResetDate()      { return " The index reset date."; }  
    static const char* StartDate()      { return " The start date."; }  
    static const char* EndDate()        { return " The end date."; }  
    static const char* TenorEndDate()   { return " The end date, as a absolute date or a relative tenor."; }  
    static const char* PayDate()        { return " The payment date."; }  

    static const char* BuildDate()      { return " The build/asOfDate date of a model."; }  
    static const char* AsOfDate()       { return " The build/asOfDate date of a model."; }  
	static const char* AsOfTime()       { return " The build/asOfDate time of a model."; }  
    static const char* ValueDate()        { return " The value date."; }  
    static const char* QuoteDate()        { return " The quoted date of market data."; }  
    static const char* TradeDate()        { return " The trade date."; }  

    static const char* Frequency()      { return " The frequncy as a tenor (1Y, 6M...)"; }

    // Currency
    static const char* Currency()         { return " A string specifying a three letter currency iso code."; }

    // Indices
    static const char* LiborIndexList()    { return " A list of libor indices."; }
    static const char* IndexLabel()        { return " A string to be used as the index tag."; }
    static const char* IndexTag()          { return " A string refering an existing index."; }
	static const char* RFIndexTag()          { return " A string refering an existing  RF index."; }
    static const char* LiborLabel()        { return " A string to be used as the libor index tag."; }
    static const char* LiborTag()          { return " A string refering an existing index."; }
    static const char* FXTag()             { return " A string refering an existing FX index."; }
	static const char* SwapIndexTag()      { return " A string refering an existing Swap index."; }
	static const char* IndexCompoundingType() { return " Type of index compouding (Simple, Averaging or Compoudining for OIS in particular)."; }


    static const char* FwdStartDate()     { return " A string specifying the forward starting date."; }
    static const char* Term()             { return " The accrual term for the index.";}
    static const char* Source()           { return " Source for the index fixings.";}

    // Market Data
    static const char* CurveTag()             { return " A curve tag."; }
    static const char* CurveLabel()           { return " A string to be used as the curve tag."; }
    static const char* MktDataTag()           { return " A market data tag."; }
    static const char* MktDataCollectionTag() { return " A market data collection tag."; }
    static const char* MktDataLabel()         { return " A string to be used as the market data tag."; }

    static const char* SpreadMarketData()   { return " An array of strings specifying spread market data."; }
    static const char* SwapDefaults()       { return " A list of parameters (Frequency, Fwd Rule, Basis, Libor Index, Spot Lag, Calendar)."; }
    static const char* SwapRange()          { return " A range of 2 columns (Swap Tenor, Swap Rate)."; }
    static const char* MoneyMarketDefaults(){ return " A list of parameters (Spot Lag, Basis, Spot, Calendar)."; }
    static const char* MoneyMarketRange()   { return " A range of 2 columns (Swap Tenor, Swap Rate)."; }
    static const char* FutureDefaults()     { return " A list of parameters (Calendar, Tick Size, Tick Value, Spot Lag)."; }
    static const char* FutureRange()        { return " A range of 3 columns (Future date, price, ."; }
    static const char* ZeroCouponRange()    { return " A range of 2 columns (Date, Zero Coupon)."; }

	static const char* AutoAdjustRates()    { return " A bool that says whether we should try to adjust for mkt rates automatically (default is true)"; }

	// Gov Bonds
	static const char* GovBondDefaults(){ return " A list of parameters (Spot Lag, Basis, Spot, Calendar, Frequency)."; }
	static const char* GovBondRange()   { return " A range of columns (Start Date, Maturity Date, coupon, ISIN, etc..)."; }

    // Models
    static const char* ModelTag()        { return " A model tag."; }
    static const char* ModelLabel()      { return " A string to be used as the model tag."; }
    static const char* BuildingDetails() { return " A range of model parameters per index (see reference documents on the model)."; }
    static const char* DiscountModelTag(){ return " A discount model tag."; }
	static const char* BlackModelTag()      { return " A string to be used as the Black model tag."; }

    static const char* Betas()          { return " An array of the betas (model parameter, see reference documents on the model).";}
    static const char* Alphas()         { return " An array of the alphas (model parameter, see reference documents on the model).";}
    static const char* Rhos()           { return " An array of the rhos (model parameter, see reference documents on the model).";}
    static const char* Vovols()         { return " An array of the vovols (model parameter, see reference documents on the model).";}
    static const char* Sigmas()         { return " An array of the sigmas (model parameter, see reference documents on the model).";}
    static const char* Correlations()   { return " An array of the correlations (model parameter, see reference documents on the model).";}
    static const char* Volatilities()   { return " Volatility array/matrix.";}
    static const char* mrv()            { return " An array of dates and Mean reversion values.";}

    static const char* Lambda()                 { return " An array of the lambdas which are adapted to economic calendar and used for computing ATM vol.";}
    static const char* Eweights()               { return " An array of eweights that are assigned for reflecting economic events.";}
    static const char* OptionExpiries_ECal()    { return " (E-Calendar) An array of Option Expiries, as dates or tenor."; }
    static const char* OptionTermDates_ECal()   { return " (E-Calendar) An array of Option Terms, as dates or tenor."; }

    static const char* VolType()        { return " A string indicating the volatility type (normal, lognormal...).";}

    // Products
    static const char* ProductTag()        { return " A product tag."; }
    static const char* ProductLabel()      { return " A string to be used as the product tag."; }

    static const char* OptionExpiries()    { return " An array of Option Expiries, as dates or tenor."; }
    static const char* OptionTermDates()   { return " An array of Option Terms, as dates or tenor."; }
    static const char* Strikes()           { return " An array of Option Strikes."; }
    static const char* StrikeType()        { return " The type of strikes used in the sheet (absolute, relative...)."; }

    static const char* FirstExpiry()       { return " The First Exercise Date."; }
    static const char* LastExpiry()        { return " The Last Exercise Date."; }
    static const char* BermudanType()      { return " The type Bermudan Swaption (fixed end or Fixed tenor)."; }
    
    static const char* UnderlyingTenor()   { return " A Swap Tenor as Underlying to the Option."; }  
    static const char* UnderlyingTenors()  { return " An array of Swap Tenors as Underlying to the option."; }

    static const char* BermSwaptionTag()   { return " A Bermudean Swaption Tag (previously registered)."; }

    static const char* FixedSchedule()     { return " A range of 5 columns (Start Date, End Date, Rate, Notional, Pay Date)."; }
    static const char* FloatingSchedule()  { return " A range of 6 columns (Start Date, End Date, Fixing Date, Spread, Notional, Pay Date)."; }

	static const char* LinearProductTag()  { return " A Tag for a previously registered linear product."; }
	static const char* CmsLinearProductTag()  { return " A Tag for a previously registered Cms linear Floating Leg."; }


    // Trade information
    static const char* Notional()         { return " A notional."; }
    static const char* Notionals()        { return " An array of notionals."; }
    static const char* MarketValue()      { return " Market value for the related trade."; }
    static const char* MarketValues()     { return " An array of market values for the related trades."; }
    static const char* CapOrFloor()       { return " CAP or FLOOR."; }
    static const char* PayOrRec()         { return " PAY or REC."; }

    // Algorithm
    static const char* AlgorithmTag()		{ return " An Algorithm tag."; }
    static const char* AlgorithmLabel()	{ return " A string to be used as the algorithm tag."; }

    // Fixings
    static const char* FixingDetails()	{ return " A list of Fixing details (Fixing page and fixing time), one for each libor index."; }
    static const char* FixingTable()		{ return " A table of fixings per date and per libor index."; }
	static const char* FixingLibor()		{ return " An explicit Libor fixing"; }
	static const char* FixingHit()		{ return " A boolean specifying whether Libor fixing should be hit in curve construction"; }

    // Static functions to return help strings for variable names.
    static const char* Empty()            { return " "; }  

    // Dates and periods.
    static const char* Date()             { return " A Date."; }  
    static const char* Dates()            { return " An array of dates."; }  
    static const char* Factor()           { return " A number used as a factor."; }  

    static const char* ResetDates()       { return " An array of index reset dates."; }  
    static const char* StartDates()       { return " An array of start dates."; }  
    static const char* EndDates()         { return " An array of end dates."; }  
    static const char* PayDates()         { return " An array of payment dates."; }  
    static const char* FixingTiming()     { return " A string specifiyng Fixing Timing (Advance/Arrears)."; }  
    static const char* PayTiming()        { return " A string specifiyng Payment Timing (Advance/Arrears)."; }  

    static const char* NoticeDate()       { return " The notice date of the option."; }  
    static const char* NoticeDates()      { return " An array of the notices dates of the option."; }  
    static const char* OptionExpiryDate() { return " The (adjusted) expiration date of the option."; }  
    static const char* ExerciseStartDate(){ return " The exercise start date of the option."; }  

    static const char* Spot()             { return " The spot value."; }
    static const char* SpotRate()         { return " The spot rate value."; }
    static const char* SpotOrForward()    { return " The spot or forward value of the index."; }
    static const char* ForwardDate()      { return " The forward date."; }
    static const char* ForwardTenors()    { return " A 1D range of forward tenors."; }
    static const char* ForwardPrices()    { return " A 1D range of forward prices."; }
    static const char* ForwardRates()     { return " A 1D range of forward rates."; }
    static const char* Tenor()            { return " A string specifying the tenor."; }
    static const char* Tenors()           { return " An array of tenors."; }
    static const char* FixedTenor()       { return " A string specifying the tenor for a fixed side."; }
    static const char* FloatTenor()       { return " A string specifying the tenor for a floating side."; }

    static const char* Period()           { return " A string specifying the period."; }
    static const char* FixedPeriod()      { return " A string specifying the period for a fixed side."; }
    static const char* FloatPeriod()      { return " A string specifying the period for a floating side."; }

    static const char* TenorOrEndDate()   { return " Either a string specifying the tenor, or an end date."; }

    static const char* Terms()            { return " An array of accrual terms for the index.";}
    static const char* FixedBasis()       { return " A string specifying the basis convention for a fixed side."; }
    static const char* FloatBasis()       { return " A string specifying the basis convention for a floating side."; }

    static const char* BusinessDayAdjustment()  { return " A string specifying the business day adjustment convention ('None', 'ModFol', 'Fol', 'ModPrev', 'Prev')"; }
    static const char* FixedBusinessDayAdjustment()        { return " A string specifying the business day adjustment for a fixed side."; }
    static const char* FloatBusinessDayAdjustment()        { return " A string specifying the business day adjustment for a floating side."; }
    static const char* FixingBusinessDayAdjustment()       { return " A string specifying the business day adjustment for resets if difTPrent to the payment one."; }
    static const char* PayBusinessDayAdjustment()       { return " A string specifying the business day adjustment for payment."; }
    static const char* AccrualBusinessDayAdjustment()      { return " A string specifying the business day adjustment for accrual if difTPrnt to the payment one."; }
    static const char* FixingCities()     { return " A string specifying the holiday centres if difTPrnt to the payment holiday centres."; }
    static const char* RollingDay()          { return " A number (0-31), a date or a string (IMM,...) specifiying the rolling day.";}
	static const char* SpotDays()		{ return " The number of business from today that spot date is."; }

    static const char* Cities()           { return " A string specifying the holiday centres."; }
    static const char* ProtectionPaymentLagOrDate()
                                    { return " Protection Payment Timing either a Lag Period or a specific date.";}
    // Market information.
    static const char* Market()           { return " A market tag."; }
    static const char* Markets()          { return " A array of market tags."; }
    static const char* MarketTag()        { return " (Optional) A string to be used as the market tag."; }
    static const char* DiscountCurrency() { return " A string specifying a (discount) currency code."; }
    static const char* Curve()            { return " A string specifying a curve index."; }
    static const char* DiscountCurve()    { return " A string specifying the discount curve index."; }
    
	static const char* Expiries()          { return " An array of expiries."; }
    static const char* Rates()            { return " An array of rates."; }
    static const char* Prices()           { return " An array of prices."; }
    static const char* Convexities()      { return " An array of convexities."; }
    static const char* BondStartDates()   { return " An array of start dates for bonds."; }
    static const char* BondMaturityDates()  { return " An array of maturity dates for bonds."; }
    static const char* BondPrices()       { return " An array of bond prices."; }
    static const char* ZeroCouponDates()  { return " An array of discount (or zero-coupon) bond maturity dates"; }
    static const char* ZeroCouponPrices() { return " An array of discount (or zero-coupon) bond prices (ex : 0.85)"; }
    static const char* CashTenors()       { return " An array of tenors for cash deposits."; }
    static const char* CashRates()        { return " An array of rates for cash deposits (ex: 0.05 for 5%)."; }
    static const char* CashDefaults()     { return " An array of label-value pairs giving defaults for cash deposits."; }
    static const char* SwapTenors()       { return " An array of tenors for vanilla swaps."; }
    static const char* SwapRates()        { return " An array of rates for vanilla swaps (ex: 0.06 for 6%)."; }
    static const char* BasisSwapDefaults() { return " An array of label-value pairs giving defaults for vanilla basis swaps."; }
    static const char* FutCodes()         { return " An array of futures codes (ex: Mar2002 or H02)."; }
    static const char* FutDates()         { return " An array of futures expiry dates."; }
    static const char* FutPrices()        { return " An array of prices for futures."; }
    static const char* FutCvx()           { return " An array of convexity adjustment for futures."; }
    static const char* FutDefaults()      { return " An array of label-value pairs giving defaults for futures."; }
    static const char* SpreadZeroRates()  { return " An array of strings specifying spread market data."; }
    static const char* FXRates()          { return " An array of fx rates."; }
	static const char* FXFwdPoints()       { return " An array of fx fwd points."; }
    static const char* FraTenors()        { return " An array of FRA tenors."; }
    static const char* FraExpiries()      { return " An array of FRA expiries."; }
	static const char* FraRates()         { return " An array of rates for FRA's (ex: 0.03 for 3%)."; }
    static const char* FraDefaults()      { return " An array of label-value pairs giving defaults for FRA's."; }
	static const char* UseFutures()		{ return " A boolean that specifies whether we should use Futures in bootstrapp"; }
	static const char* PrefFutOver()		{ return " A boolean that specifies whether we should prefer Futures over other instruments in bootstrapp"; }
	static const char* FirstSwap()		{ return " A string that specifies first swap tenor to use in bootstrapp"; }

	static const char* CapDfs()			{ return " A boolean that specifies if Discounts are capped at 1"; }
	static const char* AllowNegRates()	{ return " A string that specifies if we allow for negative forward rates in bootstrapp"; }
    
    static const char* FuturesType()      { return " A string (IMM, AUD, CAD, ...) describing the type of Futures."; }

    static const char* SigmaTensor()      { return " The Tag of a (3D) Tensor that contains the volatilities as %."; }
    static const char* SigmaInputType()   { return " The type of volatility passed in (LogNormal, Normal, CEV, ...)"; }

    static const char* HullWhiteSigmas()  { return " An array of Hull White sigmas as %."; }
    static const char* HullWhiteLambdas() { return " An array of Hull White lambdas."; }
    static const char* Correlation()      { return " A symmetric positive definite matrix of correlations."; }

    static const char* RecoveryMarketData() { return " An array of strings specifying recovery market data."; }

    static const char* DividendCurve()     { return "A 2D array of numbers with 3 columns: ExDivDate, PayDate, Amount";}
    static const char* DividendTaxCredit() { return "A single number the represents the tax credit to be applied to dividends." ;}

    static const char* MarketDataCollectionTag() { return "A string specifying the Tag of the Market Data collection to use."; }
    static const char* MarketDataCurveTags()     { return "An array of strings specifying market data curves."; }

	static const char* CurveTenor()		{ return "An integer representing the underlying Libor Tenor corresponding to the Dual Curve being built"; }
    
    // Trade information.
    static const char* TradeID()          { return " A number specifying the trade's ID."; }
    static const char* LongShort()        { return " A string specifying Long or Short."; }
    static const char* PayRec()           { return " A string specifying Pay or Receive."; }
    static const char* PayerReceiver()    { return " A string specifying a Payer or a Receiver swap (re. to the fixed side)."; }
    static const char* CapFloor()         { return " A string specifying Cap or Floor."; }
    static const char* CallPut()          { return " A string specifiying call or put (C/P)."; }
    static const char* NotionalExchange() { return " A string specifying Yes or No."; }

    static const char* Amounts  ()        { return " An array of fixed, known cashflows."; }
    static const char* ContinuouslyCompoundedInterestRate()             { return " A continuously compounded interest rate, as a decimal."; }
    static const char* ContinuouslyCompoundedYield()                    { return " A continuously compounded yield, as a decimal. This could be a dividend yield or a foreign interest rate."; }
    static const char* Rate()             { return " A fixed rate, as a decimal"; }
    static const char* Coupon()           { return " A fixed coupon, as a decimal"; }
    static const char* Coupons()          { return " An array of fixed coupons, as a decimal"; }
    static const char* Spread()           { return " A floating spread, as a decimal"; }
    static const char* Spreads()          { return " An array of floating spreads, as a decimal"; }
    static const char* CouponsOrSpreads() { return " An array of fixed coupons or floating spreads, as a decimal"; }
	static const char* Strike()           { return " The Strike of the option"; }
    static const char* StrikeRate()       { return " The Strike as a % Rate"; }
    static const char* StrikeRates()      { return " An array of Strikes as % Rates"; }
    static const char* Stub()             { return " A string specifiying the stub type required for irregular Dates(SS,SE,LS,LE)"; }
    static const char* StubLength()       { return " A string specifiying short or long"; }

    static const char* Product()          { return " A product tag."; }
    static const char* Products()         { return " An array of product tags."; }
    static const char* Underlying()       { return " An tag for a product to be used as an underlying."; }
    
    // Indices
    static const char* EquityIndexTag()   { return " A string (=='name') used to reTPr to the Equity stock or index."; }
    static const char* FxIndexTag()       { return " A string (=='name') used to reTPr to the Fx Rate index."; }
    static const char* IrIndexTag()       { return " A string (=='name') used to reTPr to the Interest Rate index."; }
    static const char* LiborIndexTag()    { return " A string (=='name') used to reTPr to the Libor (short term Interest Rate) index."; }
    static const char* Index()            { return " An index tag."; }
    static const char* Indices()          { return " A 1-D array of index tags."; }
    static const char* StockIndices()     { return " A 1-D array of stock index tags."; }
    static const char* Implementation()   { return " An Implementation Method."; }


    static const char* ForwardRate()      { return " A forward (swap,Libor,...) rate, as a %"; }

    // Credit
    static const char* CreditEntity()             { return " A risky counter party.";}
    static const char* CreditAssetType()          { return " A risky asset type.";}
    static const char* DiscountCreditIndex()      { return " A discount credit index.";}
    static const char* CreditCollectionIndex()    { return " A discount credit collection index.";}
    static const char* CreditRecoveryRates()      { return " A list of credit recovery rates.";}
    static const char* CreditDiscountIndices()    { return " An array of credit credit discount indices.";}
    static const char* CouplingCoefficientData()  { return " A tag for coupling coefficient data.";}
    static const char* CouplingCoefficientMatrix(){ return " An array of coupling coefficients.";}
    static const char* PremiumAccruedFlag()       { return " A flag set to TRUE if the accrued part of current premium coupon is paid on default.";}
 


    // Portfolios.    
    static const char* Weights()          { return " An array of weights: positive means 'long', negative means 'short'."; }
	static const char* PortfolioLabel()      { return " A string to be used as the Portfolio/Folder tag."; }

    // Analytics
    static const char* Forward()          { return " The Forward value of the underlying (stock,rate,...)"; }
    static const char* TimeToExpiry()     { return " The Time in Number of Years to the Option Expiry Date"; }
    static const char* BlackVolatility()  { return " A Black volatility number : LogNormal (as %) or Normal ."; }
    static const char* LognormalVolatility()        { return " A Black volatility number (lognormal), as a decimal."; }
    static const char* JumpRate()         { return " A Poisson process intensity. Also known as jump rate and usually denoted by lambda."; }
    static const char* JumpMean()         { return " The expectation of the jump ratio amplitude. Enter 1.0 for centered jumps."; }
    static const char* LognormalJumpVolatility()    { return " A standard deviation number (lognormal) of the jump ratio amplitude , as a decimal."; }
    static const char* DisplacedDiffusionQFactor()  { return " A coefficient specifying the relative displacement. Enter 1.0 for exact lognormality. The Q factor must be greater than 0. In the limit to 0.0, the underling distribution approaches the normal distribution."; }
    static const char* Sigma()            { return " A volatility number (LogNormal, Normal, CEV...) as %."; }
    static const char* Beta()             { return " A CEV coefficient ( 0<=Beta<=1 )."; }
    static const char* Alpha()            { return " A Volatility of Volatility (LogNormal) coefficient as %"; }
    static const char* NormalOrLogNormal(){ return " The type of (Black) volatility: 'N'[ormal] or 'L'[ognormal]"; }
    static const char* Accrual()          { return " The accrual factor"; }
    static const char* PaymentDelay()     { return " The delay between fixing and payment (years)"; }
    static const char* NumberOfYears()    { return " The tenor"; }
    static const char* PeriodFrequency()  { return " The number of periods per year"; }
    static const char* DiscountFactor()   { return " The discount factor to the payment date"; }
    static const char* VarianceGammaTheta()         { return " The theta coefficient of the Variance Gamma process. The skew is dominated by theta."; }
    static const char* VarianceGammaNu()            { return " The nu coefficient of the Variance Gamma process. The kurtosis is dominated by nu."; }
    static const char* GeneralisedBetaTwoA()         { return " The a coefficient of the generalised beta - 2 distribution."; }
    static const char* GeneralisedBetaTwoP()         { return " The p coefficient of the generalised beta - 2 distribution."; }
    static const char* GeneralisedBetaTwoQ()         { return " The q coefficient of the generalised beta - 2 distribution."; }
    static const char* NumberGenerator()            { return " The type of number generator driving the Monte Carlo evaluation."; }

    // Models.
    static const char* SubModelTag()      { return " A sub-model tag."; }
    static const char* DiscountModel()    { return " A discount model tag (a 'yield curve')"; }
    static const char* IrSpreadModel()    { return " An IR spread model tag (a 'yield curve')"; }
    static const char* SpreadIRModel()    { return " A spread IR model tag (a 'yield curve' with spreads)"; }
    static const char* BuildDetails()     { return " An array of label-value pairs specifying the model build method."; }
    static const char* RiskyEntity()      { return " A Risky Counterparty."; }
    static const char* RiskyAssetType()   { return " A Risky Asset type."; }    
    static const char* HullWhiteFactor()  { return " A Hull-White driving factor."; }


    // Templates.
    static const char* Request()          { return " A string specifying the output template."; }
    static const char* InfoRequestList()  { return " A string with ',' delimited info requests (see ListValidInfoRequests())."; }
    static const char* ValueRequestList() { return " A string with ',' delimited value requests (Pv,Delta,...) - defaults to PV."; }
    static const char* ReportTag()        { return " A string to be used as the report tag (to be used for further reTPrences)."; }
    
    // Maths and general objects.
    static const char* ObjectTag()        { return " (Optional) A string to be used as the object tag."; }
    static const char* Data()             { return " An array or matrix of data."; }
    static const char* XValue()           { return " A single x-value."; }
    static const char* XValues()          { return " An array of values to determine the x-axis."; }
    static const char* YValue()           { return " A single y-value."; }
    static const char* YValues()          { return " An array of values to determine the y-axis."; }
    static const char* ZValue()           { return " A single z-value."; }
    static const char* ZValues()          { return " An array of values to determine the z-axis."; }
    static const char* ZSlices()          { return " An array of object tags representing z-slices."; }
    static const char* FValues()          { return " An array or matrix of function values."; }
    static const char* InterpType()       { return " A string specifying the interpolation type."; }
    static const char* ExtrapType()       { return " A string specifying the extrapolation type."; }
    
    // Files and Utilities
    static const char* FilePathAndName()  { return " A string specifying the full path and name of a file."; }

	// A utlity..
	//======
// bariemai na grapso comment.. koita ton kodika gia leptomereies..
//===========================================

//   static double AnyExpiryTodouble(const TPAny& 	expiry_any, const TPDate& asOfDate)
//   {
//	double res;
//    switch(expiry_any.getType())
//	{
//		// If string, then a period is given
//	case TPAny::STRING:
//		{
//			TPPeriod expiry_period(ToString(expiry_any));
//			TPDate Expirydate = asOfDate; Expirydate += expiry_period;
//			Expirydate.adjustBusinessDays(MODIFIEDFOLLOWING,TP_LON);
//			res= (Expirydate.getExcelDate() - asOfDate.getExcelDate())/365.;
//			break;
//		}
//	case TPAny::DOUBLE:
//	case TPAny::LONG:
//		{
//		double tau = expiry_any.toDouble();
//		if (tau > 500.) // in this case the supplied number is a date, have to convert it to year fraction
//		{
//			res= (tau -asOfDate.getExcelDate())/365.;
//		}
//		else
//			res = expiry_any;
//		}
//		break;
//	default:
//		THROW("AnyExpiryTodouble: Invalid Expiry Type ")
//
//
//	}
//
//	return res;
//}


};
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#endif

