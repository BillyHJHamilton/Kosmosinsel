#pragma once

#include "Common.h"
#include <fstream>
#include <sstream>
#include <string>

class CGame;
struct SUnit;

class CFleetLoader
{
public:
	void Load(CGame& game, std::ifstream& file);

private:
	void ProcessLine(std::stringstream& stream, CGame& game);
	void ProcessUnit(std::stringstream& stream, CGame& game, int32_t unitId);
	void ProcessUnitArgument(std::stringstream& stream, SUnit& unit);
	void ProcessTeam(std::stringstream& stream);
	void ProcessCommander(std::stringstream& stream);

	int32_t m_Team = c_IndexNone;
	std::string m_Commander;
};

