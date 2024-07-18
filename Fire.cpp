#include "Fire.h"

#include "Damage.h"
#include "Formation.h"
#include "GameConstants.h"
#include "Morale.h"
#include "Random.h"
#include "Unit.h"
#include "UnitType.h"
#include <iostream>

static constexpr float s_RangeFactor[(int32_t)EFireRange::Count] =
{
	1.0f,	// Near
	0.5f	// Far
};

void ApplyFire(SUnit& attacker, SUnit& target, EFireRange range, Flag fireFlags,
	SDestroyEvents& outDestroyEvents)
{
	const float unitFactor = SUnitTypeStats::Get(attacker.m_Type).m_Fire;
	const float attackerFormation = SFormationStats::Get(attacker.m_Formation).m_FireFactor;
	const float targetFormation = SFormationStats::Get(target.m_Formation).m_ReceivedFactor;
	const float rangeFactor = s_RangeFactor[(int32_t)range];
	const float flankFactor = IsFlagSet(fireFlags, f_Flanking) ? c_FlankFireFactor : 1.0f;
	const float pseudoLancaster = 0.5f + (0.5f * target.m_Strength);

	const float targetDamage = c_BaseFireDamage
		* unitFactor
		* attacker.m_Strength * GetEffectiveMorale(attacker)
		* attackerFormation * targetFormation
		* rangeFactor * flankFactor
		* pseudoLancaster
		* Random(0.5f, 1.5f);

	const float targetMoraleLoss = c_BaseMoraleLossFromReceiving
		* unitFactor
		* attacker.m_Strength
		* attackerFormation * targetFormation
		* rangeFactor * flankFactor
		* Random(0.5f, 1.5f);

	const bool testMorale = !IsFlagSet(target.m_StatusFlags, f_ReceivedFire);
	ApplyDamage(target, targetDamage, targetMoraleLoss, testMorale, outDestroyEvents);
	SetFlag(target.m_StatusFlags, f_ReceivedFire);

	const float attackerMoraleLoss = c_BaseMoraleLossFromFiring
		* rangeFactor
		* Random(0.5f, 1.5f);
	//attacker.m_Morale -= attackerMoraleLoss;
	ApplyDamageMoraleOnly(attacker, attackerMoraleLoss, false);
}
