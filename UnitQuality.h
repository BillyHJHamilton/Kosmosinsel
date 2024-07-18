#pragma once

#include <string>

enum class EUnitQuality : uint8_t
{
	Standard,
	Elite,

	Count,
	None = Count
};

EUnitQuality StrToQuality(std::string str);
std::string QualityToStr(EUnitQuality quality);

float GetStartingMorale(EUnitQuality quality);
