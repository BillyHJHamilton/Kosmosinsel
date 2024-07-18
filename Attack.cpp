#include "Attack.h"
#include "Damage.h"
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
	const std::vector<SUnit*>& defenders,
	const std::vector<Flag>& defenderFlags,
	SDestroyEvents& outDestroyEvents)
{
	assert(attackers.size() == attackerFlags.size());
	assert(defenders.size() == defenderFlags.size());

	std::cout << "Attackers: " << std::fixed << std::setprecision(2);
	float attackStrength = 0.0f;
	for (int i = 0; i < attackers.size(); ++i)
	{
		const SUnit& attacker = *attackers[i];

		const float base = SUnitTypeStats::Get(attacker.m_Type).m_Battle;
		const float formation = SFormationStats::Get(attacker.m_Formation).m_AttackFactor;
		const float flank = IsFlagSet(attackerFlags[i], f_Flanking) ? c_FlankBattleFactor : 1.0f;

		const float contribution = base
			* formation * flank
			* attacker.m_Strength * GetEffectiveMorale(attacker);
		attackStrength += contribution;

		if (i > 0)
		{
			std::cout << " + ";
		}
		std::cout << contribution;
	}
	if (attackers.size() > 1)
	{
		std::cout << " = " << attackStrength;
	}
	std::cout << std::endl;

	std::cout << "Defenders: ";
	float defStrength = 0.0f;
	for (int i = 0; i < defenders.size(); ++i)
	{
		const SUnit& defender = *defenders[i];

		const float base = SUnitTypeStats::Get(defender.m_Type).m_Battle;
		const float formation = SFormationStats::Get(defender.m_Formation).m_DefenceFactor;
		const float flank = IsFlagSet(defenderFlags[i], f_Flanking) ? c_FlankBattleFactor : 1.0f;

		const float contribution = base
			* formation * flank
			* defender.m_Strength * GetEffectiveMorale(defender);
		defStrength += contribution;

		if (i > 0)
		{
			std::cout << " + ";
		}
		std::cout << contribution;
	}
	if (defenders.size() > 1)
	{
		std::cout << " = " << defStrength;
	}
	std::cout << std::endl;

	const float adjustedAttack = attackStrength * Random(0.5f, 2.0f);
	const float adjustedDefence = defStrength * Random(0.5f, 2.0f);

	assert(adjustedAttack > 0.0f);
	assert(adjustedDefence > 0.0f);

	if (adjustedAttack > adjustedDefence)
	{
		std::cout << "Attackers win - Final rolls: " << adjustedAttack << " vs " << adjustedDefence
			<< "; win ratio " << adjustedAttack/adjustedDefence << "." << std::endl;
		ApplyAttackResults(attackers, defenders, adjustedAttack, adjustedDefence, outDestroyEvents);
	}
	else
	{
		std::cout << "Defenders win - Final rolls: " << adjustedAttack << " vs " << adjustedDefence
			<< "; win ratio " << adjustedDefence/adjustedAttack << "." << std::endl;
		ApplyAttackResults(defenders, attackers, adjustedDefence, adjustedAttack, outDestroyEvents);
	}
}

void ApplyAttackResults(
	const std::vector<SUnit*>& winners,
	const std::vector<SUnit*>& losers,
	const float winStrength,
	const float loseStrength,
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

	float winnerLosses = c_BaseBattleDamageForWinners * (1.0f / winRatio);
	for (SUnit* winnerUnit : winners)
	{
		const float strengthDamage = winnerLosses * Random(0.5f, 1.5f);
		const bool checkDisruption = false;
		ApplyDamage(*winnerUnit, strengthDamage, 0.0f, checkDisruption, outDestroyEvents);
	}
}

// todo: award morale for destroyed ship?  mess up formation for successful attacker?
