#pragma once

#include "Common.h"
#include <string>

enum class EUnitType : uint8_t;
enum class EQuality : uint8_t;
enum class EFormation : uint8_t;
enum class EFighterState : uint8_t;

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
	EQuality m_Quality;
	EFormation m_Formation;
	EFighterState m_FighterState;

	int32_t m_FighterTarget = c_IndexNone; // Id of ship fighters are striking or defending.

	float m_Strength = 1.0f; // Percent of ships still capable of battle.
	float m_Morale;
	float m_FighterStrength = 1.0f;

	Flag m_StatusFlags = f_None;
};

std::string GetUnitStatusString(const SUnit& unit);
