#include "Damage.h"

//#include "Fire.h"
#include "Fighters.h"
#include "Formation.h"
#include "Game.h"
#include "GameConstants.h"
#include "Morale.h"
#include "Output.h"
#include "Random.h"
#include <algorithm>
#include <cassert>
#include <iomanip>
#include <iostream>

void ApplyDamage(SUnit& unit, float strengthDamage, float moraleDamage, bool testDisruption, SDestroyEvents& inOutDestroyEvents)
{
	if (IsDestroyed(unit))
	{
		return;
	}

	const float strengthBefore = unit.m_Strength;

	unit.m_Strength = std::max(0.0f, unit.m_Strength - strengthDamage);
	unit.m_Morale = std::max(0.0f, unit.m_Morale - moraleDamage);

	{
		CDarkGreyScope grey;
		std::cout << std::fixed << std::setprecision(0)
			<< unit.m_Id << " lost "
			<< 100.0f*strengthDamage << "% strength (now "
			<< 100.0f*unit.m_Strength << "%) and "
			<< 100.0f*moraleDamage << "% morale (now "
			<< 100.0f*unit.m_Morale << "%";
		const float effectiveMorale = GetEffectiveMorale(unit);
		if (effectiveMorale != unit.m_Morale)
		{
			std::cout << ", effective " << 100.0f*GetEffectiveMorale(unit) << "%";
		}
		std::cout << "); overall "
			<< 100.0f*GetEffectiveMorale(unit)*unit.m_Strength << "%" << std::endl;
	}

	if (IsDestroyed(unit))
	{
		CRedScope r;
		//std::cout << "Unit " << unit.m_Id << " is destroyed!" << std::endl;
		inOutDestroyEvents.m_UnitsDestroyed.push_back(unit.m_Id);
		if (IsFlagSet(unit.m_StatusFlags, f_Flagship))
		{
			inOutDestroyEvents.m_FlagshipsLost.push_back(unit.m_Commander);
		}
	}
	else
	{
		if (HasSurvivingFighters(unit))
		{
			if (unit.m_FighterState == EFighterState::Docked)
			{
				const float fighterDamage = strengthDamage * c_FighterDamageWhenDocked * Random(0.0f, 2.0f);
				ApplyFighterDamage(unit, fighterDamage);
			}
			else if (unit.m_FighterState == EFighterState::Launching)
			{
				const float fighterDamage = strengthDamage * c_FighterDamageWhenLaunching * Random(0.0f, 2.0f);
				ApplyFighterDamage(unit, fighterDamage);
			}
			if (unit.m_FighterState == EFighterState::Recalling)
			{
				const float fighterDamage = strengthDamage * c_FighterDamageWhenRecalling * Random(0.0f, 2.0f);
				ApplyFighterDamage(unit, fighterDamage);
			}
		}

		CheckFlagshipDamage(unit, strengthDamage, strengthBefore, inOutDestroyEvents);
		CheckMoraleEffects(unit, testDisruption);
	}
}

void ApplyDamageMoraleOnly(SUnit& unit, float moraleDamage, bool testDisruption)
{
	if (IsDestroyed(unit))
	{
		return;
	}

	unit.m_Morale = std::max(0.0f, unit.m_Morale - moraleDamage);

	{
		CDarkGreyScope grey;
		std::cout << std::fixed << std::setprecision(0)
			<< unit.m_Id << " lost "
			<< 100.0f*moraleDamage << "% morale (now "
			<< 100.0f*unit.m_Morale << "%";
		const float effectiveMorale = GetEffectiveMorale(unit);
		if (effectiveMorale != unit.m_Morale)
		{
			std::cout << ", effective " << 100.0f*GetEffectiveMorale(unit) << "%";
		}
		std::cout << "); strength is "
			<< 100.0f*unit.m_Strength << "%; overall "
			<< 100.0f*GetEffectiveMorale(unit)*unit.m_Strength << "%" << std::endl;
	}

	CheckMoraleEffects(unit, testDisruption);
}

void CheckFlagshipDamage(SUnit& unit, float damage, float strengthBefore, SDestroyEvents& inOutDestroyEvents)
{
	if (IsFlagSet(unit.m_StatusFlags, f_Flagship) &&
		unit.m_Strength + c_Epsilon < c_FlagshipHitThreshold)
	{
		const float chanceOfHit = (damage / strengthBefore) * c_FlagshipHitFactor;
		//std::cout << "Chance of hit = " << chanceOfHit << std::endl;
		if (Random(0.0f, 1.0f) < chanceOfHit)
		{
			if (!IsFlagSet(unit.m_StatusFlags, f_FlagshipDamaged))
			{
				CYellowScope y;
				std::cout << unit.m_Commander << "'s flagship is damaged!" << std::endl;
				SetFlag(unit.m_StatusFlags, f_FlagshipDamaged);
			}
			else
			{
				ClearFlag(unit.m_StatusFlags, f_Flagship);
				ClearFlag(unit.m_StatusFlags, f_FlagshipDamaged);
				inOutDestroyEvents.m_FlagshipsLost.push_back(unit.m_Commander);
			}
		}
	}
}

void CheckMoraleEffects(SUnit& unit, bool testDisruption)
{
	if (ShouldRout(unit))
	{
		if (unit.m_Formation != EFormation::Rout)
		{
			CRedScope r;
			std::cout << "Unit " << unit.m_Id << " is routed!" << std::endl;
			unit.m_Formation = EFormation::Rout;
		}
	}
	else if (testDisruption)
	{
		TestMoraleForDisruption(unit);
	}
}

void HandleDestroyEvents(CGame& game, const SDestroyEvents& destroyEvents)
{
	for (int32_t unitId : destroyEvents.m_UnitsDestroyed)
	{
		CRedScope r;
		std::cout << "Unit " << unitId << " was destroyed!" << std::endl;
		game.DestroyUnit(unitId);
	}

	for (const std::string& name : destroyEvents.m_FlagshipsLost)
	{
		SCommandGroup* group = game.FindCommandGroup(name);
		if (group)
		{
			CRedScope r;
			std::cout << name << "'s flagship was destroyed!" << std::endl;
			for (SUnit& unit : group->m_Units)
			{
				ApplyDamageMoraleOnly(unit, c_MoraleLossOnFlagshipLoss, true);
			}
		}
	}
}

bool IsDestroyed(const SUnit& unit)
{
	return unit.m_Strength + c_Epsilon < c_DestroyedThreshold;
}
