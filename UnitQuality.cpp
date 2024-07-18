#include "UnitQuality.h"
#include <cstdint>
#include "Utility.h"

static float s_StartingMorale[(int32_t)EUnitQuality::Count] =
{
	/* Standard */ 0.8f,
	/* Elite */	1.0f
};

const char* s_UnitQualityName [(int32_t)EUnitQuality::Count] =
{
	"standard",
	"elite"
};

EUnitQuality StrToQuality(std::string str)
{
	LowercaseInPlace(str);
	for (int32_t i = 0; i < (int32_t)EUnitQuality::Count; ++i)
	{
		if (str == s_UnitQualityName[i])
		{
			return (EUnitQuality)i;
		}
	}
	return EUnitQuality::None;
}

std::string QualityToStr(EUnitQuality quality)
{
	if (quality < EUnitQuality::Count)
	{
		return s_UnitQualityName[(int32_t)quality];
	}
	return "none";
}

float GetStartingMorale(EUnitQuality quality)
{
	return s_StartingMorale[(int32_t)quality];
}
