#pragma once

#include "Common.h"
#include <vector>

struct SUnit;
struct SDestroyEvents;

void ApplyAttack(
	const std::vector<SUnit*>& attackers,
	const std::vector<Flag>& attackerFlags,
	const std::vector<SUnit*>& attackerFighters,
	const std::vector<SUnit*>& defenders,
	const std::vector<Flag>& defenderFlags,
	const std::vector<SUnit*>& defenderFighters,
	SDestroyEvents& outDestroyEvents);

float TotalAttackStrength(const std::vector<SUnit*>& units,
	const std::vector<Flag>& flags,
	const std::vector<SUnit*>& fighters,
	bool isDefender);

void ApplyAttackResults(
	const std::vector<SUnit*>& winners,
	const std::vector<SUnit*>& losers,
	const float winStrength,
	const float loseStrength,
	const std::vector<SUnit*>& winnerFighters,
	const std::vector<SUnit*>& loserFighters,
	SDestroyEvents& outDestroyEvents);
