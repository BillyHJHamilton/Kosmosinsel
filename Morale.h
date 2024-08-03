#pragma once

struct SUnit;

float GetEffectiveMorale(const SUnit& unit);
bool ShouldRout(const SUnit& unit);

bool TestMorale(const SUnit& unit);
void TestMoraleForDisruption(SUnit& unit);
void TryToRally(SUnit& unit);
