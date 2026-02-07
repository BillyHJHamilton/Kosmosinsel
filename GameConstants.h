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

// Fighters - Airstrikes and Dogfights
static constexpr float c_DogfightDefenderBonus			= 1.25f;
static constexpr float c_BaseDogfightDamageForLosers	= 0.12f;
static constexpr float c_BaseDogfightDamageForWinners	= 0.10f;
static constexpr float c_BaseDefensiveFireDamage		= 0.10f;
static constexpr float c_BaseStrikeDamageIfNoDogfight	= 0.09f;
static constexpr float c_BaseStrikeDamageIfWonDogfight	= 0.06f;
static constexpr float c_BaseStrikeDamageIfLostDogfight	= 0.04f;
static constexpr float c_BaseStrikeMoraleIfNoDogfight	= 0.05f;
static constexpr float c_BaseStrikeMoraleIfWonDogfight	= 0.03f;
static constexpr float c_BaseStrikeMoraleIfLostDogfight	= 0.02f;

// Fighters - Damage taken when mother unit is hit
static constexpr float c_FighterDamageWhenDocked		= 0.60f;
static constexpr float c_FighterDamageWhenLaunching		= 1.40f;
static constexpr float c_FighterDamageWhenRecalling		= 1.40f;

// Fighters - Other
static constexpr float c_BaseFighterAttrition			= 0.08f;
static constexpr float c_FighterAttackFactor			= 0.25f;

// Thresholds
static constexpr float c_FlagshipHitThreshold			= 0.9f;
static constexpr float c_FlagshipHitFactor				= 1.2f;
static constexpr float c_RoutThreshold					= 0.2f;
static constexpr float c_DestroyedThreshold				= 0.1f;
static constexpr float c_MoraleLostToTestDisruption		= 0.05f;

// Morale effects
static constexpr float c_DisruptionMoralePenalty		= 0.2f;
static constexpr float c_FlagshipMoraleBonus			= 0.1f;
static constexpr float c_MoraleLossOnFlagshipLoss		= 0.2f;
