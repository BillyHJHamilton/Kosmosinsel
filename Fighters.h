#pragma once

#include "Common.h"
#include <string>
#include <vector>

class CGame;
struct SDestroyEvents;
struct SUnit;

enum class EFighterState : uint8_t
{
	Docked,
	Launching,
	Striking,
	Defending,
	Recalling,
	
	Count,
	None = Count
};

EFighterState StrToFighterState(std::string str);
std::string FighterStateToStr(EFighterState state);

bool HasSurvivingFighters(const SUnit& unit);
float GetCurrentFighterPower(const SUnit& unit);
float GetCombinedFighterPower(const std::vector<SUnit*>& units);

std::vector<SUnit*> GetUnitsDefendingUnit(CGame& game, const SUnit& defendedUnit);
std::vector<SUnit*> GetAllDefendingUnits(CGame& game, const std::vector<SUnit*>& defendedUnits);
std::vector<SUnit*> GetUnitsStrikingUnit(CGame& game, const SUnit& targetedUnit, int32_t strikingTeamId);
std::vector<SUnit*> GetAllStrikingUnits(CGame& game, const std::vector<SUnit*>& targetedUnits, int32_t strikingTeamId);

void ApplyFighterAttrition(SUnit& unit);

void ApplyStrike(CGame& game, std::vector<SUnit*> attackers, SUnit& target, SDestroyEvents& outDestroyEvents);

void ApplyDogfightResults(
	const std::vector<SUnit*>& winners, const std::vector<SUnit*>& losers,
	const float totalWinnersStrength, const float totalLosersStrength,
	const float modifiedWinStrength, const float modifiedLoseStrength,
	SDestroyEvents& outDestroyEvents);

void ApplyFighterDamage(SUnit& unit, float fighterDamage);

