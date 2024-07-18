#include "Transaction.h"
#include <cassert>

void CTransaction::AddDelta(ITransactionDelta* newDelta)
{
	m_Deltas.emplace_back(newDelta);
}

void CTransaction::Apply(SGameState& gameState)
{
	assert(!m_Applied);
	for (int32_t i = 0; i < m_Deltas.size(); ++i)
	{
		m_Deltas[i]->Apply(gameState);
	}
	m_Applied = true;
}

void CTransaction::Undo(SGameState& gameState)
{
	assert(m_Applied);
	for (int32_t i = m_Deltas.size() - 1; i >= 0; --i)
	{
		m_Deltas[i]->Undo(gameState);
	}
	m_Applied = false;
}
