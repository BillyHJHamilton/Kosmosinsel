#pragma once

#include "Common.h"
#include <string>
#include <vector>

class CGame;
struct SUnit;

struct SDestroyEvents
{
	std::vector<int32_t> m_UnitsDestroyed;
	std::vector<std::string> m_FlagshipsLost;
};

void ApplyDamage(SUnit& unit, float strengthDamage, float moraleDamage, bool testDisruption, SDestroyEvents& inOutDestroyEvents);
void ApplyDamageMoraleOnly(SUnit& unit, float moraleDamage, bool testDisruption);
void CheckFlagshipDamage(SUnit& unit, float damage, float strengthBefore, SDestroyEvents& inOutDestroyEvents);
void CheckMoraleEffects(SUnit& unit, bool testDisruption);
void HandleDestroyEvents(CGame& game, const SDestroyEvents& destroyEvents);
bool IsDestroyed(const SUnit& unit);
