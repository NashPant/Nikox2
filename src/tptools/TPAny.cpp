#include "TPAny.h"
#include "TPException.h"

//We cont allow any silly operations on dates
//not even addition of two dates.
//strings can be added but that's about it.
//namespace NIKO {

	TPAny& TPAny::operator +=(const TPAny& rhs)
	{
		switch (type_)
		{
		case INT:
			int_ += int(rhs); break;
		case LONG:
			long_ += long(rhs); break;
		case DOUBLE:
			double_ += double(rhs); break;
		case STRING:
			string_ += std::string(rhs); break;
			//TODO: Shall we add the date in here?
		default:	THROW("TPAny::operator +=: can not action perator +=")
		}

		return *this;
	}

	TPAny& TPAny::operator -=(const TPAny& rhs)
	{
		switch (type_)
		{
		case INT:
			int_ -= int(rhs); break;
		case LONG:
			long_ -= long(rhs); break;
		case DOUBLE:
			double_ -= double(rhs); break;
		default:	THROW("TPAny& TPAny::operator -=: can not action operator +=")
		}

		return *this;
	}

	TPAny& TPAny::operator *=(const TPAny& rhs)
	{
		switch (type_)
		{
		case INT:
			int_ *= int(rhs); break;
		case LONG:
			long_ *= long(rhs); break;
		case DOUBLE:
			double_ *= double(rhs); break;
		default:	THROW("TPAny& TPAny::operator *=: can not action operator +=")
		}

		return *this;
	}

	TPAny& TPAny::operator /=(const TPAny& rhs)
	{
		switch (type_)
		{
		case INT:
			int_ /= int(rhs); break;
		case LONG:
			long_ /= long(rhs); break;
		case DOUBLE:
			double_ /= double(rhs); break;
		default:	THROW("TPAny& TPAny::operator /=: can not action operator +=")
		}

		return *this;
	}


	//Casting
	//to handle with care


	TPAny::operator bool()	const
	{

		switch (type_)
		{
		case BOOL:	return bool_;
		default:	THROW("TPAny::operator bool(): can not cast TPAny to bool")
		}
	}

	TPAny::operator int()	const
	{
		switch (type_)
		{
		case BOOL:		return int(bool_);
		case INT:		return int_;
		case LONG:		return int(long_);
		case DOUBLE:	return int(double_);
		case STRING:	return 0;
		case DATE:		return date_.getExcelDate();
		default:		THROW("TPAny::operator int(): can not cast TPAny to int")
		}

	}

	TPAny::operator long()	const
	{
		switch (type_)
		{
		case BOOL:		return long(bool_);
		case INT:		return long(int_);
		case LONG:		return long_;
		case DOUBLE:	return long(double_);
		case STRING:	return 0;
		case DATE:		return date_.getExcelDate();
		default:		THROW("TPAny::operator long(): can not cast TPAny to long")
		}

	}

	TPAny::operator double()	const
	{
		switch (type_)
		{
		case BOOL:		return double(bool_);
		case INT:		return double(int_);
		case LONG:		return double(long_);
		case DOUBLE:	return double_;
		default:		return 0.0;
		}

	}

	TPAny::operator std::string()	const
	{
		if (type_ == STRING) return string_;
		else return "";

	}

	TPAny::operator TPDate()	const
	{
		switch (type_)
		{
		case INT:		return TPDate(int_);
		case LONG:		return TPDate(long_);
		case DOUBLE:	return TPDate(double_);
		case DATE:		return date_;
		case STRING:
		{
			// Correct string..
//			string mystring = string_.substr(
			return TPDate(string_);
		}
		default:		THROW("TPAny::operator TPDate(): can not cast TPAny to TPDate")
		}

	}


	bool TPAny::operator ==(const TPAny& rhs) const
	{

		if (type_ != rhs.type_) return false;

		switch (type_)
		{
		case EMPTY:		return true;
		case BOOL:		return bool_ == rhs.bool_;
		case INT:		return int_ == rhs.int_;
		case LONG:		return long_ == rhs.long_;
		case DOUBLE:	return double_ == rhs.double_;
		case STRING:	return string_ == rhs.string_;
		case DATE:		return date_ == rhs.date_;
		case INVALID:
		default:	THROW("TPAny::operator ==: can not compare undefined types")
		}

	}

	bool TPAny::operator !=(const TPAny& rhs) const
	{
		return !(*this == rhs);
	}

	void	TPAny::setEmpty()
	{
		bool_ = false;
		int_ = 0;
		long_ = 0L;
		double_ = 0.0;
		string_ = "";
		date_ = TPDate::getInvalidDate();
		type_ = EMPTY;

	}

	bool TPAny::operator <(const TPAny& rhs) const
	{

		if (type_ != rhs.type_) return (type_ < rhs.type_);

		switch (type_)
		{
		case BOOL:	return bool_ < rhs.bool_;
		case INT:		return int_ < rhs.int_;
		case LONG:		return long_ < rhs.long_;
		case DOUBLE:	return double_ < rhs.double_;
		case STRING:	return string_ < rhs.string_;
		case DATE:		return date_ < rhs.date_;
		case EMPTY:		return true;
		default:	THROW("TPAny::operator <: can no compare Anys of different types")
		}
	}

	bool TPAny::operator >(const TPAny& rhs) const
	{

		return !((*this) < rhs) && !((*this) == rhs);
	}


	std::variant<bool, int, long, double, std::string, TPDate> TPAny::value() const
	{
		switch (type_)
		{
		case BOOL:	return std::variant<bool, int, long, double, std::string, TPDate>(this->operator bool());
		case INT:		return std::variant<bool, int, long, double, std::string, TPDate>(this->operator int());
		case LONG:		return std::variant<bool, int, long, double, std::string, TPDate>(this->operator long());
		case DOUBLE:	return std::variant<bool, int, long, double, std::string, TPDate>(this->operator double());
		case STRING:	return std::variant<bool, int, long, double, std::string, TPDate>(this->operator std::string());
		case DATE:		return std::variant<bool, int, long, double, std::string, TPDate>(this->operator TPDate());
		case EMPTY:		return std::variant<bool, int, long, double, std::string, TPDate>();
		default:	THROW("TPAny::operator <: can no compare Anys of different types")
		}
	}