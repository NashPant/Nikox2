#pragma once
#include "TPNikopolisUtils.h"
//#include "TPTools/Utils/Operators.h"
#include "TPNikopolisUtils.h"
#include "TPTools/Utils/Formatting.h"
#include <optional>
#include <string>
//#include <cereal/cereal.hpp>
//#include  <cereal/types/string.hpp>

#include <fmt/ostream.h>
#include <initializer_list>
//#include "TPCcy.h"
#include <map>

namespace TP
{
	struct Currency;
	using OptCurrency = std::optional<Currency>;

	struct  CurrencyData final
	{
		std::string		const code;
		int              const id;
		int              const minorUnit;
		std::string const name;
		bool             const onshore;
		bool             const official;

		template<class Archive>
		void save(Archive & archive) const
		{
			archive(code, id, minorUnit, name, onshore, official);
		}
		
	};


	struct TPNIKOPOLIS_IMPEXP Currency final // : operators::totally_ordered<Currency>
	{
		std::string_view  code;
		int               id;
		int               minorUnit;
		std::string_view  name;
		bool              onshore; // == non-deliverable
		bool              official; // is the currency official for trading (i.e. not a unit of measure, accounting currency, commodity...). One per country

		/*
		 * \brief default country is USD 
		 */
		Currency() noexcept;
		/*
		 * @brief. Default assigment operatos
		 */
		const Currency& operator=(const Currency& rhs);

		/**
		 * \brief builds a currency from id, will throw if not a valid currency (as defined in the list below)
		 */
		explicit Currency(int id);

		/**
		 * \brief builds a currency from ISO code (e.g. 'USD'), will throw if the code is not a valid currency (as defined in the list below)
		 */
		explicit Currency(std::string_view code);

		/**
		 * \brief if the id is valid (i.e. is a member of the currencies list) return the corresponding currency it otherwise return nullopt
		 */
		static OptCurrency maybeGet(int id);
		/*
		 * @brief. Create an implicit constructor from TPCcy
		 */
		//static Currency fromTPCcy(const TPCcy& tpccy);

		/**
		 * \brief if the ISO code is valid  (i.e. is a member of the currencies list) return the corresponding currency it otherwise return nullopt
		 */
		static OptCurrency maybeGet(std::string_view code);

		/**
		 * \brief gets a valid string for the currency using code
		 */
		[[nodiscard]] std::string toString() const;

		// only used to generate currencies please don't use
		constexpr Currency(const std::string_view iso, const int id, const int minorUnit,
			const bool onshore, const bool official,
			const std::string_view name)
			: code(iso), id(id), minorUnit(minorUnit), name(name), onshore(onshore), official(official)
		{}

		/*template<typename Archive>
		void serialize(Archive& archive, TP::Currency& d)
		{
			archive(d.code, d.id, d.minorUnit, d.name, d.onshore, d.official);
		}*/
		//static std::map< CCY_TYPE, Currency> tpCcytoCurrencyMap;
	};

	constexpr bool operator==(Currency const & lhs, Currency const & rhs) noexcept { return lhs.id == rhs.id; }
	constexpr bool operator<(Currency const & lhs, Currency const & rhs) noexcept { return lhs.id < rhs.id; }

	template<class CharT, class Traits>
	std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const Currency& ccy)
	{
		os << ccy.code;
		return os;
	}

	namespace currency
	{
		// active ISO 4217 currency codes 
		// ReSharper disable CppInconsistentNaming
		constexpr Currency AED{ "AED", 784, 2, false, true, "United Arab Emirates dirham" };
		constexpr Currency AFN{ "AFN", 971, 2, false, true, "Afghan afghani" };
		constexpr Currency ALL{ "ALL",   8, 2, false, true, "Albanian lek" };
		constexpr Currency AMD{ "AMD",  51, 2, false, true, "Armenian dram" };
		constexpr Currency ANG{ "ANG", 532, 2, false, true, "Netherlands Antillean guilder" };
		constexpr Currency AOA{ "AOA", 973, 2, false, true, "Angolan kwanza" };
		constexpr Currency ARS{ "ARS",  32, 2, true, true, "Argentine peso" };
		constexpr Currency AUD{ "AUD",  36, 2, false, true, "Australian dollar" };
		constexpr Currency AWG{ "AWG", 533, 2, false, true, "Aruban florin" };
		constexpr Currency AZN{ "AZN", 944, 2, false, true, "Azerbaijani manat" };
		constexpr Currency BAM{ "BAM", 977, 2, false, true, "Bosnia and Herzegovina convertible mark" };
		constexpr Currency BBD{ "BBD",  52, 2, false, true, "Barbados dollar" };
		constexpr Currency BDT{ "BDT",  50, 2, false, true, "Bangladeshi taka" };
		constexpr Currency BGN{ "BGN", 975, 2, false, true, "Bulgarian lev" };
		constexpr Currency BHD{ "BHD",  48, 3, false, true, "Bahraini dinar" };
		constexpr Currency BIF{ "BIF", 108, 0, false, true, "Burundian franc" };
		constexpr Currency BMD{ "BMD",  60, 2, false, true, "Bermudian dollar" };
		constexpr Currency BND{ "BND",  96, 2, false, true, "Brunei dollar" };
		constexpr Currency BOB{ "BOB",  68, 2, false, true, "Boliviano" };
		constexpr Currency BOV{ "BOV", 984, 2, false, false, "Bolivian Mvdol" };
		constexpr Currency BRL{ "BRL", 986, 2, true, true, "Brazilian real" };
		constexpr Currency BSD{ "BSD",  44, 2, false, true, "Bahamian dollar" };
		constexpr Currency BTN{ "BTN",  64, 2, false, true, "Bhutanese ngultrum" };
		constexpr Currency BWP{ "BWP",  72, 2, false, true, "Botswana pula" };
		constexpr Currency BYN{ "BYN", 933, 2, false, true, "Belarusian ruble" };
		constexpr Currency BZD{ "BZD",  84, 2, false, true, "Belize dollar" };
		constexpr Currency CAD{ "CAD", 124, 2, false, true, "Canadian dollar" };
		constexpr Currency CDF{ "CDF", 976, 2, false, true, "Congolese franc" };
		constexpr Currency CHE{ "CHE", 947, 2, false, false, "WIR Euro" };
		constexpr Currency CHF{ "CHF", 756, 2, false, true, "Swiss franc" };
		constexpr Currency CHW{ "CHW", 948, 2, false, false, "WIR Franc" };
		constexpr Currency CLF{ "CLF", 990, 4, false, false, "Unidad de Fomento" };
		constexpr Currency CLP{ "CLP", 152, 0, true, true, "Chilean peso" };
		constexpr Currency CNY{ "CNY", 156, 2, true, true, "Renminbi" };
		constexpr Currency COP{ "COP", 170, 2, true, true, "Colombian peso" };
		constexpr Currency COU{ "COU", 970, 2, false, false, "Unidad de Valor Real" };
		constexpr Currency CRC{ "CRC", 188, 2, true, true, "Costa Rican colon" };
		constexpr Currency CUC{ "CUC", 931, 2, false, false, "Cuban convertible peso" };
		constexpr Currency CUP{ "CUP", 192, 2, false, true, "Cuban peso" };
		constexpr Currency CVE{ "CVE", 132, 0, false, true, "Cape Verde escudo" };
		constexpr Currency CZK{ "CZK", 203, 2, false, true, "Czech koruna" };
		constexpr Currency DJF{ "DJF", 262, 0, false, true, "Djiboutian franc" };
		constexpr Currency DKK{ "DKK", 208, 2, false, true, "Danish krone" };
		constexpr Currency DOP{ "DOP", 214, 2, false, true, "Dominican peso" };
		constexpr Currency DZD{ "DZD",  12, 2, false, true, "Algerian dinar" };
		constexpr Currency EGP{ "EGP", 818, 2, true, true, "Egyptian pound" };
		constexpr Currency ERN{ "ERN", 232, 2, false, true, "Eritrean nakfa" };
		constexpr Currency ETB{ "ETB", 230, 2, false, true, "Ethiopian birr" };
		constexpr Currency EUR{ "EUR", 978, 2, false, true, "Euro" };
		constexpr Currency FJD{ "FJD", 242, 2, false, true, "Fiji dollar" };
		constexpr Currency FKP{ "FKP", 238, 2, false, true, "Falkland Islands pound" };
		constexpr Currency GBP{ "GBP", 826, 2, false, true, "Pound sterling" };
		constexpr Currency GEL{ "GEL", 981, 2, false, true, "Georgian lari" };
		constexpr Currency GHS{ "GHS", 936, 2, false, true, "Ghanaian cedi" };
		constexpr Currency GIP{ "GIP", 292, 2, false, true, "Gibraltar pound" };
		constexpr Currency GMD{ "GMD", 270, 2, false, true, "Gambian dalasi" };
		constexpr Currency GNF{ "GNF", 324, 0, false, true, "Guinean franc" };
		constexpr Currency GTQ{ "GTQ", 320, 2, true, true, "Guatemalan quetzal" };
		constexpr Currency GYD{ "GYD", 328, 2, false, true, "Guyanese dollar" };
		constexpr Currency HKD{ "HKD", 344, 2, false, true, "Hong Kong dollar" };
		constexpr Currency HNL{ "HNL", 340, 2, false, true, "Honduran lempira" };
		constexpr Currency HRK{ "HRK", 191, 2, false, true, "Croatian kuna" };
		constexpr Currency HTG{ "HTG", 332, 2, false, false, "Haitian gourde" };
		constexpr Currency HUF{ "HUF", 348, 2, false, true, "Hungarian forint" };
		constexpr Currency IDR{ "IDR", 360, 2, true, true, "Indonesian rupiah" };
		constexpr Currency ILS{ "ILS", 376, 2, false, true, "Israeli new shekel" };
		constexpr Currency INR{ "INR", 356, 2, true, true, "Indian rupee" };
		constexpr Currency IQD{ "IQD", 368, 2, false, true, "Iraqi dinar" };
		constexpr Currency IRR{ "IRR", 364, 2, false, true, "Iranian rial" };
		constexpr Currency ISK{ "ISK", 352, 2, false, true, "Icelandic krona" };
		constexpr Currency JMD{ "JMD", 388, 2, false, true, "Jamaican dollar" };
		constexpr Currency JOD{ "JOD", 400, 3, false, true, "Jordanian dinar" };
		constexpr Currency JPY{ "JPY", 392, 0, false, true, "Japanese yen" };
		constexpr Currency KES{ "KES", 404, 2, false, true, "Kenyan shilling" };
		constexpr Currency KGS{ "KGS", 417, 2, false, true, "Kyrgyzstani som" };
		constexpr Currency KHR{ "KHR", 116, 2, false, true, "Cambodian riel" };
		constexpr Currency KMF{ "KMF", 174, 0, false, true, "Comoro franc" };
		constexpr Currency KPW{ "KPW", 408, 2, true, true, "North Korean won" };
		constexpr Currency KRW{ "KRW", 410, 0, true, true, "South Korean won" };
		constexpr Currency KWD{ "KWD", 414, 3, false, true, "Kuwaiti dinar" };
		constexpr Currency KYD{ "KYD", 136, 2, false, true, "Cayman Islands dollar" };
		constexpr Currency KZT{ "KZT", 398, 2, true, true, "Kazakhstani tenge" };
		constexpr Currency LAK{ "LAK", 418, 2, false, true, "Lao kip" };
		constexpr Currency LBP{ "LBP", 422, 2, false, true, "Lebanese pound" };
		constexpr Currency LKR{ "LKR", 144, 2, false, true, "Sri Lankan rupee" };
		constexpr Currency LRD{ "LRD", 430, 2, false, true, "Liberian dollar" };
		constexpr Currency LSL{ "LSL", 426, 2, false, true, "Lesotho loti" };
		constexpr Currency LYD{ "LYD", 434, 3, false, true, "Libyan dinar" };
		constexpr Currency MAD{ "MAD", 504, 2, false, true, "Moroccan dirham" };
		constexpr Currency MDL{ "MDL", 498, 2, false, true, "Moldovan leu" };
		constexpr Currency MGA{ "MGA", 969, 1, false, true, "Malagasy ariary" };
		constexpr Currency MKD{ "MKD", 807, 2, false, true, "Macedonian denar" };
		constexpr Currency MMK{ "MMK", 104, 2, false, true, "Myanmar kyat" };
		constexpr Currency MNT{ "MNT", 496, 2, false, true, "Mongolian something" };
		constexpr Currency MOP{ "MOP", 446, 2, false, true, "Macanese pataca" };
		constexpr Currency MRU{ "MRU", 929, 1, false, true, "Mauritanian ouguiya" };
		constexpr Currency MUR{ "MUR", 480, 2, false, true, "Mauritian rupee" };
		constexpr Currency MVR{ "MVR", 462, 2, false, true, "Maldivian rufiyaa" };
		constexpr Currency MWK{ "MWK", 454, 2, false, true, "Malawian kwacha" };
		constexpr Currency MXN{ "MXN", 484, 2, false, true, "Mexican peso" };
		constexpr Currency MXV{ "MXV", 979, 2, false, false, "Mexican Unidad de Inversion" };
		constexpr Currency MYR{ "MYR", 458, 2, true, true, "Malaysian ringgit" };
		constexpr Currency MZN{ "MZN", 943, 2, false, true, "Mozambican metical" };
		constexpr Currency NAD{ "NAD", 516, 2, false, true, "Namibian dollar" };
		constexpr Currency NGN{ "NGN", 566, 2, true, true, "Nigerian naira" };
		constexpr Currency NIO{ "NIO", 558, 2, false, true, "Nicaraguan cordoba" };
		constexpr Currency NOK{ "NOK", 578, 2, false, true, "Norwegian krone" };
		constexpr Currency NPR{ "NPR", 524, 2, false, true, "Nepalese rupee" };
		constexpr Currency NZD{ "NZD", 554, 2, false, true, "New Zealand dollar" };
		constexpr Currency OMR{ "OMR", 512, 3, false, true, "Omani rial" };
		constexpr Currency PAB{ "PAB", 590, 2, false, false, "Panamanian balboa" };
		constexpr Currency PEN{ "PEN", 604, 2, true, true, "Peruvian sol" };
		constexpr Currency PGK{ "PGK", 598, 2, false, true, "Papua New Guinean kina" };
		constexpr Currency PHP{ "PHP", 608, 2, true, true, "Philippine peso" };
		constexpr Currency PKR{ "PKR", 586, 2, false, true, "Pakistani rupee" };
		constexpr Currency PLN{ "PLN", 985, 2, false, true, "Polish zloty" };
		constexpr Currency PYG{ "PYG", 600, 0, false, true, "Paraguayan guarani" };
		constexpr Currency QAR{ "QAR", 634, 2, false, true, "Qatari riyal" };
		constexpr Currency RON{ "RON", 946, 2, false, true, "Romanian leu" };
		constexpr Currency RSD{ "RSD", 941, 2, false, true, "Serbian dinar" };
		constexpr Currency RUB{ "RUB", 643, 2, false, true, "Russian ruble" };
		constexpr Currency RWF{ "RWF", 646, 0, false, true, "Rwandan franc" };
		constexpr Currency SAR{ "SAR", 682, 2, false, true, "Saudi riyal" };
		constexpr Currency SBD{ "SBD",  90, 2, false, true, "Solomon Islands dollar" };
		constexpr Currency SCR{ "SCR", 690, 2, false, true, "Seychelles rupee" };
		constexpr Currency SDG{ "SDG", 938, 2, false, true, "Sudanese pound" };
		constexpr Currency SEK{ "SEK", 752, 2, false, true, "Swedish krona" };
		constexpr Currency SGD{ "SGD", 702, 2, false, true, "Singapore dollar" };
		constexpr Currency SHP{ "SHP", 654, 2, false, true, "Saint Helena pound" };
		constexpr Currency SLL{ "SLL", 694, 2, false, true, "Sierra Leonean leone" };
		constexpr Currency SOS{ "SOS", 706, 2, false, true, "Somali shilling" };
		constexpr Currency SRD{ "SRD", 968, 2, false, true, "Surinamese dollar" };
		constexpr Currency SSP{ "SSP", 728, 2, false, true, "South Sudanese pound" };
		constexpr Currency STN{ "STN", 930, 2, false, true, "Sao Tome and Principe dobra" };
		constexpr Currency SVC{ "SVC", 222, 2, false, false, "Salvadoran colon" };
		constexpr Currency SYP{ "SYP", 760, 2, false, true, "Syrian pound" };
		constexpr Currency SZL{ "SZL", 748, 2, false, true, "Swazi lilangeni" };
		constexpr Currency THB{ "THB", 764, 2, false, true, "Thai baht" };
		constexpr Currency TJS{ "TJS", 972, 2, false, true, "Tajikistani somoni" };
		constexpr Currency TMT{ "TMT", 934, 2, false, true, "Turkmenistan manat" };
		constexpr Currency TND{ "TND", 788, 3, false, true, "Tunisian dinar" };
		constexpr Currency TOP{ "TOP", 776, 2, false, true, "Tongan pa'anga" };
		constexpr Currency TRY{ "TRY", 949, 2, false, true, "Turkish lira" };
		constexpr Currency TTD{ "TTD", 780, 2, false, true, "Trinidad and Tobago dollar" };
		constexpr Currency TWD{ "TWD", 901, 2, true, true, "New Taiwan dollar" };
		constexpr Currency TZS{ "TZS", 834, 2, false, true, "Tanzanian shilling" };
		constexpr Currency UAH{ "UAH", 980, 2, false, true, "Ukrainian hryvnia" };
		constexpr Currency UGX{ "UGX", 800, 2, false, true, "Ugandan shilling" };
		constexpr Currency USD{ "USD", 840, 2, false, true, "United States dollar" };
		constexpr Currency USN{ "USN", 997, 2, false, false, "United States dollar (next day) " };
		constexpr Currency UYI{ "UYI", 940, 0, false, false, "Uruguay Peso en Unidades Indexadas " };
		constexpr Currency UYU{ "UYU", 858, 2, true, true, "Uruguayan peso" };
		constexpr Currency UYW{ "UYW", 927, 4, false, true, "Unidad previsional" };
		constexpr Currency UZS{ "UZS", 860, 2, false, true, "Uzbekistan som" };
		constexpr Currency VES{ "VES", 928, 2, true, true, "Venezuelan bolivar soberano" };
		constexpr Currency VND{ "VND", 704, 0, true, true, "Vietnamese dong" };
		constexpr Currency VUV{ "VUV", 548, 0, false, true, "Vanuatu vatu" };
		constexpr Currency WST{ "WST", 882, 2, false, true, "Samoan tala" };
		constexpr Currency XAF{ "XAF", 950, 0, false, true, "CFA franc BEAC" };
		constexpr Currency XAG{ "XAG", 961,-1, false, false, "Silver" };
		constexpr Currency XAU{ "XAU", 959,-1, false, false, "Gold" };
		constexpr Currency XBA{ "XBA", 955,-1, false, false, "European Composite Unit (EURCO)" };
		constexpr Currency XBB{ "XBB", 956,-1, false, false, "European Monetary Unit" };
		constexpr Currency XBC{ "XBC", 957,-1, false, false, "European Unit of Account 9" };
		constexpr Currency XBD{ "XBD", 958,-1, false, false, "European Unit of Account 17" };
		constexpr Currency XCD{ "XCD", 951, 2, false, true, "East Caribbean dollar" };
		constexpr Currency XDR{ "XDR", 960,-1, false, false, "Special drawing rights" };
		constexpr Currency XOF{ "XOF", 952, 0, false, true, "CFA franc BCEAO" };
		constexpr Currency XPD{ "XPD", 964,-1, false, false, "Palladium" };
		constexpr Currency XPF{ "XPF", 953, 0, false, true, "CFP franc" };
		constexpr Currency XPT{ "XPT", 962,-1, false, false, "Platinum" };
		constexpr Currency XSU{ "XSU", 994,-1, true, false, "SUCRE" };
		constexpr Currency XTS{ "XTS", 963,-1, true, false, "Reserved for testing" };
		constexpr Currency XUA{ "XUA", 965,-1, true, false, "ADB Unit of Account" };
		constexpr Currency XXX{ "XXX", 999,-1, true, false, "" };
		constexpr Currency YER{ "YER", 886, 2, false, true, "Yemeni rial" };
		constexpr Currency ZAR{ "ZAR", 710, 2, false, true, "South African rand" };
		constexpr Currency ZMW{ "ZMW", 967, 2, false, true, "Zambian kwacha" };
		constexpr Currency ZWL{ "ZWL", 932, 2, false, true, "Zimbabwean dollar" };

		// unofficial codes
		constexpr Currency CNH{ "CNH", -1, 2, false, false, "Chinese yuan" };
		constexpr Currency CNO{ "CNO", -2, 2, false, false, "Renminbi offshore" };

		// crypto-currencies
		constexpr Currency DASH{ "DASH", -100,  8, false, false, "Dash" };
		constexpr Currency ETH{ "ETH",  -101, 18, false, false, "Ether" };
		constexpr Currency VTC{ "VTC",  -102,  8, false, false, "Vertcoin" };
		constexpr Currency XBC_{ "XBC_", -103,  8, false, false, "Bitcoin Cash" };
		constexpr Currency XBT{ "XBT",  -104,  8, false, false, "Bitcoin" };
		constexpr Currency XLM{ "XLM",  -105,  8, false, false, "Stellar Lumen" };
		constexpr Currency XMR{ "XMR",  -106, 12, false, false, "Monero" };
		constexpr Currency XRP{ "XRP",  -107,  6, false, false, "Ripple" };
		constexpr Currency ZEC{ "ZEC",  -108,  8, false, false, "Zcash" };

		constexpr Currency NONE{ std::string_view{}, 0, 0, true, false , std::string_view{} };

		constexpr Currency BASE{ "USD", 840, 2, false, true, "United States dollar" };
		// ReSharper restore CppInconsistentNaming

		static const std::initializer_list<Currency> currencies
		{
			// official
			AED, AFN, ALL, AMD, ANG, AOA, ARS, AUD, AWG, AZN,
			BAM, BBD, BDT, BGN, BHD, BIF, BMD, BND, BOB, BOV, BRL, BSD, BTN, BWP, BYN, BZD,
			CAD, CDF, CHE, CHF, CHW, CLF, CLP, CNY, COP, COU, CRC, CUC, CUP, CVE, CZK,
			DJF, DKK, DOP, DZD,
			EGP, ERN, ETB, EUR,
			FJD, FKP,
			GBP, GEL, GHS, GIP, GMD, GNF, GTQ, GYD,
			HKD, HNL, HRK, HTG, HUF,
			IDR, ILS, INR, IQD, IRR, ISK,
			JMD, JOD, JPY,
			KES, KGS, KHR, KMF, KPW, KRW, KWD, KYD,
			LAK, LBP, LKR, LRD, LSL, LYD,
			MAD, MDL, MGA, MKD, MMK, MNT, MOP, MRU, MUR, MVR, MWK, MXN, MXV, MYR, MZN,
			NAD, NGN, NIO, NOK, NPR, NZD,
			OMR,
			PAB, PEN, PGK, PHP, PKR, PLN, PYG,
			QAR,
			RON, RSD, RUB, RWF,
			SAR, SBD, SCR, SDG, SEK, SGD, SHP, SLL, SOS, SRD, SSP, STN, SVC, SYP, SZL,
			THB, TJS, TMT, TND, TOP, TRY, TTD, TWD, TZS,
			UAH, UGX, USD, USN, UYI, UYU, UYW, UZS,
			VES, VND, VUV,
			WST,
			XAF, XAG, XAU, XBA, XBB, XBC, XBD, XCD, XDR, XOF, XPD, XPF, XPT, XSU, XTS, XUA, XXX,
			YER,
			ZAR, ZMW, ZWL,

			// unofficial
			CNH, CNO,
			// base
			BASE,
			// crypto-currencies
			DASH, ETH, VTC, XBC_, XBT, XLM, XMR, XRP, ZEC
		};
	}

	inline namespace formatting
	{
		template <>
		struct TPNIKOPOLIS_IMPEXP Formatter<TP::Currency>
		{
			[[nodiscard]] OptCurrency tryParse(const std::string_view& str) const;
			[[nodiscard]] std::string str(const Currency& value) const;
			[[nodiscard]] std::string failToParse(const std::string_view& str) const;
		};
	}


}
OSTREAM_FROM_TP(TP::Currency)
//DECLARE_FMT_FORMATTER_FROM_TP(TP::Currency)

FMT_BEGIN_NAMESPACE


FMT_END_NAMESPACE
