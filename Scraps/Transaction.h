#pragma once

#include "Common.h"
#include "TransactionDelta.h"
#include <memory>
#include <vector>

class CTransaction
{
public:
	void AddDelta(ITransactionDelta* newDelta);
	void Apply(SGameState& gameState);
	void Undo(SGameState& gameState);

private:
	bool m_Applied;
	std::vector<std::shared_ptr<ITransactionDelta>> m_Deltas;
};