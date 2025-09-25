#pragma once
#include "TPTools/Utils/TPEnum.h"
#include "TPTools/TPException.h"
#include "TPTools/Time/TPDate.h"
#include <map>

namespace TP::date_time
{

	//enum class BusinessDayConvention
	//{
	//	None = 0,
	//	F, // Following
	//	P, // Preceding
	//	MF, // Modified Following
	//	MP  // Modified Preceding
	//};

	TP_ENUM_CREATE(BusinessDayConvention, None, F, P, MF, MP)
		TP_ENUM_VALUE(None, "None")
		TP_ENUM_VALUE(F, "F")
		TP_ENUM_VALUE(F, "Following")
		TP_ENUM_VALUE(P, "P")
		TP_ENUM_VALUE(P, "Proceeding")
		TP_ENUM_VALUE(MF, "MF")
		TP_ENUM_VALUE(MF, "ModifiedFollowing")
		TP_ENUM_VALUE(MP, "MP")
		TP_ENUM_VALUE(MP, "ModifiedProceeding")
		TP_ENUM_CREATE_END(BusinessDayConvention)


	//using ROLL_CONVENTION = BusinessDayConvention;


	//inline ROLL_CONVENTION getOpposite(const ROLL_CONVENTION& c)
	//{
	//	switch (c)
	//	{
	//	case ROLL_CONVENTION::None: return ROLL_CONVENTION::None;
	//	case ROLL_CONVENTION::F: return ROLL_CONVENTION::P;
	//	case ROLL_CONVENTION::MF: return ROLL_CONVENTION::MP;
	//	case ROLL_CONVENTION::P: return ROLL_CONVENTION::F;
	//	case ROLL_CONVENTION::MP: return ROLL_CONVENTION::MF;
	//	//default: THROW("Unrecognized ROLL_CONVENTION [" + c.toString() + "].");
	//	default: THROW("Unrecognized ROLL_CONVENTION [].");
	//	}
	//}

	//inline static std::map<BusinessDayConvention, TPRolling> BussDayConvToRolling = 
	//{
	//	{+BusinessDayConvention::None, NOROLLING},
	//	{+BusinessDayConvention::F, FOLLOWING},
	//	{+BusinessDayConvention::MF, MODIFIEDFOLLOWING},
	//	{+BusinessDayConvention::P, PREVIOUS},
	//	{+BusinessDayConvention::MP, MODIFIEDPREVIOUS}
	//};


	//inline static std::map<TPRolling, BusinessDayConvention> RollingToBussDayConv =
	//{
	//	{ NOROLLING ,+BusinessDayConvention::None},
	//	{ FOLLOWING ,+BusinessDayConvention::F},
	//	{ MODIFIEDFOLLOWING ,+BusinessDayConvention::MF},
	//	{ PREVIOUS ,+BusinessDayConvention::P},
	//	{MODIFIEDPREVIOUS , +BusinessDayConvention::MP}
	//};
}