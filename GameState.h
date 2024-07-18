#pragma once

#include "Unit.h"
#include <string>
#include <unordered_map>

struct SCommandGroup
{
	std::string m_Commander;
	std::vector<SUnit> m_Units;
};

struct STeam
{
	int32_t m_TeamId;
	std::vector<SCommandGroup> m_CommandGroups;
};

struct SGameState
{
	std::vector<STeam> m_Teams;
};

