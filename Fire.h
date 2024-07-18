#pragma once

#include "Common.h"

struct SUnit;
struct SDestroyEvents;

enum class EFireRange : uint8_t
{
	Near,
	Far,

	Count,
	None = Count
};

static constexpr Flag f_Flanking = 1 << 0;

void ApplyFire(SUnit& attacker, SUnit& target, EFireRange range, Flag fireFlags, SDestroyEvents& outDestroyEvents);
