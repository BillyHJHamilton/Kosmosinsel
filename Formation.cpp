#include "Formation.h"
#include "Utility.h"

SFormationStats SFormationStats::s_Stats [(int32_t)EFormation::Count] =
{				   // Spd   Fire  Rcvd  Atk   Def
	/* Line */		{ 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
	/* Column */	{ 1.5f, 0.5f, 2.0f, 1.5f, 1.2f },
	/* Sphere */	{ 0.0f, 0.3f, 1.2f, 0.0f, 1.5f },
	/* Skirmish */	{ 1.5f, 0.8f, 0.9f, 0.8f, 0.5f },
	/* Rout */		{ 1.5f, 0.0f, 0.9f, 0.0f, 0.0f }
};

const char* s_FormationName [(int32_t)EFormation::Count] =
{
	"line",
	"column",
	"sphere",
	"skirmish",
	"rout"
};

SFormationStats SFormationStats::Get(EFormation formation)
{
	return SFormationStats::s_Stats[(int32_t)formation];
}

EFormation StrToFormation(std::string str)
{
	LowercaseInPlace(str);
	for (int32_t i = 0; i < (int32_t)EFormation::Count; ++i)
	{
		if (str == s_FormationName[i])
		{
			return (EFormation)i;
		}
	}
	return EFormation::None;
}

std::string FormationToStr(EFormation formation)
{
	if (formation < EFormation::Count)
	{
		return s_FormationName[(int32_t)formation];
	}
	return "none";
}
