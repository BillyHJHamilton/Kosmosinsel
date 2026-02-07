#pragma once

#include "GameState.h"
#include <stack>
#include <string>
#include <vector>

struct SUnit;
struct SDestroyEvent;

class CGame
{
public:
	void PushHistory(std::string& description);
	void Undo();

	const SGameState& GetGameState() { return m_GameState; }

	bool HasTeam(int32_t teamId) const;
	bool HasCommandGroup(const std::string& commander) const;
	bool HasUnit(int32_t id) const;

	const STeam* FindTeam (int32_t teamId) const;
	const SCommandGroup* FindCommandGroup(const std::string& commander) const;
	const SCommandGroup* FindCommandGroup(const STeam& team, const std::string& commander) const;
	const SUnit* FindUnit(int32_t id) const;

	STeam* FindTeam (int32_t teamId);
	SCommandGroup* FindCommandGroup(const std::string& commander);
	SCommandGroup* FindCommandGroup(STeam& team, const std::string& commander);
	SUnit* FindUnit(int32_t id);

	STeam* FindFollowingTeam (int32_t teamId);

	bool TryFindUnits(const std::vector<int32_t>& unitsToFind, std::vector<SUnit*>& outUnits, int32_t& outInvalidId);
	void AddUnit(const SUnit& newUnit);
	void DestroyUnit(int32_t unitId);

	// Public members
	bool m_ShouldQuit = false;

private:
	STeam* AddTeam(int32_t teamId);
	SCommandGroup* AddCommandGroup(STeam& team, std::string name);

	SGameState m_GameState;

	struct SHistoryEntry
	{
		std::string m_Description;
		SGameState m_State;
	};

	// Complete game state history, to support the "Undo" feature.
	// It would be more memory-efficient to store only the deltas for each transaction.
	// However, this will be much simpler and I don't think memory will be a problem.
	std::stack<SHistoryEntry> m_GameStateHistory;
};
