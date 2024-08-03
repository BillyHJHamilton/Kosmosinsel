#include "Quality.h"
#include <cstdint>
#include "Utility.h"

static SQualityStats s_QualityStats[(int32_t)EQuality::Count] =
{	//				  Morale	Rally
	/* Standard */	{ 0.8f,		0.15f },
	/* Elite */		{ 1.0f,		0.20f }
};

const char* s_QualityName [(int32_t)EQuality::Count] =
{
	"standard",
	"elite"
};

EQuality StrToQuality(std::string str)
{
	LowercaseInPlace(str);
	for (int32_t i = 0; i < (int32_t)EQuality::Count; ++i)
	{
		if (str == s_QualityName[i])
		{
			return (EQuality)i;
		}
	}
	return EQuality::None;
}

std::string QualityToStr(EQuality quality)
{
	if (quality < EQuality::Count)
	{
		return s_QualityName[(int32_t)quality];
	}
	return "none";
}

float GetStartingMorale(EQuality quality)
{
	return s_QualityStats[(int32_t)quality].m_StartingMorale;
}

float GetRallyChance(EQuality quality)
{
	return s_QualityStats[(int32_t)quality].m_RallyChance;
}
