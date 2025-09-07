#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>
#include <pybind11/chrono.h>
#include <pybind11/operators.h>
#include <pybind11/pytypes.h>
//#include "pybind11_json.h"

#include <pybind11/cast.h>
//#include "VariantWrapper.h"

//include "VolFunctions.h"
//#include "TPNiCoPolisVersion.h"
//#include "QLTestInterface.h"
//#include "../NikoInter/VolAnalytics.h"
//#include "../NikoInter/Test.h"
//#include "../NikoInter/Utils.h"
//#include "../NikoInter/DateFunctions.h"
//#include "../NikoInter/MathTools.h"
//#include "../NikoInter/MarketDataBuild.h"
//#include "../NikoInter/qlVolAnalytics.h"
//#include "../NikoInter/Api/ApiConfigLoad.h"
//#include "../NikoInter/Api/OldApiLoad.h"
//#include "../NikoInter/Products/QuotedProductsCreate.h"
//#include "../NikoInter/Products/ProductCreate.h"
//#include "../NikoInter/Models/CreateLiborDiscountOld.h"
//#include "../NikoInter/Models/CreateGovBondDiscountModel.h"
//#include "../NikoInter/Pricing/PriceAndHedge.h"
//#include "../NikoInter/Pricing/PriceMidCurve.h"
//#include "../NikoInter/MarketCurves/MarketCurvesCreate.h"
//
//
////#include "../NikoInter/BondAnalytics.h"
////#include "../NikoInter/Interfaceutils.h"
//#include <TPTools/Time/TPDate.h>
//#include <TPDateTime/Cut.h>
//#include <TPTools/Time/TPTenor.h>
//#include <chrono>
//
//#include "TPDateTime/Time.h"
//#include "TPMarketData/Quoted/TPQuotedGovBond.h"
//#include "TPMaths/TPMatrix.h"
//#include "TPPricing/pricingold/TPPricingEnums.h"
//#include "TPProducts/Instruments/Bond/Bond.h"
//
//#include "TPTools/Time/TPPeriodOrDate.h"
//#include "TPFormulae/Volatility/SABR/SabrUtils.h"
//#include "nlohmann/json.hpp"

//
// linking options
// python37.lib;kernel32.lib;user32.lib;gdi32.lib;winspool.lib;comdlg32.lib;advapi32.lib;shell32.lib;ole32.lib;oleaut32.lib;uuid.lib;odbc32.lib;odbccp32.lib;%(AdditionalDependencies)
//using namespace TP::interface;

///
// More examples for pybind11 see
// https://github.com/tdegeus/pybind11_examples
///

//using namespace TP::niko_interface;
//using namespace TP::date_time;
//using namespace TP::Core;
using namespace pybind11::literals;


#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

double square(double x) {
	return x * x;
}

//void take_json(const nlohmann::json& json) {
//	std::cout << "This function took an nlohmann::json instance as argument: " << json << std::endl;
//}
//
//nlohmann::json return_json() {
//	nlohmann::json j = { {"value", 1} };
//
//	std::cout << "This function returns an nlohmann::json instance: " << j << std::endl;
//
//	return j;
//}

//namespace TP::niko_interface::time_series_tools
//{
//	void init(py::module_ &);
//}
//
//
//void init_df(py::module_ &);
//void init_dts_cals(py::module_ &);
//
//namespace TP::niko_interface::curve_tools
//{
//	void init(py::module_ &);
//}
//
////namespace TP::niko_interface::date_tools
////{
////	void init(py::module_ &);
////}
//
//
//namespace TP::niko_interface::product_tools
//{
//	void init(py::module_ &);
//}
//
//namespace TP::niko_interface::index_tools
//{
//	void init(py::module_ &);
//}

using system_time = std::chrono::system_clock::time_point;
namespace py = pybind11;
PYBIND11_DECLARE_HOLDER_TYPE(T, std::shared_ptr<T>);


PYBIND11_MODULE(pyNiko, m) {
	// optional module docstring
	m.doc() = "py-Niko. Nikopolis export for python";
	/*m.def("normalql", &QuantLib::python::normalql, "Test ql",
		py::arg("x")
		);*/
		/*
		 * @brief. TESTING SECTION
		 */
	m.def("square", &square, "A function that squares its input", py::arg("x"));
	//m.def("take_json", &take_json, "pass py::object to a C++ function that takes an nlohmann::json");
	//m.def("return_json", &return_json, "return py::object from a C++ function that returns an nlohmann::json");
	/*
	 * @brief. Niko Version
	 */
	//m.def("niko_version", &TP::api::nikoVersion);
	//m.attr("__version__") = TP_NiCoPolis_VERSION;
	/*
	*  Get all countries
	*/
	//m.def("all_countries", &TP::niko_interface::allCountries);
	//m.def("country_info", &TP::niko_interface::countryInfo, "code_or_name"_a);
	//
	///*
	// * @brief. Safety check for niko holidays
	// */
	//m.def("set_holidays", &TP::api::setNikoHolidays);
	///*
	//	@brief. Niko API config loaf
	//*/
	//m.def("loadOldIndices", &TP::api::loadOldIndices, "path"_a);
	//m.def("loadOldBondIndices", &TP::api::loadOldBondIndices, "path"_a);
	//m.def("loadOldSwapIndexTypes", &TP::api::loadOldSwapIndexTypes, "path"_a);
	//m.def("get_all_swap_index_types", &getAllSwapIndexTypes, py::arg("ccy") = std::nullopt);
	//m.def("loadCalendars", &TP::api::loadAllCalendars, "path"_a);
	//m.def("to_iso_code", &TP::api::toIsoCode, "niko_bond_code"_a);
	///*
	//* 
	//*/
	//py::class_<TP::date_time::Calendar, std::shared_ptr<TP::date_time::Calendar>>(m, "Calendar")
	//	//.def(py::init())  // Constructor
	//	;
	//m.def("all_calendars", &TP::api::loadedCalendars);
	///*
	//	@brief. Load Templates in Memory
	//*/
	//py::class_<TP::Market::TemplateStore, std::shared_ptr<TP::Market::TemplateStore>>(m, "TemplateStore")
	//	.def(py::init<>())
	//	.def("size", &TP::Market::TemplateStore::total_size)
	//	.def("ids_for_type", &TP::Market::TemplateStore::getTemplateIdsForType);

	//m.def("build_template_store", &TP::api::buildTemplateStore, "path"_a);
	//m.def("all_bond_templates", &TP::api::storedBondTemplates);
	//m.def("nominal_gov_curves", &TP::api::allNominalGovBondCurves);
	//m.def("inflation_gov_curves", &TP::api::allInflationGovBondCurves);
	///*
	// * @brief  Build All the configs collection and store it in Cache Memory
	// *		   To retrieve always use "ConfigCollection_Store" as name
	// */
	//m.def("build_market_config_collection", &TP::api::buildMaketConfigCollection, R"(Builds all the configs, Templates and Calendars.
 //      Args:
	//	- base Path
	// Returns:
	//	A string *ConfigCollection_Store* for the name of container in cache memory. Note that this is not the pointer it's a just the 
	//   name from which we can restrieve the container.
 //     )", "path"_a);
	//m.def("index_store", &TP::api::getIndexStore, R"(Returns index store from Collection.)");

	//m.def("tpnormalpdf", &normpdf, "Normal Pdf",
	//	py::arg("x"),
	//	py::arg("sigma"),
	//	py::arg("mean")
	//);
	///*======================================================================================*/
	///*
	// * @brief. Quoted Product Functions
	// */
	//m.def("create_quoted_bond", &TP::api::createQuotedBond, "quote_date"_a,
	//	"quote"_a, "quote_type"_a, "issue_date"_a, "maturity"_a, "coupon"_a, "notional"_a,
	//	"is_inflation"_a, "template_id"_a, "Tag"_a, py::arg("OptFirstCouponDate") = std::nullopt);
	//m.def("build_quoted_bond_curve", (std::string(*)(const TPDate&, const std::string&, const std::vector<std::string>&, const std::string&, const std::optional < std::string>&))
	//	&TP::niko_interface::createQuotedBondCurve, "asof"_a, "gov_index"_a, "list of quotes"_a, "template_id"_a, "optTag"_a);

	///*py::class_<TP::bond::Bond, std::shared_ptr<TP::bond::Bond>>(m, "Bond")
	//	.def(py::init<>())
	//	.def("is_zero", &TP::bond::Bond::isZeroCouponBond);*/
	//m.def("bond_coupon_dates", &TP::niko_interface::bondCouponDates, "settlement"_a, "maturity"_a, "frequency"_a, "city"_a);
	//m.def("clean_price_to_yield", &TP::niko_interface::cleanPriceToYield, "settlement"_a, "maturity"_a, "coupon"_a, "price"_a, "frequency"_a, "basis"_a, "city"_a);

	//m.def("create_fixed_coupon_bond", &createFixedCouponBond, R"(Creates a fixed coupon bond from a bond
	//Quote, which together with the template parameters are enough to create a Bond Product.)",
	//	"asof"_a, "Quoted_bond"_a, "Tag"_a);
	///*
	// * @brief. Fast bond schedule generation.
	// */
	//m.def("create_bond", &TP::niko_interface::createBond, "bond_quote"_a);
	///*
	// * @brief Create Market Data Collection
	// */
	//m.def("create_market_data_collection", &createMarketDataCollection, R"(Creates a Market Data Collection from Curve Tags or
	//other MarketData Collection into 1 container (which is just a static map of different types.)",
	//	"Tag"_a, "asof"_a, "curve_tags"_a, "opt_curve_tags_2"_a, "opt_curve_tags_3"_a, "opt_curve_tags_4"_a, "opt_curve_tags_5"_a);
	///*======================================================================================*/
	//// normal sabr function
	//m.def("normalsabrvol", &normalsabrvol, "Normal Sabr Vol approximation",
	//	py::arg("forward"),
	//	py::arg("strike"),
	//	py::arg("atmvol"),
	//	py::arg("tau"),
	//	py::arg("beta"),
	//	py::arg("rho"),
	//	py::arg("vvol")
	//);

	//// Normal sabr fit
	//m.def("fit_normal_sabr", &fitnormalsabr, R"(Normal Sabr Fit for rates.
 //      Args:
	//	- fwd Rate
	//    = atmbpvol
	//	- Time to expiry
	//	- Array of strikes
	//	- Array of bpvols
	//	- Optional beta. If not given a default of 0.5 will be used.
	// Returns:
	//	Vector of fitted sabr parameters (beta, rho, volofvol)
 //     )",
	//	py::arg("forward"),
	//	py::arg("atmvol"),
	//	py::arg("tau"),
	//	py::arg("strikes"),
	//	py::arg("vols"),
	//	py::arg_v("(Beta)", std::nullopt, "None")
	//);
	//// Robust Normal sabr fit
	//m.def("fit_robust_normal_sabr", &fitrobustnormalsabr, R"(Normal Sabr Fit for rates with beta search.
	//   Args:
	//	- fwd Rate
	//	= atmbpvol
	//	- Time to expiry
	//	- Array of strikes
	//	- Array of bpvols
	// Returns:
	//	Vector of fitted sabr parameters (beta, rho, volofvol)
	//  )",
	//	py::arg("forward"),
	//	py::arg("atmvol"),
	//	py::arg("tau"),
	//	py::arg("strikes"),
	//	py::arg("vols"),
	//	py::arg("objective_function_scaling") = 1.
	//);
	///*======================================================================================*/
	//// Create Old Style SABR grid where interpolaton is done on the sabr parameters rather than the vols
	///*======================================================================================*/
	//m.def("create_sabr_grid", &createIRSABRGrid, R"(Creates a SABR grid and stores it in memory.)",
	//	"asof"_a, "ccy"_a, "expiries"_a, "tenors"_a, "betas"_a, "rhos"_a, "vvols"_a, "shifts"_a, "model"_a, "Tag"_a);
	//m.def("get_sabr_smile", &getSABRSmile, R"(Gets sabr smile from define sabr grid parameters)",
	//	"asof"_a, "ccy"_a, "expiry"_a, "tenor"_a, "fwd"_a, "strike"_a, "atmvol"_a, "Tag"_a);
	//// add vectorised numpy functio
	////m.def("square", py::vectorize(&square));

	//// Classic B&S
	// /*
	// * optBlackPrice
	// */


	//m.def("blackscholes", &blackscholes, R"(Prices an option using black model.

	//	The models supported are:
	//	- Black normal     dF = vol*dw
	//	- Black lognormal  dF/F = vol*dwThe option pricing result metrics supported are:
	//	- Price
	//	- Delta (derivative of price to underlying forward)
	//	- Gamma (derivative of delta to underlying forward)
	//	- Vega (derivative of price to volatility)
	//	- Theta (derivative of price to time to maturity)All the metrics are unscaled. For example, to get the vega for 1% change of volatility, you have to
	//	multiply Vega by 0.01. To get 1-day theta, you have to devide Theta by 365.0

	//	Args:
	//		model: is the model to use: N (Normal),L (Lognormal).
	//		forward: is the underlying forward price.
	//		strike: is the strike of the option.
	//		tExp: is the Time to Maturity in Years. less or equal to zero means the option expired.
	//		optionType: is the type of option: CALL,PUT,STRADDLE.
	//
	//	Returns:
	//		the option pricing result depending on the outputType.
	//	)"
	//	, py::arg("forward"), py::arg("strike"), py::arg("vol"), py::arg("tau"),
	//	py::arg("optiontype"), py::arg("model"), py::arg("discount") = 1.);
	//// Implied Vol Function
	//m.def("impliedVol", &TP::niko_interface::impliedVol, R"(Calculated implied Volatility given input parameters)",
	//	py::arg("optiontype"), py::arg("forward"), py::arg("strike"), py::arg("premmium"), py::arg("[guess]"),
	//	py::arg("tau"), py::arg("[model type] (Log as Default)"), py::arg("[Root method]"));
	///*
	// * Implied strike from delta
	// */
	//m.def("implied_strike_from_delta", &TP::niko_interface::impliedStrikeFromDelta, R"(Calculated implied Strike from given delta)",
	//	py::arg("optiontype"), py::arg("forward"), py::arg("volatility"), py::arg("delta"), py::arg("[guess]"),
	//	py::arg("tau"), py::arg("[model type] (Log as Default)"), py::arg("[Root method]"));
	///*
	// *  normal to lognormal functions
	// */
	//m.def("normal_to_lognormal", &TP::niko_interface::normalToLognormal, R"(Calculated Lognormal vol from normal)",
	//	py::arg("forward"), py::arg("strike"), py::arg("normal_volatility"), py::arg("tau"),
	//	py::arg("use_exact") = true);
	//// A few Date time functions

	///*
	// * @brief. NExt IMM Date given asof.
	// */
	//m.def("nextIMMDate", &nextIMMDate, R"(Gets next IMM Date given the asofdate.
	//- i.e (TPDate, 3M(for quarterly), "Friday", 2)",
	//	py::arg("asofdate"), py::arg("MonthlyTenor"), py::arg("Dayoftheweek"), py::arg("week"));
	///*
	// * @brief. Next and previous weekdays
	//*/
	//m.def("next_weekday", &nextWeekday, R"(Gets the next weekday from given date. If same adds 1 week)",
	//	"asof"_a, "which_day"_a);
	//m.def("previous_weekday", &previousWeekday, R"(Gets the previous weekday from given date. If same subtracts 1 week)",
	//	"asof"_a, "which_day"_a);
	//m.def("which_week", &whichWeek, R"(Return the week of the month of the given date)", "asof"_a);
	///*
	// * @brief. Add a Period (positive or negative) to the given date
	// */
	//m.def("addPeriod", &addPeriod, R"(Adds a period to the given Date, Period as string, i.e. 1M, 2Y, 3W, etc.)",
	//	py::arg("startdate"), py::arg("Period str"), py::arg("roll (str)"), py::arg("city"));
	//m.def("add_periods", &addPeriods, R"(Adds 2 periods. Assumes periods are of teh same type, i.e. Months, years, etc)",
	//	"period1"_a, "period2"_a, py::arg("check_types") = true);
	//m.def("shiftDate", &shiftDate, R"(Adds a period to the given Date using new calendar interface, Period as string, i.e. 1M, 2Y, 3W, etc.)",
	//	py::arg("startdate"), py::arg("Period str"), py::arg("roll (str)"), py::arg("city"));

	///*
	// * @brief. Shift business days according to calendar. Can be postiive or negative.
	// */
	//m.def("shift_bus_days", &addBusDays, R"(Shifts current date to the number of business days(pos or negetive))",
	//	py::arg("asof"), py::arg("bus_days"), py::arg("calendar"));
	///*
	// * @brief. dayCount Fraction (basic one wihtout calendars)
	//*/
	//m.def("dcf", &dcf, R"(Computes daycount fraction between 2 dates, given a daycount str, like ACT/ACT, ACT/360, etc)",
	//	py::arg("startdate"), py::arg("enddate"), py::arg("daycount"));
	//m.def("time_dcf", &timeDcf, R"(Computes daycount fraction between 2 dates and a cut, given a daycount str, like ACT/ACT, ACT/360, etc)",
	//	py::arg("expiry"), py::arg("daycount"), py::arg("Cut"), py::arg("opt_asof") = std::nullopt);
	///*
	// * @brief. Order String periods according to their length in ascending order
	//*/
	//m.def("orderPeriods", &orderPeriods, R"(Orders a vector of strings which represent period)",
	//	py::arg("input string list of periods"));
	///*
	// * @brief. Year fraction between 2 tenors
	//*/
	//m.def("tenorDcf", &tenorDcf, R"(Returns year fraction between 2 Tenors (negative taus will be allowed if first tenor is less than second)",
	//	py::arg("Tenor 1(str)"), py::arg("Tenor 2(str)"));
	///*
	// * @brief. year month code
	// */
	//m.def("month_year_code", &monthYearCode, R"(Return the month year code of the given date)",
	//	"asof"_a, "year_digits"_a);
	///*
	// * @brief. Check if a date is holiday for the calendars in the array. Assumes that calendars have been loaded in cache memory first
	// */
	//m.def("is_holiday", &isHoliday, R"(Checks if a date is holiday for the given calendars)",
	//	"asof"_a, "calendars"_a);
	///*
	// * @brief. Dates Generator main function
	// */
	//m.def("generate_dates", &datesGenerator, R"(Generate Expiry/or any other  dates from 2 date generator templates)",
	//	"asof"_a, "imm_template"_a, "serial_template"_a, "Ntotal_dates"_a, "Nserial_dates"_a, py::arg("is_fi") = false,
	//	py::arg("is_midcurve") = false, py::arg("early_roll") = 0);
	///*
	// * @brief. Weekly Dates generator
	// */
	//m.def("generate_weekly_expiry_dates", &weeklyDatesGenerator, R"(Generate 4 weekly expiry dates)",
	//	"asof"_a, "imm_template"_a, "first_expiry"_a, "second_expiry"_a, py::arg("which_day") = "Fri");


	//// Chrono and Time Conversions

	//// Return the current time off the wall clock
	//m.def("test_chrono1", []() { return std::chrono::system_clock::now(); });
	//m.def("test_chrono2", [](system_time t) { return t; });
	//m.def("test_chrono4", [](system_time a, system_time b) { return a - b; });
	///*
	// * @brief. Linus Spline Interpolation
	//*/
	//m.def("linusSplineInterpolation", &linusSplineInterpolation, R"(3 Point Spline Interpolation for more robustness)",
	//	py::arg("vector of x-input"), py::arg("vector of y-input"), py::arg("vector of x- to compute"), py::arg("lambda"));
	///*
	// * @brief. Spline Interpolation with default gradient
	//*/
	//m.def("SplineInterpolation", &SplineInterpolation, R"(3 Point Spline Interpolation for more robustness)",
	//	py::arg("vector of x-input"), py::arg("vector of y-input"), py::arg("vector of x- to compute"), py::arg("Use default gradient"));
	///*
	// * @brief. Fitting NelsonSiegelSvenson to a bunch of yields and time to maturities
	//*/
	//m.def("fitNSSModel", &fitNSSModel, R"(Fit Nelson Siegel Svenson Model for Yields)",
	//	py::arg("vector of Time to Maturities"), py::arg("vector of Yield to Maturities"));
	///*
	// * @brief. Create a Libor Index
	//*/
	//m.def("createLiborIndex", &createLiborIndex, R"(Creates a Libor Index and saves it in Cache)",
	//	py::arg("Currency"), py::arg("SpotDays"), py::arg("Term"), py::arg("Basis"), py::arg("FwdRule"),
	//	py::arg("Calendar"), py::arg("Source"), py::arg("Reset Gap"), py::arg("Label"), py::arg("Payment Gap"));
	///*
	// * @brief. Displays Libor Index Cache
	//*/
	//m.def("displayIndexCache", &displayIndexCache, R"(Displays the Index Cache)");
	///*
	// @brief. Output enumeration for basis
	//*/
	//py::enum_<TPBasis>(m, "TPBasis")
	//	.value("TP30360", TPBasis::TP_30_360)
	//	.value("TP30360E", TPBasis::TP_30E_360)
	//	.value("TPACT360", TPBasis::TP_ACT_360)
	//	.value("TPACT365", TPBasis::TP_ACT_365)
	//	.value("TPACTACT", TPBasis::TP_ACT_ACT)
	//	.value("TPCONT", TPBasis::TP_CONT)
	//	.export_values();
	//m.def("toBasis", &toBasis, R"(Creates a Basis enumeration from string)", py::arg("string"));
	//m.def("basisToString", &basisToString, R"(TPBasis to String)", py::arg("TPBasis"));
	///*
	//  @brief Enumeration for Roll
	//*/
	//py::enum_<TPRolling>(m, "TPRolling")
	//	.value("FOLLOWING", TPRolling::FOLLOWING)
	//	.value("MODIFIEDFOLLOWING", TPRolling::MODIFIEDFOLLOWING)
	//	.value("PREVIOUS", TPRolling::PREVIOUS)
	//	.value("MODIFIEDPREVIOUS", TPRolling::MODIFIEDPREVIOUS)
	//	.value("ENDOFMONTH", TPRolling::ENDOFMONTH)
	//	.value("FRN", TPRolling::FRN)
	//	.value("NOROLLING", TPRolling::NOROLLING)
	//	.value("INVALIDROLLING", TPRolling::INVALIDROLLING)
	//	.export_values();
	//m.def("toRolling", &toRolling, R"(Creates a Roll Convention from string)", py::arg("string"));
	//m.def("rollToString", &rollToString, R"(TPRolling to String)", py::arg("TPRolling"));
	///*
	//  @brief Output enumeration for Norm (required for sabr calculations)
	//*/
	///*py::enum_<TP::math::NormType>(m, "NormType")
	//	.value("H2", TP::math::NormType::H2)
	//	.value("SQUARES", TP::math::NormType::H2)
	//	.value("HINF", TP::math::NormType::Hinfty)
	//	.value("INF", TP::math::NormType::Hinfty)
	//	.value("MAX", TP::math::NormType::Hinfty)
	//	.value("L1", TP::math::NormType::L1)
	//	.value("ABS", TP::math::NormType::L1)
	//	.export_values();
	//m.def("toNormType", &toNormType, R"(Creates a NormType Enum from string)", py::arg("string"));
	//m.def("NormTypeToString", &NormTypeToString, R"(NormType enum to String)", py::arg("NormType"));*/
	///*
	// @brief. Frequencyy Enumeration Class
	//*/
	//py::enum_<FREQUENCY>(m, "FREQUENCY")
	//	.value("ONCE", FREQUENCY::ONCE)
	//	.value("ANNUAL", FREQUENCY::ANNUAL)
	//	.value("SEMI_ANNUAL", FREQUENCY::SEMI_ANNUAL)
	//	.value("EVERY_FOURTH_MONTH", FREQUENCY::EVERY_FOURTH_MONTH)
	//	.value("QUARTERLY", FREQUENCY::QUARTERLY)
	//	.value("BIMONTHLY", FREQUENCY::BIMONTHLY)
	//	.value("MONTHLY", FREQUENCY::MONTHLY)
	//	.value("EVERY_FOURTH_WEEK", FREQUENCY::EVERY_FOURTH_WEEK)
	//	.value("BIWEEKLY", FREQUENCY::BIWEEKLY)
	//	.value("WEEKLY", FREQUENCY::WEEKLY)
	//	.value("DAILY", FREQUENCY::DAILY)
	//	.export_values();
	//m.def("toFrequency", &toFrequency, R"(Creates a frequency Enum from string)", py::arg("string"));
	//m.def("freqToString", &freqToString, R"(FERQUENCY enum to String)", py::arg("FREQUENCY"));

	///*
	// @brief. Bond Measures
	//*/
	//py::enum_<TP::pricing::MeasureType>(m, "MeasureType")
	//	.value("CLEAN_PRICE", TP::pricing::MeasureType::CLEAN_PRICE)
	//	.value("CLEAN", TP::pricing::MeasureType::CLEAN_PRICE)
	//	.value("DIRTY_PRICE", TP::pricing::MeasureType::DIRTY_PRICE)
	//	.value("DIRTY", TP::pricing::MeasureType::DIRTY_PRICE)
	//	.value("YTM", TP::pricing::MeasureType::YTM)
	//	.value("Yield", TP::pricing::MeasureType::YTM)
	//	.value("MODIFIED_DURATION", TP::pricing::MeasureType::MODIFIED_DURATION)
	//	.value("MD", TP::pricing::MeasureType::MODIFIED_DURATION)
	//	.value("DV01", TP::pricing::MeasureType::DV01)
	//	.value("CONVEXITY", TP::pricing::MeasureType::CONVEXITY)
	//	.value("GAMMA", TP::pricing::MeasureType::CONVEXITY)
	//	.value("ACCRUED_INTEREST", TP::pricing::MeasureType::ACCRUED_INTEREST)
	//	.value("ACCRUED", TP::pricing::MeasureType::ACCRUED_INTEREST)
	//	.export_values();

	///*
	//  @brief. QuantLib functions
	//*/

	//py::enum_<QuantLib::CubicInterpolation::DerivativeApprox>(m, "SPLINETYPE")
	//	.value("SPLINE", QuantLib::CubicInterpolation::DerivativeApprox::Spline)
	//	.value("SPLINEOM1", QuantLib::CubicInterpolation::DerivativeApprox::SplineOM1)
	//	.value("SPLINEOM2", QuantLib::CubicInterpolation::DerivativeApprox::SplineOM2)
	//	.value("FOURTHORDER", QuantLib::CubicInterpolation::DerivativeApprox::FourthOrder)
	//	.value("PARABOLIC", QuantLib::CubicInterpolation::DerivativeApprox::Parabolic)
	//	.value("FRTIS", QuantLib::CubicInterpolation::DerivativeApprox::FritschButland)
	//	.value("AKIMA", QuantLib::CubicInterpolation::DerivativeApprox::Akima)
	//	.value("KRUGER", QuantLib::CubicInterpolation::DerivativeApprox::Kruger)
	//	.value("HARMONIC", QuantLib::CubicInterpolation::DerivativeApprox::Harmonic)
	//	.export_values();

	//py::enum_<QuantLib::CubicInterpolation::BoundaryCondition>(m, "SPLINEBOUNDARY")
	//	.value("NOTATKNOT", QuantLib::CubicInterpolation::BoundaryCondition::NotAKnot)
	//	.value("FIRSTDERIVATIVE", QuantLib::CubicInterpolation::BoundaryCondition::FirstDerivative)
	//	.value("SECONDDERIVATIVE", QuantLib::CubicInterpolation::BoundaryCondition::SecondDerivative)
	//	.value("PERIODIC", QuantLib::CubicInterpolation::BoundaryCondition::Periodic)
	//	.value("LAGRAGNGE", QuantLib::CubicInterpolation::BoundaryCondition::Lagrange)
	//	.export_values();

	///*
	// *  PRODUCT TYPE
	// */
	//py::enum_<TP::ProductsOld::TPProductType>(m, "ProductType")
	//	.value("INVALID", TP::ProductsOld::TPProductType::INVALID_PRODUCT_TYPE)
	//	.value("FIXED_LEG", TP::ProductsOld::TPProductType::FIXED_LEG)
	//	.value("FLOATING_LEG", TP::ProductsOld::TPProductType::FLOATING_LEG)
	//	.value("SWAP", TP::ProductsOld::TPProductType::SWAP)
	//	.value("IRSWAPTION", TP::ProductsOld::TPProductType::IRSWAPTION)
	//	.value("MIDURVE", TP::ProductsOld::TPProductType::IRMIDCURVE_SWAPTION)
	//	.export_values();
	//m.def("to_product_type", &toProductType, R"(Creates a ProductType Enum from string)", py::arg("string"));
	//m.def("product_type_to_string", &productTypeToString, R"(ProductType enum to String)", py::arg("TPProductType"));

	//m.def("ql_cubic_spline", &qlCubicSpline, R"(QuantLib cubic spline)", "xs"_a, "ys"_a, "spline_type"_a, "BoundaryCondition"_a, "x_to_interp"_a, "allow_extrpolation"_a);

	//// Tenor class exposed
	//py::class_<TPTenor>(m, "TPTenor")
	//	.def(py::init<const std::string &>())
	//	.def("__str__", [](const TPTenor& d) { return d.toString(); })
	//	.def(py::self == py::self)
	//	.def(py::self != py::self)
	//	.def("year_count", &TPTenor::yearCount)
	//	.def("month_count", &TPTenor::monthCount)
	//	.def("day_count", &TPTenor::dayCount)
	//	.def("week_count", &TPTenor::weekCount)
	//	.def("toString",
	//		[](const TPTenor &d) {
	//	return d.toString();
	//});

	//py::implicitly_convertible<std::string, TPTenor>();

	//// TPPeriodorDate class exposed
	//py::class_<TPPeriodOrDate>(m, "TPPeriodOrDate")
	//	.def(py::init<const TPPeriod &>())
	//	.def(py::init<const TPDate &>())
	//	.def(py::init<const std::string &>())
	//	.def("period", &TPPeriodOrDate::period)
	//	.def("date", &TPPeriodOrDate::date)
	//	.def("isPeriod", &TPPeriodOrDate::isPeriod);

	//py::implicitly_convertible<std::string, TPPeriodOrDate>();

	///*
	// * class QuotedBondData
	// */
	//py::class_<TP::bond::BondRawQuote>(m, "BondRawQuote")
	//	.def(py::init<const double&, const std::string&>());

	//py::class_<TP::marketdata::TPQuotedGovBondData>(m, "TPQuotedGovBondData")
	//	.def(py::init<const TPPeriodOrDate, const TP::bond::BondRawQuote&, const  TPDate&, const TPPeriodOrDate&,
	//		const double&, const double&, bool, const std::string&>());
	///*
	// @brief. Shifted SABR Class. We have to expose this in order to either use it directly in the vol building process or pass around
	//		 sabr slices for each combination of expiry and Tenor.
	//*/
	//py::class_<TP::TPVolModels::ShiftedSabr>(m, "ShiftedSabr") //: class initialisation with : double atm_vol, double beta, double rho, double vov, double tau, double shift
	//	.def(py::init<double, double, double, double, double, double >())
	//	.def("getVol", &TP::TPVolModels::ShiftedSabr::getVol)
	//	.def("fitSmile", &TP::TPVolModels::ShiftedSabr::fitSmile)
	//	.def("to_dict", [](const TP::TPVolModels::ShiftedSabr& d) { return TP::TPVolModels::ShiftedSabrToJson(d); })
	//	.def("__str__", [](const TP::TPVolModels::ShiftedSabr& d) { return TP::TPVolModels::ShiftedSabrToJson(d).dump(); });
	///*
	// @brief. Frequency Class
	//*/
	//py::class_<TPFrequency>(m, "TPFrequency")
	//	.def(py::init(&TPFrequency::fromString))
	//	.def(py::init(&TPFrequency::fromEnum))
	//	.def(py::init(&TPFrequency::bullet))
	//	.def(py::init<const TPTenor &>())
	//	.def("toEnum", [](const TPFrequency &d) {return d.toEnum(); })
	//	.def("toString", [](const TPFrequency &d) {return d.toString(); });

	//	//.def("value", [](const TPAny &d) {return d.o(); });
	//// Binding to chrono Date
	///*
	// * @brief Cut Class. This where we define the expriy time exactly
	// */
	//py::class_<TP::date_time::Cut>(m, "Cut")
	//	.def(py::init<const std::string &>())
	//	.def("hours",
	//		[](const TP::date_time::Cut& c)
	//{
	//	return c.hours().count();
	//})
	//	.def("minutes",
	//		[](const TP::date_time::Cut& c)
	//{
	//	return c.minutes().count();
	//})
	//	.def("total_minutes",
	//		[](const TP::date_time::Cut& c)
	//{
	//	return c.toMinutes();
	//})
	//	.def("to_string", [](const TP::date_time::Cut& c)
	//{
	//	const TP::Formatter<TP::date_time::Cut> fmt{};
	//	return fmt.str(c);
	//});

	//m.def("make_cut", &makeCut, R"(Creates an Expiry Cut)",
	//	py::arg("TimeZone"),
	//	py::arg("WhenOrNothing"));
	//m.def("dates_lower_and_upper_Bound", &dateLowerAndUpperBound, R"(lower and upper bound for the given date in the list of dates)",
	//	"Vector of dates"_a, "AsOfDate"_a);

	///*
	// * @brief Time Class. This where we specify time to expiry
	// */
	//py::class_<TP::date_time::Time>(m, "Time")
	//	.def(py::init<const TPDate &, const TP::date_time::Cut&>())
	//	.def("sysSeconds", &TP::date_time::Time::sysSeconds)
	//	.def("dcf", &TP::date_time::Time::dcf);

	//// Looks like the only interface we can have is with the old TPDate.
	//// New is causing alot of compilation errors...
	//py::class_<TPDate>(m, "TPDate")
	//	.def(py::init<const std::string &>())
	//	.def(py::init<int, int, int>()) // day,month, year ctor
	//	// Allow construction from Python date
	//	.def(py::init([](const py::object& obj) {
	//	try {
	//		// Check if it's a datetime.date object
	//		if (py::isinstance<py::object>(obj)) {
	//			auto date_module = py::module::import("datetime");
	//			auto date_type = date_module.attr("date");

	//			if (py::isinstance(obj, date_type)) {
	//				int year = obj.attr("year").cast<int>();
	//				int month = obj.attr("month").cast<int>();
	//				int day = obj.attr("day").cast<int>();
	//				return TPDate(day, month, year);
	//			}
	//		}
	//		throw py::type_error("Expected datetime.date or (year, month, day) tuple");
	//	}
	//	catch (const py::error_already_set& e) {
	//		throw py::type_error("Could not convert to TPDate");
	//	}
	//}))
	//	// converty to pythong date
	//	.def("to_python_date", [](const TPDate& d) {
	//	auto date_module = py::module::import("datetime");
	//	return date_module.attr("date")(d.year(), d.month(), d.day());
	//})
	//	.def("toJulian",[](const TPDate &d) {return d.getJulianDate();	})
	//	.def(py::self == py::self)
	//	.def(py::self != py::self)
	//	.def(py::self > py::self)
	//	.def(py::self < py::self)
	//	.def(py::self >= py::self)
	//	.def(py::self <= py::self)
	//	.def("toString", [](const TPDate& d) {return d.ToEURString(); })
	//	.def("__repr__", [](const TPDate &a) {		return "<TPDate:'" + a.toString() + "'>";	}
	//);
	//py::implicitly_convertible<std::string, TPDate>();
	//py::implicitly_convertible<py::object, TPDate>();

	///*
	// * @brief. Location of the development configuration directory
	// */
	//m.def("get_config_dir", &TP::api::getConfigDir, R"(CGet Dev Configuration directory)");
	///*
	// * @brief. Create a SABR interest rate Vol Cube
	//*/
	//m.def("createIRVolCube", &createIRVolCube, R"(Creates a Rates Vol Cube and saves it in Cache)",
	//	py::arg("AsOf"), py::arg("Currency"), py::arg("Input Sabr Map"), py::arg("(Label)"));
	//m.def("create_exchange_vanilla_IRVolCube", &createExchangeIRVolCube, R"(Creates an exchange Rates Vol Cube and saves it in Cache)",
	//	py::arg("AsOf"), py::arg("Currency"), py::arg("Input Sabr Map"), py::arg("base_contract"), py::arg("(Label)"));
	//m.def("serialise_vanilla_vol", &serialiseVanillaVolCube, R"(Serialise Vanilla VolCube)",
	//	py::arg("VolCube"), py::arg("Currency"), py::arg("path") = "C:\\work\\development\\Data\\VolCubes");

	//m.def("getVol", &getVol, R"(Vol interpolation in the Vol Cube)",
	//	py::arg("fwd"), py::arg("expiry Date/Period"), py::arg("Tenor"), py::arg("Strike"), py::arg("Vol Cube"));
	//m.def("vol_cube_asof_date", &getAsOfDate, R"(Get the build/asofdate of the Vol Cube)", py::arg("Vol Cube"));
	///*
	// * @brief. Create a Exchange MidCurve SABR interest rate Vol Cube
	//*/
	//m.def("create_exchange_IRVolCube", &createExchangeMidCurveIRVolCube, R"(Creates an exchange Rates MidCurve Vol Cube and saves it in Cache)",
	//	py::arg("AsOf"), py::arg("Currency"), py::arg("Input Sabr Map"), py::arg("TemplateID") = std::nullopt,
	//	py::arg("(Label)") = std::nullopt);
	//m.def("get_midcurve_vol", &getMidCurveVol, R"(Vol interpolation of the MidCurve Vol Cube)",
	//	py::arg("fwd"), py::arg("expiry Data"), py::arg("fwd_tenor"), py::arg("Tenor"), py::arg("Strike"), py::arg("Vol Cube"));
	//m.def("serialise_midcurve_vol", &serialiseExchangeVolCube, R"(Serialise VolCube)",
	//	py::arg("VolCube"), py::arg("Currency"), py::arg("path") = "C:\\work\\development\\Data\\Listed");
	//m.def("load_midcurve_vol_cubes", &loadExchangeVolCubes, R"(Load VolCubes from json files)",
	//	py::arg("Dates"), py::arg("Currency"), py::arg("templateID"), py::arg("path") = "C:\\work\\development\\Data\\Listed");
	//m.def("load_vanilla_vol_cubes", &loadVanillaVolCubes, R"(Load Vanilla VolCubes(mainly for bond future options) from json files)",
	//	py::arg("Dates"), py::arg("Currency"), py::arg("templateID"), py::arg("path") = "C:\\work\\development\\Data\\VolCubes");
	//m.def("load_swaption_vol_cubes_from_csv", &loadSwaptionVolCubes, R"(Load Swaptions VolCubes from calibrated csv file(Reads csv file and load cube in memory. 
 //        Assumes there exist and an options TEmplate ID same as the ccy and loaded via OptionTemplate.csv file)",
	//	py::arg("Dates"), py::arg("Currency"), py::arg("expiry_interpolation") = "VARIANCE_LINEAR",
	//	py::arg("tenor_interpolation") = "LINEAR",
	//	py::arg("extrapolation") = "FLAT",
	//	py::arg("path") = "X:\\sharing\\data\\SABRCalibration",
	//	py::arg("verbose") = false);

	//m.def("load_bond_future_sabr_grid", &loadBondFutureSABRGrid, R"(Loads the calibrated Bond Future Sabr Grid)",
	//	py::arg("LoadDate"), "Currency"_a, "model_type"_a, py::arg("Tag") = std::nullopt,
	//	py::arg("asof") = std::nullopt, py::arg("path") = "C:\\work\\development\\Data\\VolCubes\\");
	//m.def("sabr_grid_to_csv", &SABRGridToCsv, R"(Saves the SABR Grid object from memory to csv 
	//	By default it saves to X:\\Macro_Tools\\Vols\\)",
	//	"sabr_grid_name"_a, py::arg("asof") = std::nullopt);
	///*
	// * Market Data Curves. REquired for curve construction
	// */
	//m.def("display_market_data_cache", &displayMarketDataCache, R"(Display Market Data Cache)");

	//m.def("create_mm_curve", (std::string(*) (const TPDate&, const std::string&,
	//	const std::vector<std::string>&,
	//	const std::map<std::string, double>&,
	//	const std::string&)) &createMoneyMarketCurve,
	//	R"(Creates a Money Market curve and saves it in Cache)",
	//	py::arg("AsOf"), py::arg("Currency"), py::arg("mm defaults (i.e. mMF, ACT/360, 2D, EUR)"),
	//	py::arg("mm data (map of string/rates"), py::arg("(Label/Tag)"));

	//m.def("create_futures_curve", (std::string(*) (const TPDate&, const std::string&,
	//	const std::vector<TPAny>&,
	//	const std::map<TPDate, std::vector<double>>&,
	//	const std::string&)) &createFuturesCurve,
	//	R"(Creates a STIR Futures curve and saves it in Cache)",
	//	py::arg("AsOf"), py::arg("libor string"), py::arg("Futures defaults (i.e. IMM, 0.01, 25, 2)"),
	//	py::arg("futures data (map of string/rates/convexity"), py::arg("(Label/Tag)"));

	//m.def("create_swaps_curve", (std::string(*) (const TPDate&, const std::string&,
	//	const std::string&,
	//	const std::map<std::string, double>&,
	//	const std::string&)) &createSwapCurve,
	//	R"(Creates a Swaps Market curve(Tenor, rate) and saves it in Cache. swap Defaults will define the convention)",
	//	py::arg("AsOf"), py::arg("Currency"), py::arg("Swap Index Type which defines swaps conventions"),
	//	py::arg("data (map of string/rates"), py::arg("(Label/Tag)"));

	//m.def("create_zero_curve", &createZeroCouponCurve, R"(Creates a Simple Zero Coupon object for P(0,T) interpolation)",
	//	py::arg("AsOf"), py::arg("Libor Index Tag"), py::arg("vector of dates"),
	//	py::arg("vector of zero rates"), py::arg("(Label/Tag)") = std::nullopt);

	//m.def("get_swap_type_info", &getSwapIndexTypeInfo, R"(get info about the swap index type)",
	//	py::arg("Swap Index Type"));
	//// This has to be replaced at some point with some kind of reflective pattern to get all info about the index
	//m.def("get_ibor_ccy", &getIborCcy, R"(get the Ccy of the index)", py::arg("Ibor Index"));

	//m.def("get_zc_curve", &getAllZeroCouponCurves, R"(Get All Zero Coupon Curves from cache memory)");
	//m.def("get_df", &getDF, R"(Get Df from basic Zero Curve)", py::arg("AsOf"), py::arg("ZC curve Tag"));

	//m.def("get_market_curve", &getMarketCurve, R"(Get Market Curve data)",
	//	py::arg("Market Curve Name"));


	///*m.def("create_futures_curve", &createFuturesCurve, R"(Creates a STIR futures curve and saves it in Cache)",
	//	py::arg("AsOf"), py::arg("libor_index"), py::arg("futures defaults (i.e. mMF, ACT/360, 2D, EUR)"), py::arg("(Label/Tag)"));*/
	//	/*
	//	  @grief. Calendar Functions..
	//	*/
	//m.def("loadCalendar", &loadCalendar, R"(Creates a Calendar given the full pathe name of the Json file and saves it in Cache)",
	//	py::arg("Full path"));



	//py::class_<pyMatrix>(m, "pyMatrix", py::buffer_protocol())
	//	.def(py::init<py::ssize_t, py::ssize_t>())
	//	.def(py::init([](py::buffer const b) {
	//	py::buffer_info info = b.request();
	//	if (info.ndim != 2)
	//		throw std::runtime_error("Incompatible buffer format!");

	//	auto v = new pyMatrix(TPMatrix<double>::create((double *)info.ptr, info.shape[0], info.shape[1]));
	//	return v;
	//}))
	//	.def("rows", &pyMatrix::RowNo)
	//	.def("cols", &pyMatrix::ColNo)
	//	// barebone interface
	//	.def("__getitem__", [](const pyMatrix& m, std::pair<py::ssize_t, py::ssize_t> i) {
	//	if (i.first >= m.RowNo() || i.second >= m.ColNo())
	//		throw py::index_error();
	//	return m.at(i.first, i.second);
	//})
	//	.def("__setitem__", [](pyMatrix& m, std::pair<py::ssize_t, py::ssize_t> i, float v) {
	//	if (i.first >= m.RowNo() || i.second >= m.ColNo())
	//		throw py::index_error();
	//	m.at(i.first, i.second) = v;
	//})
	//	.def_buffer([](pyMatrix& m) -> py::buffer_info {
	//	return py::buffer_info(
	//		m.pointerdata(),
	//		{ m.RowNo(), m.ColNo() },
	//		{ sizeof(double) * size_t(m.ColNo()),
	//		sizeof(double) });
	//});


	///*
	// * @brief.  Model Building Section
	// */
	//m.def("build_flat_bond_discount_model", &TP::niko_interface::createFlatBondDiscountModel, R"(Flat Bond discount Mdoel)",
	//	"AsOfDate"_a, "vector of gov indices"_a, "vector of flat rates"_a, "Model Tag"_a);

	//m.def("build_bond_discount_model", &buildBondDiscountModel, R"(Flat fwd Bond discount Mdoel)",
	//	"AsOfDate"_a, "market_data_collection"_a, "vector of building details"_a, "gov_index"_a, "Model Tag"_a);
	//m.def("get_fwds_at_pillars", &getFwdsAtPillars, R"(Get the Unit Forward Rates at the Pillar Dates)",
	//	"Model"_a, "index_string"_a);

	//// Single Unit Libor Disconut Model
	//m.def("build_libor_discount_model", &createLiborDiscountModel, R"(Single Unit old world Libor Unit)",
	//	"AsOfDate"_a, "market_data_collection"_a, "ccy"_a, "vector of building details"_a, "libor_index"_a, "Model Tag"_a, py::arg("root_method") = std::nullopt);
	///*
	// * @brief.  Pricing Section
	// */
	//m.def("price_bond", &valueBond, R"(Value a Bond using only  Analytic Formulae (i.e. no Curve)",
	//	"Product"_a, "Model"_a, "algorithm"_a, py::arg("useCurve") = false);
	//m.def("price_fwd_bond", &valueFwdBond, R"(Value a Bond forward using the spot Clean Price and repo rate)",
	//	"Product"_a, "Model"_a, "algorithm"_a, "fwd_date"_a, "spot_clean_price"_a, "repo_rate"_a);
	//m.def("get_forward_rate", &getForwardRate, R"(Returns  a forward date given dates, model and index)",
	//	"start_date"_a, "end_date"_a, "Model"_a, "index"_a);
	//m.def("get_forward_rates", &getForwardRates, R"(Returns  a forward date given expiries and tenors as strings, model and index)",
	//	"start_dates"_a, "end_dates"_a, "Model"_a, "index"_a);
	//m.def("price_cmt_bonds", &valueCMTBonds, R"(Value CMT Bonds for the given tenors)",
	//	"Tenors"_a, "templateID"_a, "Model"_a);

	//m.def("value_index", &valueIndex, R"(Get forward rate of the given index)",
	//	"reset_date"_a, "index"_a, "Model"_a, "algorithm"_a, py::arg("use_projection") = true);
	///*
	// * @brief. Arbitrary naive numerical integration. THIS HAS TO BE IMPROVED BY A LOT!
	// */
	//m.def("numerical_payoff_integration", &valueNumericalPayOff, R"(Arbitrary numerical payoff integration based on some input vols and payoff vectors)",
	//	"dfwds"_a, "payoff"_a, "rel_strikes"_a, "vols"_a, "expiry_tau"_a);
	///*
	// * Utils.
	// */
	//m.def("copy_file", &copyFile, R"(Copy a File from one location to the other but checks if it's there already.)",
	//	"source_dir"_a, "dest_dir"_a, "file"_a, py::arg("overwrite") = false);
	//m.def("copy_dir", &copyDir, R"(Copy Files from the destination directory to the source directory
	//	Recursive will copy all subdirectories and create_target will create target directory if not there. Note that by
	//	default files that exist in both target and source are not copied.)",
	//	"source_dir"_a, "dest_dir"_a, py::arg("recursive") = false, py::arg("create_target") = true);

	///*
	// *  Pricing functions for MidCurve and Forward Vols
	// */
	//m.def("mid_curve_swaption_pv", &MidCurveSwaptionPV, R"(PV of MidCurveSwaption)",
	//	"Strike"_a, "Expiry"_a, "Forward_long"_a, "Annuity_long"_a, "vol_long"_a,
	//	"Forward_short"_a, "Annuity_short"_a, "vol_short"_a, "model_type"_a, "product_type(call/put)"_a,
	//	"input_type(pv/vol/corr)"_a, "input_value"_a);

	//m.def("mid_curve_swaption_vol", &MidCurveSwaptionVol, R"(Vol of MidCurveSwaption)",
	//	"Strike"_a, "Expiry"_a, "Forward_long"_a, "Annuity_long"_a, "vol_long"_a,
	//	"Forward_short"_a, "Annuity_short"_a, "vol_short"_a, "model_type"_a, "product_type(call/put)"_a,
	//	"input_type(pv/vol/corr)"_a, "input_value"_a);

	//m.def("mid_curve_swaption_corr", &TPMidCurveSwaptionCorr, R"(Implied Correlation of MidCurveSwaption)",
	//	"Strike"_a, "Expiry"_a, "Forward_long"_a, "Annuity_long"_a, "vol_long"_a,
	//	"Forward_short"_a, "Annuity_short"_a, "vol_short"_a, "model_type"_a, "product_type(call/put)"_a,
	//	"input_type(pv/vol/corr)"_a, "input_value"_a);

	///*
	// *  Currency and FX
	//*/

	//m.def("is_valid_ccy", &isValidCurrency, R"(Checks if currency is Valid)", "Currency"_a);
	///*
	//  * DATATABLE/DATAFRAME FUNCTIONS
	// */
	//init_df(m);
	///*
	// *
	// * TIME SERIES FUNCTIONS
	// */
	// TP::niko_interface::time_series_tools::init(m);

	// /*
	//  *
	//  * CURVE FUNCTIONS
	//  */
	// TP::niko_interface::curve_tools::init(m);
	// /*
	//  *  PRODUCT TOOLS
	//  */
	// TP::niko_interface::product_tools::init(m);
	// /*
	// *  DATE TOOLS
	// */
	// init_dts_cals(m);
	// /*
	//  * 
	// */
	// TP::niko_interface::index_tools::init(m);
	 
}
