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

#ifndef TP_VARIANT
#define TP_VARIANT

#include <TPNikopolisUtils.h>
#include <TPNikopolisRoot.h>
#include "TPGlobals.h"
#include <TPTools/Time/TPDate.h>
#include <string>
#include "TPException.h"
#include "Time/TPHolidays.h"
#include <variant>
//"corba style" Any class that may contain
//the following types:
//empty, bool, int, long, double, string or date.
//
//Useful when sending data via corba bridge and
//manipulating Excel Data.
//namespace NIKO {

class TPNIKOPOLIS_IMPEXP TPAny : public TPNikopolisRoot
	{
	public:
		enum AnyType
		{
			EMPTY,
			BOOL,
			INT,
			LONG,
			DOUBLE,
			STRING,
			DATE,
			INVALID
		};

		//default ctors
		//and dtor
		TPAny();
		TPAny(const TPAny& rhs);
		const TPAny& operator=(const TPAny& rhs);
		const TPAny& operator=(int rhs);
		const TPAny& operator=(long rhs);
		const TPAny& operator=(const double& rhs);
		const TPAny& operator=(const char *rhs);
		const TPAny& operator=(const std::string& rhs);
		const TPAny& operator=(const TPDate& rhs);
		~TPAny() {}



		//self referencing
		//
		TPAny&	operator+=(const TPAny& rhs);
		TPAny&	operator-=(const TPAny& rhs);
		TPAny&	operator*=(const TPAny& rhs);
		TPAny&	operator/=(const TPAny& rhs);

		//non self referencing
		TPAny	operator+(const TPAny& rhs)const;
		TPAny	operator-(const TPAny& rhs)const;
		TPAny	operator*(const TPAny& rhs)const;
		TPAny	operator/(const TPAny& rhs)const;


		//boolean comaprison

		//returns false if different types.
		bool operator==(const TPAny& rhs) const;
		bool operator!=(const TPAny& rhs) const;

		//return ordinal order if types are different
		//example: (DOUBLE > BOOL) is a true statement.
		bool operator>(const TPAny& rhs) const;
		bool operator<(const TPAny& rhs) const;

		//No default conversion between types
		//hence the use of explicit. i.e. any
		//type conversion needs to be explicity stated.

		explicit TPAny(bool rhs);
		explicit TPAny(int rhs);
		explicit TPAny(long rhs);
		explicit TPAny(const double& rhs);
		explicit TPAny(const std::string& rhs);
		explicit TPAny(const TPDate& rhs);
		explicit TPAny(const char* rhs);

		bool isEmpty()		const { return type_ == INVALID; }
		bool isBool()		const { return type_ == BOOL; }
		bool isInteger() 	const { return type_ == INT; }
		bool isLong()		const { return type_ == LONG; }
		bool isDouble()		const { return type_ == DOUBLE; }
		bool isString()		const { return type_ == STRING; }
		bool isDate()		const { return type_ == DATE; }
		bool isInvalid()	const { return type_ == INVALID; }

		//
		//explicit casts.

		bool	toBool()	const { return operator bool(); }
		int		toInt()		const { return operator int(); }
		long	toLong()	const { return operator long(); }
		double 	toDouble()	const { return operator double(); }
		std::string	toString()	const { return operator std::string(); }
		TPDate	toDate()	const { return operator TPDate(); }



		//If you want to safe-cast a TPAny, get its type first
		//then use one of the above toType() functions.

		//returns only in case of bool
		//throw otherwise
		operator bool()		const;

		//converts any type to int;
		operator int()		const;
		//idem
		operator long()		const;

		//will only return if the type is numerical
		//throws otherwise.
		operator double()	const;

		//return string_ if type_ is string otherwise 
		//return "". Dunno, we might need to change this.

		operator std::string()	const;

		//returns a date in case of a numerical or date
		//throw otherwise. 

		operator TPDate()	const;

		//Misc
		//
		void	setEmpty();
		AnyType	getType()	const { return type_; }
		friend std::string ToString(const TPAny& rhs);

		std::variant<bool, int, long, double, std::string, TPDate> value() const;
	private:

		//class members

		bool	bool_;
		int		int_;
		long	long_;
		double	double_;
		std::string	string_;
		TPDate	date_;

		AnyType	type_;



	};

	inline TPAny::TPAny()

	{
		setEmpty();
		type_ = INVALID;
	}

	//easy stuff

	inline TPAny	TPAny::operator+(const TPAny& rhs)const
	{
		return TPAny(*this) += rhs;
	}

	inline TPAny	TPAny::operator-(const TPAny& rhs)const
	{
		return TPAny(*this) -= rhs;
	}

	inline TPAny	TPAny::operator*(const TPAny& rhs)const
	{
		return TPAny(*this) *= rhs;
	}

	inline TPAny	TPAny::operator/(const TPAny& rhs)const
	{
		return TPAny(*this) /= rhs;
	}

	//constructors

	inline TPAny::TPAny(bool rhs)
		:bool_(rhs),
		int_(0),
		long_(0L),
		double_(0.0),
		string_(),
		date_(TPDate::getInvalidDate()),
		type_(BOOL)
	{
	}

	inline TPAny::TPAny(int rhs)
		:bool_(false),
		int_(rhs),
		long_(0L),
		double_(0.0),
		string_(),
		date_(TPDate::getInvalidDate()),
		type_(INT)
	{
	}

	inline TPAny::TPAny(long rhs)
		:bool_(false),
		int_(0),
		long_(rhs),
		double_(0.0),
		string_(),
		date_(TPDate::getInvalidDate()),
		type_(LONG)
	{
	}

	inline TPAny::TPAny(const double& rhs)
		:bool_(false),
		int_(0),
		long_(0L),
		double_(rhs),
		string_(),
		date_(TPDate::getInvalidDate()),
		type_(DOUBLE)
	{
	}

	inline TPAny::TPAny(const std::string& rhs)
		:bool_(false),
		int_(0),
		long_(0L),
		double_(0.0),
		string_(rhs),
		date_(TPDate::getInvalidDate()),
		type_(STRING)
	{
	}

	inline TPAny::TPAny(const char* rhs)
		:bool_(false),
		int_(0),
		long_(0L),
		double_(0.0),
		string_(rhs),
		date_(TPDate::getInvalidDate()),
		type_(STRING)
	{
	}

	inline TPAny::TPAny(const TPDate& rhs)
		:bool_(false),
		int_(0),
		long_(0L),
		double_(0.0),
		string_(),
		date_(rhs),
		type_(DATE)
	{
	}

	inline	const TPAny& TPAny::operator=(int rhs)
	{
		int_ = rhs;
		type_ = INT;
		return *this;
	}

	inline	const TPAny& TPAny::operator=(long rhs)
	{
		long_ = rhs;
		type_ = LONG;
		return *this;
	}
	inline	const TPAny& TPAny::operator=(const double& rhs)
	{
		double_ = rhs;
		type_ = DOUBLE;
		return *this;
	}
	inline	const TPAny& TPAny::operator=(const char *rhs)
	{
		string_ = std::string(rhs);
		type_ = STRING;
		return *this;
	}
	inline	const TPAny& TPAny::operator=(const std::string& rhs)
	{
		string_ = rhs;
		type_ = STRING;
		return *this;
	}
	inline	const TPAny& TPAny::operator=(const TPDate& rhs)
	{
		date_ = rhs;
		type_ = DATE;
		return *this;
	}


	//copy and assignment

	inline const TPAny& TPAny::operator =(const TPAny& rhs)
	{
		if (this != &rhs)
		{
			bool_ = rhs.bool_;
			int_ = rhs.int_;
			long_ = rhs.long_;
			double_ = rhs.double_;
			string_ = rhs.string_;
			date_ = rhs.date_;
			type_ = rhs.type_;
		}

		return *this;
	}

	inline TPAny::TPAny(const TPAny& rhs)
	{

		bool_ = rhs.bool_;
		int_ = rhs.int_;
		long_ = rhs.long_;
		double_ = rhs.double_;
		string_ = rhs.string_;
		date_ = rhs.date_;
		type_ = rhs.type_;


	}

	inline std::string ToString(const TPAny& rhs)
	{
		switch (rhs.getType())
		{
		case TPAny::EMPTY:		return ""; break;
		case TPAny::BOOL:		return ToString(rhs.bool_); break;
		case TPAny::INT:		return ToString(rhs.int_); break;
		case TPAny::LONG:		return ToString(rhs.long_); break;
		case TPAny::DOUBLE:		return ToString(rhs.double_); break;
		case TPAny::STRING:		return rhs.string_; break;
		case TPAny::DATE:		return ToString(rhs.date_); break;
		case TPAny::INVALID:	return "Invalid TPAny"; break;
			//we should neva get to this stage..
		default: THROW("can not convert TPAny to string")
		}
	}

	inline double AnyPeriodToDouble(const TPAny& 	expiry_any, const TPDate& asOfDate)
	{
		double res;
		switch (expiry_any.getType())
		{
			// If string, then a period is given
		case TPAny::STRING:
		{
			TPPeriod expiry_period(ToString(expiry_any));
			TPDate Expirydate = asOfDate; Expirydate += expiry_period;
			if (TPHolidays::hasHolidays())
				Expirydate.adjustBusinessDays(MODIFIEDFOLLOWING, TP_LON);
			res = (Expirydate.getExcelDate() - asOfDate.getExcelDate()) / 365.;
			break;
		}
		case TPAny::DOUBLE:
		case TPAny::LONG:
		{
			double tau = expiry_any.toDouble();
			if (tau > 500.) // in this case the supplied number is a date, have to convert it to year fraction
			{
				res = (tau - asOfDate.getExcelDate()) / 365.;
			}
			else
				res = expiry_any;
		}
		break;
		default:
			THROW("AnyExpiryTodouble: Invalid Expiry Type ")


		}

		return res;
	}
#endif