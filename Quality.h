#pragma once

#include <string>

enum class EQuality : uint8_t
{
	Standard,
	Elite,

	Count,
	None = Count
};

EQuality StrToQuality(std::string str);
std::string QualityToStr(EQuality quality);

struct SQualityStats
{
	float m_StartingMorale;
	float m_RallyChance;
};

float GetStartingMorale(EQuality quality);
float GetRallyChance(EQuality quality);
