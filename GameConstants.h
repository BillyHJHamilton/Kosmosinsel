#pragma once

// Fire
static constexpr float c_BaseFireDamage					= 0.12f;
static constexpr float c_BaseMoraleLossFromFiring		= 0.03f;
static constexpr float c_BaseMoraleLossFromReceiving	= 0.06f;
static constexpr float c_FlankFireFactor				= 2.0f;

// Attacks
static constexpr float c_MoraleLossForLosers			= 0.20f;
static constexpr float c_BaseBattleDamageForLosers		= 0.10f;
static constexpr float c_BaseBattleDamageForWinners		= 0.07f;
static constexpr float c_FlankBattleFactor				= 2.0f;
static constexpr float c_MaxWinRatio					= 3.0f;

// Thresholds
static constexpr float c_FlagshipHitThreshold			= 0.9f;
static constexpr float c_FlagshipHitFactor				= 1.2f;
static constexpr float c_RoutThreshold					= 0.2f;
static constexpr float c_DestroyedThreshold				= 0.1f;

// Morale effects
static constexpr float c_DisruptionMoralePenalty		= 0.2f;
static constexpr float c_FlagshipMoraleBonus			= 0.1f;
static constexpr float c_MoraleLossOnFlagshipLoss		= 0.2f;
