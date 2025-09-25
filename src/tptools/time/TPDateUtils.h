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

#ifndef TP_DATE_UTILS_H
#define TP_DATE_UTILS_H

#include <string>
#include <TPNikopolisUtils.h>
#include <set>

template<typename  T> const T& TPMax(const T& a, const T& b) { return ( a > b ? a : b) ;}
template<typename T> const T& TPMin(const T& a, const T& b) { return ( a < b ? a : b) ;}
const double DAYS_IN_YEAR_STOCH = 365.25;

enum TP_ARR_ADV
{
  ARR,	
  ADV,
  INVALID
};

enum CASH_DATE_CONVENTION
{
	CCUNDEFINED = -1,
	CCSTUBSTART = 0,
	CCSTUBEND = 1,
	CCROLLDATE = 2,
	CCIMM = 3,
	CCENDOFMONTH = 4
};

enum TP_City
{
	TP_INVALID_CITY = -1,

	TP_BRA = 1,			// Bratislava
	TP_BRU = 2,			// Bruxell
	TP_EUR = 3,			// Paris, Berlin...
	TP_FRA = 4,			// Frankfort
	TP_HKG = 5,			// Hong kong
	TP_LON = 6,			// London
	TP_MAD = 7,			// Madrid
	TP_NY = 8,			// New York
	TP_PAR = 9,			// Paris
	TP_STO = 10,		// Stockholm
	TP_SIN = 11,		// Singapour
	TP_SYD = 12,		// Sydney
	TP_AKL = 13,		// Auckland
	TP_TOK = 14,		// Tokyo
	TP_SEL = 15,		// Seoul
	TP_TOR = 16,		// Toronto
	TP_ZUR = 17,		// Zurich
	TP_COP = 18,		// Copenhagen
	TP_OSL = 19,		// Oslo
	TP_MCW = 20,		// Moscow
	TP_VAR = 21,		// Varsovia
	TP_BUD = 22,		// Budapest
	TP_BUA = 23,		// Buenos Aires
	TP_BRS = 24,		// Brasilia
	TP_BJG = 25,		// Beijing
	TP_JAK = 26,        // Jakarta
	TP_PRT = 27,		// Pretoria
	TP_ANK = 28,		// Ankara
	TP_ALG = 29,		// Algiers
	TP_TUN = 30,		// Tunis
	TP_ABD = 31,		// Abu Dhabi
	TP_MAN = 32,		// Manama
	TP_DHK = 33,		// Dhaka
	TP_CAI = 34,		// Cairo
	TP_JER = 35,		// Jerusalem
	TP_KUC = 36,		// Kuwait City
	TP_RIY = 37,		// Riyadh
	TP_KAB = 38,		// Kabul
	TP_BSB = 39,		// Bandar Seri Begawan (Brunei)
	TP_BOG = 40,		// Bogota
	TP_SJS = 41,		// San Jose (Costa Rica)
	TP_MXO = 42,		// Mexico
	TP_MUM = 43,		// Mumbai
	TP_PRA = 44,		// Prague
	TP_REK = 45,		// Rekyavik
	TP_TAI = 46,			// Taipei
	TP_MLN = 47,			// Milan
	TP_LIS = 48			// Lisbon
};

inline std::string  toString(TP_City city)
{
	if (city == TP_BRA) return std::string("BRA");

	else if (city == TP_BRU) return std::string("BRU");
	else if (city == TP_EUR) return std::string("EUR");
	else if (city == TP_FRA) return std::string("FRA");
	else if (city == TP_HKG) return std::string("HKG");
	else if (city == TP_LON) return std::string("LON");
	else if (city == TP_MAD) return std::string("MAD");
	else if (city == TP_NY)  return std::string("NY");
	else if (city == TP_PAR) return std::string("PAR");
	else if (city == TP_STO) return std::string("STO");
	else if (city == TP_SIN) return std::string("SIN");
	else if (city == TP_SYD) return std::string("SYD");
	else if (city == TP_AKL) return std::string("AKL");
	else if (city == TP_TOK) return std::string("TOK");
	else if (city == TP_SEL) return std::string("SEL");
	else if (city == TP_TOR) return std::string("TOR");
	else if (city == TP_ZUR) return std::string("ZUR");
	else if (city == TP_COP) return std::string("COP");
	else if (city == TP_OSL) return std::string("OSL");
	else if (city == TP_MCW) return std::string("MCW");
	else if (city == TP_VAR) return std::string("VAR");
	else if (city == TP_BUD) return std::string("BUD");
	else if (city == TP_BUA) return std::string("BUA");
	else if (city == TP_BRS) return std::string("BRS");
	else if (city == TP_BJG) return std::string("BJG");
	else if (city == TP_JAK) return std::string("JAK");
	else if (city == TP_PRT) return std::string("PRT");
	else if (city == TP_ANK) return std::string("ANK");
	else if (city == TP_ALG) return std::string("ALG");
	else if (city == TP_TUN) return std::string("TUN");
	else if (city == TP_ABD) return std::string("ABD");
	else if (city == TP_MAN) return std::string("MAN");
	else if (city == TP_DHK) return std::string("DHK");
	else if (city == TP_CAI) return std::string("CAI");
	else if (city == TP_JER) return std::string("JER");
	else if (city == TP_KUC) return std::string("KUC");
	else if (city == TP_RIY) return std::string("RIY");
	else if (city == TP_KAB) return std::string("KAB");
	else if (city == TP_BSB) return std::string("BSB");
	else if (city == TP_BOG) return std::string("BOG");
	else if (city == TP_SJS) return std::string("SJS");
	else if (city == TP_MXO) return std::string("MXO");
	else if (city == TP_MUM) return std::string("MUM");
	else if (city == TP_PRA) return std::string("PRA");
	else if (city == TP_REK) return std::string("REK");
	else if (city == TP_TAI) return std::string("TAI");
	else if (city == TP_MLN) return std::string("MLN");
	else if (city == TP_LIS) return std::string("LIS");

	else return std::string("INVALID_CITY");
}

inline TP_City  toCity(const std::string& city)
{
	if (city == std::string("BRA")) return TP_BRA;

	else if (city == std::string("BRU")) return TP_BRU;
	else if (city == std::string("EUR")) return TP_EUR;
	else if (city == std::string("FRA")) return TP_FRA;
	else if (city == std::string("HKG")) return TP_HKG;
	else if (city == std::string("LON")) return TP_LON;
	else if (city == std::string("MAD")) return TP_MAD;
	else if (city == std::string("NY"))  return TP_NY;
	else if (city == std::string("PAR")) return TP_PAR;
	else if (city == std::string("STO")) return TP_STO;
	else if (city == std::string("SIN")) return TP_SIN;
	else if (city == std::string("SYD")) return TP_SYD;
	else if (city == std::string("AKL")) return TP_AKL;
	else if (city == std::string("TOK")) return TP_TOK;
	else if (city == std::string("SEL")) return TP_SEL;
	else if (city == std::string("TOR")) return TP_TOR;
	else if (city == std::string("ZUR")) return TP_ZUR;
	else if (city == std::string("COP")) return TP_COP;
	else if (city == std::string("OSL")) return TP_OSL;
	else if (city == std::string("MCW")) return TP_MCW;
	else if (city == std::string("VAR")) return TP_VAR;
	else if (city == std::string("BUD")) return TP_BUD;
	else if (city == std::string("BUA")) return TP_BUA;
	else if (city == std::string("BRS")) return TP_BRS;
	else if (city == std::string("BJG")) return TP_BJG;
	else if (city == std::string("JAK")) return TP_JAK;
	else if (city == std::string("PRT")) return TP_PRT;
	else if (city == std::string("ANK")) return TP_ANK;
	else if (city == std::string("ALG")) return TP_ALG;
	else if (city == std::string("TUN")) return TP_TUN;
	else if (city == std::string("ABD")) return TP_ABD;
	else if (city == std::string("MAN")) return TP_MAN;
	else if (city == std::string("DHK")) return TP_DHK;
	else if (city == std::string("CAI")) return TP_CAI;
	else if (city == std::string("JER")) return TP_JER;
	else if (city == std::string("KUC")) return TP_KUC;
	else if (city == std::string("RIY")) return TP_RIY;
	else if (city == std::string("KAB")) return TP_KAB;
	else if (city == std::string("BSB")) return TP_BSB;
	else if (city == std::string("BOG")) return TP_BOG;
	else if (city == std::string("SJS")) return TP_SJS;
	else if (city == std::string("MXO")) return TP_MXO;
	else if (city == std::string("MUM")) return TP_MUM;
	else if (city == std::string("PAR")) return TP_PRA;
	else if (city == std::string("REK")) return TP_REK;
	else if (city == std::string("TAI")) return TP_TAI;
	else if (city == std::string("MLN")) return TP_MLN;
	else if (city == std::string("LIS")) return TP_LIS;

	else return TP_INVALID_CITY;
}

inline std::string  FromCitytoCcy(TP_City city)
{
	if (city == TP_BRA) return std::string("EUR");

	else if (city == TP_BRU) return std::string("EUR");
	else if (city == TP_EUR) return std::string("EUR");
	else if (city == TP_FRA) return std::string("EUR");
	else if (city == TP_PAR) return std::string("EUR");
	else if (city == TP_MAD) return std::string("EUR");
	else if (city == TP_HKG) return std::string("HKD");
	else if (city == TP_LON) return std::string("GBP");
	else if (city == TP_NY)  return std::string("USD");
	else if (city == TP_STO) return std::string("SEK");
	else if (city == TP_SIN) return std::string("SGD");
	else if (city == TP_SYD) return std::string("AUD");
	else if (city == TP_AKL) return std::string("NZD");
	else if (city == TP_TOK) return std::string("JPY");
	else if (city == TP_SEL) return std::string("KRW");
	else if (city == TP_TOR) return std::string("CAD");
	else if (city == TP_ZUR) return std::string("CHF");
	else if (city == TP_COP) return std::string("DKK");
	else if (city == TP_OSL) return std::string("NOK");
	else if (city == TP_MCW) return std::string("RUB");
	else if (city == TP_VAR) return std::string("PLN");
	else if (city == TP_BUD) return std::string("HUF");
	else if (city == TP_BUA) return std::string("ARS");
	else if (city == TP_BRS) return std::string("BRL");
	else if (city == TP_BJG) return std::string("CNY");
	else if (city == TP_JAK) return std::string("IDR");
	else if (city == TP_PRT) return std::string("ZAR");
	else if (city == TP_ANK) return std::string("TRY");
	else if (city == TP_ALG) return std::string("DZD");
	else if (city == TP_TUN) return std::string("TUD");
	else if (city == TP_ABD) return std::string("AED");
	else if (city == TP_MAN) return std::string("BHD");
	else if (city == TP_DHK) return std::string("BDT");
	else if (city == TP_CAI) return std::string("EGP");
	else if (city == TP_JER) return std::string("ILS");
	else if (city == TP_KUC) return std::string("KWD");
	else if (city == TP_RIY) return std::string("SAR");
	else if (city == TP_KAB) return std::string("AFN");
	else if (city == TP_BSB) return std::string("BND");
	else if (city == TP_BOG) return std::string("COP");
	else if (city == TP_SJS) return std::string("CRC");
	else if (city == TP_MXO) return std::string("MXN");
	else if (city == TP_MUM) return std::string("INR");
	else if (city == TP_PRA) return std::string("CZK");
	else if (city == TP_REK) return std::string("ISK");
	else if (city == TP_TAI) return std::string("TWD");
	else if (city == TP_MLN) return std::string("EUR");
	else if (city == TP_LIS) return std::string("EUR");

	else return std::string("INVALID_CITY");
}

inline TP_City  FromCcytoCity(const std::string& Ccy)
{
	if (Ccy == std::string("EUR")) return TP_EUR;

	else if (Ccy == std::string("HKD")) return TP_HKG;
	else if (Ccy == std::string("GBP")) return TP_LON;
	else if (Ccy == std::string("USD")) return TP_NY;
	else if (Ccy == std::string("SEK")) return TP_STO;
	else if (Ccy == std::string("SGD")) return TP_SIN;
	else if (Ccy == std::string("AUD")) return TP_SYD;
	else if (Ccy == std::string("NZD")) return TP_AKL;
	else if (Ccy == std::string("JPY")) return TP_TOK;
	else if (Ccy == std::string("KRW")) return TP_SEL;
	else if (Ccy == std::string("CAD")) return TP_TOR;
	else if (Ccy == std::string("CHF")) return TP_ZUR;
	else if (Ccy == std::string("DKK")) return TP_COP;
	else if (Ccy == std::string("NOK")) return TP_OSL;
	else if (Ccy == std::string("RUB")) return TP_MCW;
	else if (Ccy == std::string("PLN")) return TP_VAR;
	else if (Ccy == std::string("HUF")) return TP_BUD;
	else if (Ccy == std::string("ARS")) return TP_BUA;
	else if (Ccy == std::string("BRL")) return TP_BRS;
	else if (Ccy == std::string("CNY")) return TP_BJG;
	else if (Ccy == std::string("IDR")) return TP_JAK;
	else if (Ccy == std::string("ZAR")) return TP_PRT;
	else if (Ccy == std::string("TRY")) return TP_ANK;
	else if (Ccy == std::string("DZD")) return TP_ALG;
	else if (Ccy == std::string("TUD")) return TP_TUN;
	else if (Ccy == std::string("AED")) return TP_ABD;
	else if (Ccy == std::string("BHD")) return TP_MAN;
	else if (Ccy == std::string("BDT")) return TP_DHK;
	else if (Ccy == std::string("EGP")) return TP_CAI;
	else if (Ccy == std::string("ILS")) return TP_JER;
	else if (Ccy == std::string("KWD")) return TP_KUC;
	else if (Ccy == std::string("SAR")) return TP_RIY;
	else if (Ccy == std::string("AFN")) return TP_KAB;
	else if (Ccy == std::string("BND")) return TP_BSB;
	else if (Ccy == std::string("COP")) return TP_BOG;
	else if (Ccy == std::string("CRC")) return TP_SJS;
	else if (Ccy == std::string("MXN")) return TP_MXO;
	else if (Ccy == std::string("INR")) return TP_MUM;
	else if (Ccy == std::string("CZK")) return TP_PRA;
	else if (Ccy == std::string("ISK")) return TP_REK;
	else if (Ccy == std::string("TWD")) return TP_TAI;

	else return TP_INVALID_CITY;
}

inline TP_ARR_ADV  toArrears(const std::string& str_arrears)
{
	if(str_arrears == "ARR" || str_arrears == "arr" || str_arrears == "ARREARS" || str_arrears == "arrears")
		return ARR;
	else if(str_arrears == "ADV" || str_arrears == "adv" || str_arrears == "ADVANCE" || str_arrears == "advance")
		return ADV;
	else
		return INVALID;
}

#include "TPTools\Time\TPTenor.h"
#include "TPTools\TPException.h"
namespace TP
{
	namespace date_time
	{
		inline unsigned tenorToFrequency(const TP::Core::TPTenor& tenor)
		{
			if (tenor == TP::Core::TPTenor("0D"))
				return 0U;
			if (tenor == TP::Core::TPTenor("1W"))
				return 52U;
			if (tenor == TP::Core::TPTenor("2W"))
				return 26U;
			if (tenor == TP::Core::TPTenor("1M"))
				return 12U;
			if (tenor == TP::Core::TPTenor("2M"))
				return 6U;
			if (tenor == TP::Core::TPTenor("3M"))
				return 4U;
			if (tenor == TP::Core::TPTenor("4M"))
				return 3U;
			if (tenor == TP::Core::TPTenor("6M"))
				return 2U;
			if (tenor == TP::Core::TPTenor("12M") || tenor == TP::Core::TPTenor("1Y"))
				return 1U;

			THROW("CAnnot convert from Tenor to Frequency");
		}
	}
}
#endif // TP_DATE_UTILS_H
