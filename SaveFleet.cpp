#include "SaveFleet.h"

#include "Game.h"
#include "GameState.h"
#include "Unit.h"

void SaveFleet(CGame& game, std::ofstream& file)
{
	const SGameState& state = game.GetGameState();

	for (const STeam& team : state.m_Teams)
	{
		file << "Team " << team.m_TeamId << std::endl;
		for (const SCommandGroup& commandGroup : team.m_CommandGroups)
		{
			file << "Commander " << commandGroup.m_Commander << std::endl;
			for (const SUnit& unit : commandGroup.m_Units)
			{
				file << GetUnitStatusString(unit) << std::endl;
			}
		}
	}
}
