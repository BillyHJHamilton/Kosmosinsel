#pragma once

#include "Common.h"
#include <string>

enum class EUnitType : uint8_t;
enum class EUnitQuality : uint8_t;
enum class EFormation : uint8_t;

static constexpr Flag f_Disrupted		= 1 << 0;
static constexpr Flag f_ReceivedFire	= 1 << 1;
static constexpr Flag f_Acted			= 1 << 2;
static constexpr Flag f_Flagship		= 1 << 3;
static constexpr Flag f_FlagshipDamaged	= 1 << 4;

struct SUnit
{
	int32_t m_Id;
	int32_t m_Team;
	std::string m_Commander;

	EUnitType m_Type;
	EUnitQuality m_Quality;
	EFormation m_Formation;

	float m_Strength = 1.0f; // Percent of ships still capable of battle.
	float m_Morale;

	Flag m_StatusFlags = f_None;
};

std::string GetUnitStatusString(const SUnit& unit);
