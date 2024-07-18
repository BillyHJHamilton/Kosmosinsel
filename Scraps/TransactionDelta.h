#pragma once

#include "Common.h"
#include "Formation.h"
#include <cfloat>

struct SGameState;
class CUnit;

class ITransactionDelta
{
public:
	virtual ~ITransactionDelta() {}
	virtual void Apply(SGameState& gameState) = 0;
	virtual void Undo(SGameState& gameState) = 0;
};

class CStrengthDelta : ITransactionDelta
{
public:
	CStrengthDelta(int32_t unitId, float delta) :
		m_UnitId(unitId), m_Delta(delta) {}

	virtual void Apply(SGameState& gameState) override;
	virtual void Undo(SGameState& gameState) override;

private:
	int32_t m_UnitId;
	float m_Delta;
	float m_OldValue = FLT_MIN;
};

class CMoraleDelta : ITransactionDelta
{
public:
	CMoraleDelta(int32_t unitId, float delta) :
		m_UnitId(unitId), m_Delta(delta) {}

	virtual void Apply(SGameState& gameState) override;
	virtual void Undo(SGameState& gameState) override;

private:
	int32_t m_UnitId;
	float m_Delta;
	float m_OldValue = FLT_MIN;
};

class CFormationDelta: ITransactionDelta
{
public:
	CFormationDelta(int32_t unitId, EFormation newValue) :
		m_UnitId(unitId), m_NewValue(newValue) {}

	virtual void Apply(SGameState& gameState) override;
	virtual void Undo(SGameState& gameState) override;

private:
	int32_t m_UnitId;
	EFormation m_NewValue;
	EFormation m_OldValue = EFormation::Count;
};
