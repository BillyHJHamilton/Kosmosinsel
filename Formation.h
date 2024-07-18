#pragma once

#include <cstdint>
#include <string>

enum class EFormation : uint8_t
{
	Line,
	Column,
	Sphere,
	Skirmish,
	Rout,

	Count,
	None = Count
};

EFormation StrToFormation(std::string str);
std::string FormationToStr(EFormation formation);

struct SFormationStats
{
	float m_SpeedFactor;
	float m_FireFactor;			// Damage dealt by the formation when firing.
	float m_ReceivedFactor;		// Damage received by the formation from enemy fire.
	float m_AttackFactor;
	float m_DefenceFactor;

	static SFormationStats s_Stats [(int32_t)EFormation::Count];
	static SFormationStats Get(EFormation formation);
};
