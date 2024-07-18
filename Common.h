#pragma once

#include <cstdint>

using Flag = uint32_t;
static constexpr Flag f_None = 0u;
inline void SetFlag(Flag& bitset, Flag flagToSet) { bitset |= flagToSet; }
inline void ClearFlag(Flag& bitset, Flag flagToClear) { bitset &= ~flagToClear; }
inline bool IsFlagSet(const Flag& bitset, Flag flagToCheck) { return bitset & flagToCheck; }

static constexpr int32_t c_IndexNone = -1;
static constexpr float c_Epsilon = 0.001f;