#pragma once

#include "Common.h"
#include <vector>

struct SUnit;
struct SDestroyEvents;

void ApplyAttack(
	const std::vector<SUnit*>& attackers,
	const std::vector<Flag>& attackerFlags,
	const std::vector<SUnit*>& defenders,
	const std::vector<Flag>& defenderFlags,
	SDestroyEvents& outDestroyEvents);

void ApplyAttackResults(
	const std::vector<SUnit*>& winners,
	const std::vector<SUnit*>& losers,
	const float winStrength,
	const float loseStrength,
	SDestroyEvents& outDestroyEvents);
