#include "Morale.h"

#include "GameConstants.h"
#include "Output.h"
#include "Random.h"
#include "Unit.h"
#include <algorithm>
#include <iostream>

float GetEffectiveMorale(const SUnit& unit)
{
	float effectiveMorale = unit.m_Morale;
	if (IsFlagSet(unit.m_StatusFlags, f_Disrupted))
	{
		effectiveMorale -= c_DisruptionMoralePenalty;
	}
	if (IsFlagSet(unit.m_StatusFlags, f_Flagship))
	{
		effectiveMorale += c_FlagshipMoraleBonus;
	}
	return std::max(0.0f, effectiveMorale);
}

bool TestMorale(const SUnit& unit)
{
	const float roll = Random(0.0f, 1.0f);
	return roll < GetEffectiveMorale(unit);
}

void TestMoraleForDisruption(SUnit& unit)
{
	if (IsFlagSet(unit.m_StatusFlags, f_Disrupted))
	{
		return;
	}

	if (!TestMorale(unit))
	{
		CYellowScope y;
		SetFlag(unit.m_StatusFlags, f_Disrupted);
		std::cout << unit.m_Id << " failed morale test and became disrupted." << std::endl;
	}
	else
	{
		//std::cout << unit.m_Id << " passed morale test." << std::endl;
	}
}

bool ShouldRout(const SUnit& unit)
{
	// Don't count "effective" modifiers for this check.
	return unit.m_Morale + c_Epsilon < c_RoutThreshold;
}