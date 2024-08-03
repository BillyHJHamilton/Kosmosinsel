#include "LoadFleet.h"
#include "Formation.h"
#include "Game.h"
#include "Quality.h"
#include "Unit.h"
#include "UnitType.h"
#include "Utility.h"
#include <cfloat>
#include <iostream>
#include <sstream>

void CFleetLoader::Load(CGame& game, std::ifstream& file)
{

	while (file)
	{
		std::string line;
		std::getline(file, line);

		if (file)
		{
			std::stringstream stream(line);
			ProcessLine(stream, game);
		}
	}
}

void CFleetLoader::ProcessLine(std::stringstream& stream, CGame& game)
{
	// Maybe it starts with a number.
	int32_t unitId;
	stream >> unitId;
	if (stream)
	{
		ProcessUnit(stream, game, unitId);
	}
	else
	{
		// Maybe a word then?
		stream.clear();
		std::string word;
		stream >> word;
		LowercaseInPlace(word);
		
		if (word == "team")
		{
			ProcessTeam(stream);
		}
		else if (word == "commander")
		{
			ProcessCommander(stream);
		}
		else if (word != "")
		{
			std::cout << "Error: Unknown instruction " << word << std::endl;
		}
	}
}

void CFleetLoader::ProcessUnit(std::stringstream& stream, CGame& game, int32_t unitId)
{
	if (m_Team == c_IndexNone)
	{
		std::cout << "Error: No team set." << std::endl;
		return;
	}
	if (m_Commander == "")
	{
		std::cout << "Error: No commander set." << std::endl;
		return;
	}

	std::string typeName;
	stream >> typeName;
	if (!stream)
	{
		std::cout << "Error: Could not read unit type." << std::endl;
		return;
	}
	EUnitType type = StrToUnitType(typeName);
	if (type == EUnitType::None)
	{
		std::cout << "Error: Unknown unit type " << typeName << std::endl;
		return;
	}

	SUnit newUnit;
	newUnit.m_Id = unitId;
	newUnit.m_Team = m_Team;
	newUnit.m_Commander = m_Commander;
	newUnit.m_Type = type;

	// Defaults which may be overridden
	newUnit.m_Quality = EQuality::Standard;
	newUnit.m_Formation = EFormation::Line;
	newUnit.m_Strength = FLT_MAX;
	newUnit.m_Morale = FLT_MAX;
	newUnit.m_StatusFlags = f_None;

	// Parse other arguments
	while (stream)
	{
		ProcessUnitArgument(stream, newUnit);
	}

	if (newUnit.m_Morale == FLT_MAX)
	{
		newUnit.m_Morale = GetStartingMorale(newUnit.m_Quality);
	}
	if (newUnit.m_Strength == FLT_MAX)
	{
		newUnit.m_Strength = 1.0f;
	}

	game.AddUnit(newUnit);
}

void CFleetLoader::ProcessUnitArgument(std::stringstream& stream, SUnit& unit)
{
	std::string argument;
	stream >> argument;
	if (stream)
	{
		LowercaseInPlace(argument);
		EQuality quality = StrToQuality(argument);
		EFormation formation = StrToFormation(argument);

		if (quality != EQuality::None)
		{
			unit.m_Quality = quality;
		}
		else if (formation != EFormation::None)
		{
			unit.m_Formation = formation;
		}
		else if (argument == "morale")
		{
			float morale;
			stream >> morale;
			if (stream)
			{
				unit.m_Morale = morale;
			}
			else
			{
				std::cout << "Failed to read morale." << std::endl;
			}
		}
		else if (argument == "strength")
		{
			float strength;
			stream >> strength;
			if (stream)
			{
				unit.m_Strength = strength;
			}
			else
			{
				std::cout << "Failed to read strength." << std::endl;
			}
		}
		else if (argument == "disrupted")
		{
			SetFlag(unit.m_StatusFlags, f_Disrupted);
		}
		else if (argument == "received")
		{
			SetFlag(unit.m_StatusFlags, f_ReceivedFire);
		}
		else if (argument == "acted")
		{
			SetFlag(unit.m_StatusFlags, f_Acted);
		}
		else if (argument == "flagship")
		{
			SetFlag(unit.m_StatusFlags, f_Flagship);
		}
		else if (argument == "flagdmg")
		{
			SetFlag(unit.m_StatusFlags, f_FlagshipDamaged);
		}
	}
}

void CFleetLoader::ProcessTeam(std::stringstream& stream)
{
	int32_t teamNum;
	stream >> teamNum;
	if (stream)
	{
		m_Team = teamNum;
	}
	else
	{
		std::cout << "Failed to read team number." << std::endl;
	}
}

void CFleetLoader::ProcessCommander(std::stringstream& stream)
{
	std::string name;
	stream >> name;
	if (stream)
	{
		m_Commander = name;
	}
	else
	{
		std::cout << "Failed to read commander name." << std::endl;
	}
}

