#include "Attack.h"
#include "Damage.h"
#include "Fighters.h"
#include "Fire.h"
#include "Formation.h"
#include "GameConstants.h"
#include "Morale.h"
#include "Random.h"
#include "Unit.h"
#include "UnitType.h"
#include <algorithm>
#include <cassert>
#include <iomanip>
#include <iostream>

void ApplyAttack(
	const std::vector<SUnit*>& attackers,
	const std::vector<Flag>& attackerFlags,
	const std::vector<SUnit*>& attackerFighters,
	const std::vector<SUnit*>& defenders,
	const std::vector<Flag>& defenderFlags,
	const std::vector<SUnit*>& defenderFighters,
	SDestroyEvents& outDestroyEvents)
{
	assert(attackers.size() == attackerFlags.size());
	assert(defenders.size() == defenderFlags.size());

	float attackStrength = TotalAttackStrength(attackers, attackerFlags, attackerFighters, false);
	float defenceStrength = TotalAttackStrength(defenders, defenderFlags, defenderFighters, true);

	const float adjustedAttack = attackStrength * Random(0.5f, 2.0f);
	const float adjustedDefence = defenceStrength * Random(0.5f, 2.0f);

	assert(adjustedAttack > 0.0f);
	assert(adjustedDefence > 0.0f);

	if (adjustedAttack > adjustedDefence)
	{
		std::cout << "Attackers win - Final rolls: " << adjustedAttack << " vs " << adjustedDefence
			<< "; win ratio " << adjustedAttack/adjustedDefence << "." << std::endl;
		ApplyAttackResults(attackers, defenders, adjustedAttack, adjustedDefence,
			attackerFighters, defenderFighters, outDestroyEvents);
	}
	else
	{
		std::cout << "Defenders win - Final rolls: " << adjustedAttack << " vs " << adjustedDefence
			<< "; win ratio " << adjustedDefence/adjustedAttack << "." << std::endl;
		ApplyAttackResults(defenders, attackers, adjustedDefence, adjustedAttack,
			defenderFighters, attackerFighters, outDestroyEvents);
	}
}

float TotalAttackStrength(const std::vector<SUnit*>& units,
	const std::vector<Flag>& flags,
	const std::vector<SUnit*>& fighters,
	bool isDefender)
{
	if (isDefender)
	{
		std::cout << "Defenders: ";
	}
	else
	{
		std::cout << "Attackers: ";
	}
	std::cout << std::fixed << std::setprecision(2);

	float totalStrength = 0.0f;
	for (int i = 0; i < units.size(); ++i)
	{
		const SUnit& unit = *units[i];

		const float base = SUnitTypeStats::Get(unit.m_Type).m_Battle;
		const float formation = isDefender ?
			SFormationStats::Get(unit.m_Formation).m_DefenceFactor :
			SFormationStats::Get(unit.m_Formation).m_AttackFactor;
		const float flank = IsFlagSet(flags[i], f_Flanking) ? c_FlankBattleFactor : 1.0f;

		const float contribution = base
			* formation * flank
			* unit.m_Strength * GetEffectiveMorale(unit);
		totalStrength += contribution;

		if (i > 0)
		{
			std::cout << " + ";
		}
		std::cout << contribution;
	}

	for (int i = 0; i < fighters.size(); ++i)
	{
		const SUnit& fighterUnit = *fighters[i];
		const float contribution = GetCurrentFighterPower(fighterUnit) * c_FighterAttackFactor;
		totalStrength += contribution;

		std::cout << " + ";
		if (i == 0)
		{
			std::cout << "(ftrs) ";
		}
		std::cout << contribution;
	}

	if (units.size() > 1 || fighters.size() > 0)
	{
		std::cout << " = " << totalStrength;
	}
	std::cout << std::endl;

	return totalStrength;
}

void ApplyAttackResults(
	const std::vector<SUnit*>& winners,
	const std::vector<SUnit*>& losers,
	const float winStrength,
	const float loseStrength,
	const std::vector<SUnit*>& winnerFighters,
	const std::vector<SUnit*>& loserFighters,
	SDestroyEvents& outDestroyEvents)
{
	const float winRatio = std::min((winStrength / loseStrength), c_MaxWinRatio);
	float loserLosses = c_BaseBattleDamageForLosers * winRatio;
	for (SUnit* loserUnit : losers)
	{
		const float strengthDamage = loserLosses*Random(0.5f, 1.5f);
		const bool checkDisruption = true;
		ApplyDamage(*loserUnit, strengthDamage, c_MoraleLossForLosers, checkDisruption, outDestroyEvents);
	}
	for (SUnit* loserFighterUnit : loserFighters)
	{
		const float damage = loserLosses*Random(0.5f, 1.5f);
		ApplyFighterDamage(*loserFighterUnit, damage);
	}

	float winnerLosses = c_BaseBattleDamageForWinners * (1.0f / winRatio);
	for (SUnit* winnerUnit : winners)
	{
		const float strengthDamage = winnerLosses * Random(0.5f, 1.5f);
		const bool checkDisruption = false;
		ApplyDamage(*winnerUnit, strengthDamage, 0.0f, checkDisruption, outDestroyEvents);
	}
	for (SUnit* winnerFighterUnit : winnerFighters)
	{
		const float damage = winnerLosses*Random(0.5f, 1.5f);
		ApplyFighterDamage(*winnerFighterUnit, damage);
	}
}

// todo: award morale for destroyed ship?  mess up formation for successful attacker?
