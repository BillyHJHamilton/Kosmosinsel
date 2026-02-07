#include "Fighters.h"

#include "Damage.h"
#include "Formation.h"
#include "GameConstants.h"
#include "Game.h"
#include "Output.h"
#include "Random.h"
#include "Unit.h"
#include "UnitType.h"
#include "Utility.h"
#include <algorithm>
#include <cassert>
#include <iomanip>
#include <iostream>

const char* s_FighterStateName [(int32_t)EFighterState::Count] =
{
	"docked",
	"launching",
	"striking",
	"defending",
	"recalling"
};

EFighterState StrToFighterState(std::string str)
{
	LowercaseInPlace(str);
	for (int32_t i = 0; i < (int32_t)EFighterState::Count; ++i)
	{
		if (str == s_FighterStateName[i])
		{
			return (EFighterState)i;
		}
	}
	return EFighterState::None;
}

std::string FighterStateToStr(EFighterState state)
{
	if (state < EFighterState::Count)
	{
		return s_FighterStateName[(int32_t)state];
	}
	return "none";
}

bool HasSurvivingFighters(const SUnit& unit)
{
	return GetCurrentFighterPower(unit) > c_Epsilon;
}

float GetCurrentFighterPower(const SUnit& unit)
{
	return unit.m_FighterStrength * SUnitTypeStats::Get(unit.m_Type).m_Fighters;
}

float GetCombinedFighterPower(const std::vector<SUnit*>& units)
{
	float fighterPower = 0.0f;
	for (SUnit* unit : units)
	{
		fighterPower += GetCurrentFighterPower(*unit);
	}
	return fighterPower;
}

std::vector<SUnit*> GetUnitsDefendingUnit(CGame& game, const SUnit& defendedUnit)
{
	std::vector<SUnit*> output;
	STeam* team = game.FindTeam(defendedUnit.m_Team);
	if (team)
	{
		for (SCommandGroup& g : team->m_CommandGroups)
		{
			for (SUnit& u : g.m_Units)
			{
				if (u.m_FighterState == EFighterState::Defending
					&& HasSurvivingFighters(u)
					&& u.m_FighterTarget == defendedUnit.m_Id)
				{
					output.push_back(&u);
				}
			}
		}
	}
	return output;
}

std::vector<SUnit*> GetAllDefendingUnits(CGame& game, const std::vector<SUnit*>& defendedUnits)
{
	std::vector<SUnit*> output;
	for (SUnit* unit : defendedUnits)
	{
		std::vector<SUnit*> defenders = GetUnitsDefendingUnit(game, *unit);
		output.insert(output.end(), defenders.begin(), defenders.end());
	}
	return output;
}

std::vector<SUnit*> GetUnitsStrikingUnit(CGame& game, const SUnit& targetedUnit, int32_t strikingTeamId)
{
	std::vector<SUnit*> output;
	STeam* team = game.FindTeam(strikingTeamId);
	if (team)
	{
		for (SCommandGroup& g : team->m_CommandGroups)
		{
			for (SUnit& u : g.m_Units)
			{
				if (u.m_FighterState == EFighterState::Striking
					&& HasSurvivingFighters(u)
					&& u.m_FighterTarget == targetedUnit.m_Id)
				{
					output.push_back(&u);
				}
			}
		}
	}
	return output;
}

std::vector<SUnit*> GetAllStrikingUnits(CGame& game, const std::vector<SUnit*>& targetedUnits, int32_t strikingTeamId)
{
	std::vector<SUnit*> output;
	for (SUnit* unit : targetedUnits)
	{
		std::vector<SUnit*> strikers = GetUnitsStrikingUnit(game, *unit, strikingTeamId);
		output.insert(output.end(), strikers.begin(), strikers.end());
	}
	return output;
}

void ApplyFighterAttrition(SUnit& unit)
{
	if (HasSurvivingFighters(unit) &&
		(unit.m_FighterState == EFighterState::Defending || 
		 unit.m_FighterState == EFighterState::Striking))
	{
		const float attrition = Random(0.5f, 1.5f) * c_BaseFighterAttrition;
		ApplyFighterDamage(unit, attrition);
	}
}

void ApplyStrike(CGame& game, std::vector<SUnit*> attackers, SUnit& target, SDestroyEvents& outDestroyEvents)
{
	float attackerFighters = GetCombinedFighterPower(attackers);
	assert(attackerFighters > c_Epsilon);

	std::vector<SUnit*> defenders = GetUnitsDefendingUnit(game, target);
	float defenderFighters = GetCombinedFighterPower(defenders);

	// Dogfight
	float baseStrikeDamage = c_BaseStrikeDamageIfNoDogfight;
	float baseStrikeMorale = c_BaseStrikeMoraleIfNoDogfight;
	if (defenderFighters > c_Epsilon)
	{
		const float modifiedAtt = attackerFighters * Random(0.5f, 1.5f);
		const float modifiedDef = defenderFighters * Random(0.5f, 1.5f) * c_DogfightDefenderBonus;

		std::cout << std::fixed << std::setprecision(2);
		if (modifiedAtt > modifiedDef)
		{
			// Attackers win dogfight.
			std::cout << "Attackers win dogfight - Final rolls: " << modifiedAtt << " vs " << modifiedDef << std::endl;
			ApplyDogfightResults(attackers, defenders, attackerFighters, defenderFighters, modifiedAtt, modifiedDef, outDestroyEvents);

			baseStrikeDamage = c_BaseStrikeDamageIfWonDogfight;
			baseStrikeMorale = c_BaseStrikeMoraleIfWonDogfight;
		}
		else
		{
			// Defenders win dogfight.
			std::cout << "Defenders win dogfight - Final rolls: " << modifiedAtt << " vs " << modifiedDef << std::endl;
			ApplyDogfightResults(defenders, attackers, defenderFighters, attackerFighters, modifiedDef, modifiedAtt, outDestroyEvents);

			baseStrikeDamage = c_BaseStrikeDamageIfLostDogfight;
			baseStrikeMorale = c_BaseStrikeMoraleIfLostDogfight;
		}

		// Update remaining attack strength.
		attackerFighters = GetCombinedFighterPower(attackers);
	}

	if (attackerFighters < c_Epsilon)
	{
		std::cout << "Attacking fighters were wiped out in dogfight." << std::endl;
		return;
	}

	// Defensive fire
	const float defensiveFire = c_BaseDefensiveFireDamage
		* target.m_Morale * target.m_Strength
		* SUnitTypeStats::Get(target.m_Type).m_Battle
		* SFormationStats::Get(target.m_Formation).m_DefenceFactor;

	{
		CDarkGreyScope grey;
		std::cout << std::fixed << std::setprecision(2);
		std::cout << "Defensive fire: " << defensiveFire << std::endl;
	}
	for (SUnit* attacker : attackers)
	{
		const float shareOfLosses = GetCurrentFighterPower(*attacker) / attackerFighters;
		const float damage = defensiveFire * shareOfLosses * Random(0.5f, 1.5f);
		ApplyFighterDamage(*attacker, damage);
	}

	// Update remaining attack strength.
	attackerFighters = GetCombinedFighterPower(attackers);
	if (attackerFighters < c_Epsilon)
	{
		std::cout << "Attacking fighters were wiped out by defensive fire." << std::endl;
		return;
	}

	// Strike damage
	const float strikeDamage = baseStrikeDamage
		* attackerFighters * Random(0.5f, 1.5f);
	const float moraleDamage = baseStrikeMorale
		* attackerFighters * Random(0.5f, 1.5f);
	const bool testDisruption = strikeDamage > 0.1f;
	ApplyDamage(target, strikeDamage, moraleDamage, testDisruption, outDestroyEvents);
}

void ApplyDogfightResults(
	const std::vector<SUnit*>& winners, const std::vector<SUnit*>& losers,
	const float totalWinnersStrength, const float totalLosersStrength,
	const float modifiedWinStrength, const float modifiedLoseStrength,
	SDestroyEvents& outDestroyEvents)
{
	for (SUnit* loserUnit : losers)
	{
		const float shareOfLosses = GetCurrentFighterPower(*loserUnit) / totalLosersStrength;
		const float damage = c_BaseDogfightDamageForLosers * modifiedWinStrength * shareOfLosses * Random(0.5f, 1.5f);
		ApplyFighterDamage(*loserUnit, damage);
	}

	for (SUnit* winnerUnit : winners)
	{
		const float shareOfLosses = GetCurrentFighterPower(*winnerUnit) / totalWinnersStrength;
		const float damage = c_BaseDogfightDamageForWinners * modifiedLoseStrength * shareOfLosses * Random(0.5f, 1.5f);
		ApplyFighterDamage(*winnerUnit, damage);
	}
}

void ApplyFighterDamage(SUnit& unit, float fighterDamage)
{
	unit.m_FighterStrength = std::max(0.0f, unit.m_FighterStrength - fighterDamage);

	{
		CDarkGreyScope grey;
		std::cout << std::fixed << std::setprecision(0)
			<< unit.m_Id << " lost "
			<< 100.0f*fighterDamage << "% fighters (now "
			<< 100.0f*unit.m_FighterStrength << "%)."
			<< std::endl;
	}
	if (unit.m_FighterStrength < c_Epsilon)
	{
		CRedScope red;
		std::cout << "Unit " << unit.m_Id << " has lost all fighters!" << std::endl;
	}
}
