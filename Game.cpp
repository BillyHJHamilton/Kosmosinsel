#include "Game.h"
#include "Utility.h"
#include <algorithm>
#include <iostream>
#include <unordered_map>

void CGame::PushHistory(std::string& description)
{
	m_GameStateHistory.push({ description, m_GameState });
}

void CGame::Undo()
{
	if (m_GameStateHistory.empty())
	{
		std::cout << "No commands to undo." << std::endl;
	}
	else
	{
		std::cout << "Undo: " << m_GameStateHistory.top().m_Description << std::endl;
		m_GameState = m_GameStateHistory.top().m_State;
		m_GameStateHistory.pop();
	}
}

bool CGame::HasTeam(int32_t teamId) const
{
	return FindTeam(teamId) != nullptr;
}

bool CGame::HasCommandGroup(const std::string& commander) const
{
	return FindCommandGroup(commander) != nullptr;
}

bool CGame::HasUnit(int32_t id) const
{
	return FindUnit(id) != nullptr;
}

const STeam* CGame::FindTeam(int32_t teamId) const
{
	for (const STeam& t : m_GameState.m_Teams)
	{
		if (t.m_TeamId == teamId)
		{
			return &t;
		}
	}
	return nullptr;
}

const SCommandGroup* CGame::FindCommandGroup(const std::string& commander) const
{
	for (const STeam& t : m_GameState.m_Teams)
	{
		for (const SCommandGroup& cg : t.m_CommandGroups)
		{
			if (cg.m_Commander == commander)
			{
				return &cg;
			}
		}
	}
	return nullptr;
}

const SCommandGroup* CGame::FindCommandGroup(const STeam& team, const std::string& commander) const
{
	for (const SCommandGroup& cg : team.m_CommandGroups)
	{
		if (cg.m_Commander == commander)
		{
			return &cg;
		}
	}
	return nullptr;
}

const SUnit* CGame::FindUnit(int32_t id) const
{
	for (const STeam& t : m_GameState.m_Teams)
	{
		for (const SCommandGroup& cg : t.m_CommandGroups)
		{
			for (const SUnit& u : cg.m_Units)
			{
				if (u.m_Id == id)
				{
					return &u;
				}
			}
		}
	}
	return nullptr;
}

STeam* CGame::FindTeam(int32_t teamId)
{
	return const_cast<STeam*>(const_cast<const CGame*>(this)->FindTeam(teamId));
}

SCommandGroup* CGame::FindCommandGroup(const std::string& commander)
{
	return const_cast<SCommandGroup*>(const_cast<const CGame*>(this)->FindCommandGroup(commander));
}

SCommandGroup* CGame::FindCommandGroup(STeam& team, const std::string& commander)
{
	return const_cast<SCommandGroup*>(const_cast<const CGame*>(this)->FindCommandGroup(team, commander));
}

SUnit* CGame::FindUnit(int32_t id)
{
	return const_cast<SUnit*>(const_cast<const CGame*>(this)->FindUnit(id));
}

STeam* CGame::FindFollowingTeam(int32_t teamId)
{
	for (int i = 0; i < m_GameState.m_Teams.size(); ++i)
	{
		if (m_GameState.m_Teams[i].m_TeamId == teamId)
		{
			if (i < m_GameState.m_Teams.size() - 1)
			{
				return &m_GameState.m_Teams[i + 1];
			}
			else
			{
				return &m_GameState.m_Teams[0];
			}
		}
	}

	return nullptr;
}

bool CGame::TryFindUnits(const std::vector<int32_t>& unitsToFind, std::vector<SUnit*>& outUnits, int32_t& outInvalidId)
{
	outUnits.reserve(unitsToFind.size());
	for (int32_t id : unitsToFind)
	{
		SUnit* unit = FindUnit(id);
		if (unit)
		{
			outUnits.push_back(unit);
		}
		else
		{
			outInvalidId = id;
			return false;
		}
	}
	return true;
}

void CGame::AddUnit(const SUnit& newUnit)
{
	if (HasUnit(newUnit.m_Id))
	{
		std::cout << "Cannot add unit " << newUnit.m_Id << " - duplicate ID."
			<< std::endl;
		return;
	}

	STeam* team = FindTeam(newUnit.m_Team);
	if (!team)
	{
		team = AddTeam(newUnit.m_Team);
	}

	SCommandGroup* commandGroup = FindCommandGroup(*team, newUnit.m_Commander);
	if (!commandGroup)
	{
		commandGroup = AddCommandGroup(*team, newUnit.m_Commander);
	}

	commandGroup->m_Units.push_back(newUnit);
	std::cout << GetUnitStatusString(newUnit) << std::endl;
}

void CGame::DestroyUnit(int32_t unitId)
{
	for (STeam& t : m_GameState.m_Teams)
	{
		for (SCommandGroup& cg : t.m_CommandGroups)
		{
			for (auto itr = cg.m_Units.begin(); itr != cg.m_Units.end(); ++itr)
			{
				if (itr->m_Id == unitId)
				{
					cg.m_Units.erase(itr);
					return;
				}
			}
		}
	}
}

STeam* CGame::AddTeam(int32_t teamId)
{
	m_GameState.m_Teams.push_back(STeam());
	m_GameState.m_Teams.back().m_TeamId = teamId;
	std::cout << "Added team " << teamId << "." << std::endl;
	return &(m_GameState.m_Teams.back());
}

SCommandGroup* CGame::AddCommandGroup(STeam& team, std::string name)
{
	team.m_CommandGroups.push_back(SCommandGroup());
	team.m_CommandGroups.back().m_Commander = name;
	std::cout << "Added commander " << name << "." << std::endl;
	return &(team.m_CommandGroups.back());
}

