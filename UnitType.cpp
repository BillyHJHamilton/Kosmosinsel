#include "UnitType.h"
#include "Utility.h"

SUnitTypeStats SUnitTypeStats::s_Stats [(int32_t)EUnitType::Count] =
{					   // Spd   Fire  Bttl  Ftrs
	/* Battleships */	{ 1.0f, 1.0f, 1.0f, 1.0f },
	/* Motherships */	{ 1.0f, 0.5f, 0.3f, 2.0f },
	/* Destroyers*/		{ 1.5f, 0.7f, 0.5f, 0.0f }
};

const char* s_UnitTypeName [(int32_t)EUnitType::Count] =
{
	"battleships",
	"motherships",
	"destroyers"
};

EUnitType StrToUnitType(std::string str)
{
	LowercaseInPlace(str);
	for (int32_t i = 0; i < (int32_t)EUnitType::Count; ++i)
	{
		if (str == s_UnitTypeName[i])
		{
			return (EUnitType)i;
		}
	}
	return EUnitType::None;
}

std::string UnitTypeToStr(EUnitType type)
{
	if (type < EUnitType::Count)
	{
		return s_UnitTypeName[(int32_t)type];
	}
	return "none";
}

SUnitTypeStats SUnitTypeStats::Get(EUnitType unitType)
{
	return SUnitTypeStats::s_Stats[(int32_t)unitType];
}
