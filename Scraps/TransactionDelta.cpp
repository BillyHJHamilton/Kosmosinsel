#include "TransactionDelta.h"

#include "GameState.h"
#include <cassert>

void CStrengthDelta::Apply(SGameState& gameState)
{
	auto result = gameState.m_Units.find(m_UnitId);
	if (result != gameState.m_Units.end())
	{
		m_OldValue = result->second.m_Strength;
		result->second.m_Strength += m_Delta;
	}
}

void CStrengthDelta::Undo(SGameState& gameState)
{
	assert(m_OldValue != FLT_MIN);
	auto result = gameState.m_Units.find(m_UnitId);
	if (result != gameState.m_Units.end())
	{
		result->second.m_Strength = m_OldValue;
	}
}

void CMoraleDelta::Apply(SGameState& gameState)
{
	auto result = gameState.m_Units.find(m_UnitId);
	if (result != gameState.m_Units.end())
	{
		m_OldValue = result->second.m_Morale;
		result->second.m_Strength += m_Delta;
	}
}

void CMoraleDelta::Undo(SGameState& gameState)
{
	assert(m_OldValue != FLT_MIN);
	auto result = gameState.m_Units.find(m_UnitId);
	if (result != gameState.m_Units.end())
	{
		result->second.m_Morale = m_OldValue;
	}
}

void CFormationDelta::Apply(SGameState& gameState)
{
	auto result = gameState.m_Units.find(m_UnitId);
	if (result != gameState.m_Units.end())
	{
		m_OldValue = result->second.m_Formation;
		result->second.m_Formation = m_NewValue;
	}
}

void CFormationDelta::Undo(SGameState& gameState)
{
	assert(m_OldValue != EFormation::Count);
	auto result = gameState.m_Units.find(m_UnitId);
	if (result != gameState.m_Units.end())
	{
		result->second.m_Formation = m_OldValue;
	}
}
