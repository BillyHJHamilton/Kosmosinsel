#pragma once

#include "Common.h"
#include <string>

enum class EUnitType : uint8_t
{
	Battleships,
	Motherships,
	Destroyers,

	Count,
	None
};

EUnitType StrToUnitType(std::string str);
std::string UnitTypeToStr(EUnitType type);

static constexpr Flag f_Carrier	 = 1 << 0;
static constexpr Flag f_AirDefence = 1 << 1;

struct SUnitTypeStats
{
	float m_Speed;
	float m_Fire;	// Damage caused to enemy unit when firing at full morale.
	float m_Battle;	// Relative strength of this unit type in close battle.
	Flag m_Special;

	static SUnitTypeStats s_Stats [(int32_t)EUnitType::Count];
	static SUnitTypeStats Get(EUnitType unitType);
};
