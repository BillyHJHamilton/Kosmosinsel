#include "Command.h"

#include "Attack.h"
#include "Damage.h"
#include "Fighters.h"
#include "Fire.h"
#include "Formation.h"
#include "Game.h"
#include "LoadFleet.h"
#include "Morale.h"
#include "SaveFleet.h"
#include "UnitType.h"
#include "Utility.h"
#include <cassert>
#include <fstream>
#include <iostream>

const static std::string s_CommandString [(int32_t)ECommand::Count]
{
	"near",
	"far",
	"attack",
	"form",
	"stat",
	"test",

	"launch",
	"strike",
	"defend",
	"recall",

	"end",

	"undo",
	"load",
	"save",
	"quit"
};

ECommand StrToCommand(std::string word)
{
	for (char& c : word)
	{
		c = tolower(c);
	}

	for (int32_t i = 0; i < (int32_t)ECommand::Count; ++i)
	{
		if (word == s_CommandString[i])
		{
			return (ECommand)i;
		}
	}
	return ECommand::None;
}

void HandleCommand(CGame& game)
{
	std::cout << "> ";

	std::string line;
	std::getline(std::cin, line);
	std::stringstream stream(line);

	SUnitFlagList subjectList = TryParseUnitFlagList(stream, game);
	ECommand command = TryParseCommand(stream);
	if (command == ECommand::None)
	{
		std::cout << "Failed to parse comand." << std::endl;
		return;
	}

	switch(command)
	{
	case ECommand::Near:
		HandleFire(line, game, subjectList, TryParseUnitFlagList(stream, game), EFireRange::Near);
		break;
	case ECommand::Far:
		HandleFire(line, game, subjectList, TryParseUnitFlagList(stream, game), EFireRange::Far);
		break;
	case ECommand::Attack:
		HandleAttack(line, game, subjectList, TryParseUnitFlagList(stream, game));
		break;
	case ECommand::Form:
		HandleForm(line, game, subjectList, TryParseFormation(stream));
		break;
	case ECommand::Stat:
		HandleStat(line, game, subjectList);
		break;
	case ECommand::Test:
		HandleTest(line, game, subjectList);
		break;
	case ECommand::Launch:
		HandleLaunch(line, game, subjectList);
		break;
	case ECommand::Strike:
		HandleStrike(line, game, subjectList, TryParseUnitFlagList(stream, game));
		break;
	case ECommand::Defend:
		HandleDefend(line, game, subjectList, TryParseUnitFlagList(stream, game));
		break;
	case ECommand::Recall:
		HandleRecall(line, game, subjectList);
		break;
	case ECommand::End:
		HandleEnd(line, stream, game);
		break;
	case ECommand::Undo:
		HandleUndo(game);
		break;
	case ECommand::Load:
		HandleLoad(stream, game);
		break;
	case ECommand::Save:
		HandleSave(stream, game);
		break;
	case ECommand::Quit:
		HandleQuit(game);
		break;
	default:
		std::cout << "Unhandled case." << std::endl;
	}
};

SUnitFlagList TryParseUnitFlagList(std::stringstream& stream, CGame& game)
{
	SUnitFlagList result;

L_TryRead:
	int x;
	stream >> x;
	if (stream)
	{
		SUnit* unit = game.FindUnit(x);
		if (!unit)
		{
			std::cout << "Cannot find unit " << x << "." << std::endl;
			return SUnitFlagList();
		}

		result.m_UnitList.push_back(unit);
		result.m_FlagList.push_back(f_None);

		char next = stream.peek();
		if (next == '+')
		{
			char dummy;
			stream.get(dummy);
			SetFlag(result.m_FlagList.back(), f_Flanking);
		}

		next = stream.peek();
		if (next == ',')
		{
			char dummy;
			stream.get(dummy);
			goto L_TryRead;
		}
	}
	else
	{
		stream.clear();
	}

	return result;
}

ECommand TryParseCommand(std::stringstream& stream)
{
	std::string word;
	stream >> word;
	if (stream)
	{
		return StrToCommand(word);
	}
	return ECommand::None;
}

EFormation TryParseFormation(std::stringstream& stream)
{
	std::string word;
	stream >> word;
	if (stream)
	{
		return StrToFormation(word);
	}
	return EFormation::None;
}

bool AreTeamsDistinct(const std::vector<SUnit*>& a, const std::vector<SUnit*>& b)
{
	if (a.empty() || b.empty())
	{
		return false;
	}

	int32_t teamA = a[0]->m_Team;
	int32_t teamB = b[0]->m_Team;

	if (teamA == teamB)
	{
		return false;
	}

	for (int32_t i = 1; i < a.size(); ++i)
	{
		if (a[i]->m_Team != teamA)
		{
			return false;
		}
	}
	for (int32_t i = 1; i < b.size(); ++i)
	{
		if (b[i]->m_Team != teamB)
		{
			return false;
		}
	}

	return true;
}

void HandleFire(std::string& line, CGame& game, const SUnitFlagList& subjectList, const SUnitFlagList& targetList, EFireRange range)
{
	if (subjectList.m_UnitList.empty())
	{
		std::cout << "You must specify the unit(s) to fire." << std::endl;
		return;
	}
	if (targetList.m_UnitList.empty())
	{
		std::cout << "You must specify the target unit(s)." << std::endl;
		return;
	}

	const SUnit* const common = FindCommonElement(subjectList.m_UnitList, targetList.m_UnitList, (SUnit*)nullptr);
	if (common != nullptr)
	{
		std::cout << "Unit " << common->m_Id << " cannot fire at itself." << std::endl;
		return;
	}

	if (!AreTeamsDistinct(subjectList.m_UnitList, targetList.m_UnitList))
	{
		std::cout << "Attacking units and target units do not form distinct teams." << std::endl;
		return;
	}

	for (SUnit* u : subjectList.m_UnitList)
	{
		if (IsFlagSet(u->m_StatusFlags, f_Acted))
		{
			std::cout << "Unit " << u->m_Id << " has already acted this turn." << std::endl;
			return;
		}
		if (u->m_Formation == EFormation::Rout)
		{
			std::cout << "Unit " << u->m_Id << " has routed and cannot fire." << std::endl;
			return;
		}
	}

	if (targetList.m_UnitList.size() > 1)
	{
		for (SUnit* u : subjectList.m_UnitList)
		{
			if (u->m_Formation != EFormation::Sphere)
			{
				std::cout << "Unit " << u->m_Id << " is not in sphere, so it cannot fire at multiple targets." << std::endl;
				return;
			}
		}
	}

	// Action seems valid, so save current state.
	game.PushHistory(line);

	SDestroyEvents destroyEvents;
	for (int i = 0; i < subjectList.m_UnitList.size(); ++i)
	{
		SUnit* f = subjectList.m_UnitList[i];
		Flag fireFlags = subjectList.m_FlagList[i];

		for (SUnit* t : targetList.m_UnitList)
		{
			ApplyFire(*f, *t, range, fireFlags, destroyEvents);
		}
	}
	HandleDestroyEvents(game, destroyEvents);
}

void HandleAttack(std::string& line, CGame& game, const SUnitFlagList& subjectList, const SUnitFlagList& targetList)
{
	const std::vector<SUnit*>& attackingUnits = subjectList.m_UnitList;
	const std::vector<SUnit*>& defendingUnits = targetList.m_UnitList;

	if (attackingUnits.empty())
	{
		std::cout << "You must specify the attacking unit(s)." << std::endl;
		return;
	}
	if (defendingUnits.empty())
	{
		std::cout << "You must specify the defending unit(s)." << std::endl;
		return;
	}

	const SUnit* const common = FindCommonElement(attackingUnits, defendingUnits, (SUnit*)nullptr);
	if (common != nullptr)
	{
		std::cout << "Unit " << common->m_Id << " cannot fight on both sides." << std::endl;
		return;
	}

	if (!AreTeamsDistinct(subjectList.m_UnitList, targetList.m_UnitList))
	{
		std::cout << "Attacking units and target units do not form distinct teams." << std::endl;
		return;
	}

	for (SUnit* u : attackingUnits)
	{
		if (IsFlagSet(u->m_StatusFlags, f_Acted))
		{
			std::cout << "Unit " << u->m_Id << " has already acted this turn." << std::endl;
			return;
		}
	}

	// Find all relevant fighters.
	std::vector<SUnit*> attackingFighters = GetAllDefendingUnits(game, attackingUnits);
	std::vector<SUnit*> fightersStrikingDefenders = GetAllStrikingUnits(game, defendingUnits, attackingUnits[0]->m_Team);
	attackingFighters.insert(attackingFighters.end(), fightersStrikingDefenders.begin(), fightersStrikingDefenders.end());
	std::vector<SUnit*> defendingFighters = GetAllDefendingUnits(game, defendingUnits);
	std::vector<SUnit*> fightersStrikingAttackers = GetAllStrikingUnits(game, attackingUnits, defendingUnits[0]->m_Team);
	defendingFighters.insert(defendingFighters.end(), fightersStrikingAttackers.begin(), fightersStrikingAttackers.end());

	game.PushHistory(line);
	SDestroyEvents destroyEvents;
	ApplyAttack(attackingUnits, subjectList.m_FlagList, attackingFighters,
		defendingUnits, targetList.m_FlagList, defendingFighters,
		destroyEvents);
	HandleDestroyEvents(game, destroyEvents);
}

void HandleForm(std::string& line, CGame& game, const SUnitFlagList& subjectList, EFormation formation)
{
	if (subjectList.m_UnitList.empty())
	{
		std::cout << "You must specify the unit(s) to change formation." << std::endl;
		return;
	}

	if (formation == EFormation::None)
	{
		std::cout << "Invalid formation." << std::endl;
		return;
	}

	game.PushHistory(line);
	for (SUnit* unit : subjectList.m_UnitList)
	{
		unit->m_Formation = formation;
	}
	std::cout << "Done." << std::endl;
}

void HandleStat(std::string& line, CGame& game, const SUnitFlagList& subjectList)
{
	if (subjectList.m_UnitList.empty())
	{
		std::cout << "You must specify the unit(s) to report status." << std::endl;
		return;
	}

	for (SUnit* unit : subjectList.m_UnitList)
	{
		std::cout << GetUnitStatusString(*unit) << std::endl;
	}
}

void HandleTest(std::string& line, CGame& game, const SUnitFlagList& subjectList)
{
	if (subjectList.m_UnitList.empty())
	{
		std::cout << "You must specify the unit(s) to test morale." << std::endl;
		return;
	}

	game.PushHistory(line);
	for (SUnit* unit : subjectList.m_UnitList)
	{
		TestMoraleForDisruption(*unit);
	}
}

void HandleLaunch(std::string& line, CGame& game, const SUnitFlagList& subjectList)
{
	if (subjectList.m_UnitList.empty())
	{
		std::cout << "You must specify the unit(s) to launch fighters." << std::endl;
		return;
	}

	for (SUnit* u : subjectList.m_UnitList)
	{
		if (SUnitTypeStats::Get(u->m_Type).m_Fighters < c_Epsilon)
		{
			std::cout << "Unit " << u->m_Id << " is " << UnitTypeToStr(u->m_Type)
				<< ", which do not carry fighters." << std::endl;
			return;
		}
		if (!HasSurvivingFighters(*u))
		{
			std::cout << "Unit " << u->m_Id << " has no fighters left!" << std::endl;
			return;
		}

		if (u->m_FighterState != EFighterState::Docked)
		{
			std::cout << "Unit " << u->m_Id << " does not have docked fighters." << std::endl;
			return;
		}
	}

	game.PushHistory(line);
	for (SUnit* unit : subjectList.m_UnitList)
	{
		unit->m_FighterState = EFighterState::Launching;
	}
	std::cout << "Launching fighters!" << std::endl;
}

void HandleStrike(std::string& line, CGame& game, const SUnitFlagList& subjectList, const SUnitFlagList& targetList)
{
	if (subjectList.m_UnitList.empty())
	{
		std::cout << "You must specify the unit(s) whose fighters should strike." << std::endl;
		return;
	}
	if (targetList.m_UnitList.size() != 1)
	{
		std::cout << "You must provide a single target." << std::endl;
		return;
	}

	for (SUnit* u : subjectList.m_UnitList)
	{
		if (SUnitTypeStats::Get(u->m_Type).m_Fighters < c_Epsilon)
		{
			std::cout << "Unit " << u->m_Id << " is " << UnitTypeToStr(u->m_Type)
				<< ", which do not carry fighters." << std::endl;
			return;
		}
		if (!HasSurvivingFighters(*u))
		{
			std::cout << "Unit " << u->m_Id << " has no fighters left!" << std::endl;
			return;
		}

		if (u->m_FighterState == EFighterState::Docked)
		{
			std::cout << "Unit " << u->m_Id << " must launch fighters first." << std::endl;
			return;
		}

		if (u->m_FighterState == EFighterState::Launching)
		{
			std::cout << "Unit " << u->m_Id << " is still launching its fighters." << std::endl;
			return;
		}

		if (u->m_FighterState == EFighterState::Recalling)
		{
			std::cout << "Unit " << u->m_Id << " is already recalling its fighters to docking bays." << std::endl;
			return;
		}
	}

	const SUnit* const common = FindCommonElement(subjectList.m_UnitList, targetList.m_UnitList, (SUnit*)nullptr);
	if (common != nullptr)
	{
		std::cout << "Unit " << common->m_Id << " cannot strike itself." << std::endl;
		return;
	}

	game.PushHistory(line);

	for (SUnit* unit : subjectList.m_UnitList)
	{
		unit->m_FighterState = EFighterState::Striking;
		unit->m_FighterTarget = targetList.m_UnitList[0]->m_Id;
	}

	SDestroyEvents destroyEvents;
	ApplyStrike(game, subjectList.m_UnitList, *targetList.m_UnitList[0], destroyEvents);
	for (int i = 0; i < subjectList.m_UnitList.size(); ++i)
	{
		SUnit* f = subjectList.m_UnitList[i];

	}
	HandleDestroyEvents(game, destroyEvents);
}

void HandleDefend(std::string& line, CGame& game, const SUnitFlagList& subjectList, const SUnitFlagList& targetList)
{
	if (subjectList.m_UnitList.empty())
	{
		std::cout << "You must specify the unit(s) whose fighters should defend." << std::endl;
		return;
	}

	if (targetList.m_UnitList.size() > 1)
	{
		std::cout << "Cannot defend multiple targets.  Specify no target to defend mother unit." << std::endl;
		return;
	}

	for (SUnit* u : subjectList.m_UnitList)
	{
		if (SUnitTypeStats::Get(u->m_Type).m_Fighters < c_Epsilon)
		{
			std::cout << "Unit " << u->m_Id << " is " << UnitTypeToStr(u->m_Type)
				<< ", which do not carry fighters." << std::endl;
			return;
		}
		if (!HasSurvivingFighters(*u))
		{
			std::cout << "Unit " << u->m_Id << " has no fighters left!" << std::endl;
			return;
		}

		if (u->m_FighterState == EFighterState::Docked)
		{
			std::cout << "Unit " << u->m_Id << " must launch fighters first." << std::endl;
			return;
		}

		if (u->m_FighterState == EFighterState::Launching)
		{
			std::cout << "Unit " << u->m_Id << " is still launching its fighters." << std::endl;
			return;
		}

		if (u->m_FighterState == EFighterState::Recalling)
		{
			std::cout << "Unit " << u->m_Id << " is already recalling its fighters to docking bays." << std::endl;
			return;
		}
	}

	game.PushHistory(line);
	for (SUnit* unit : subjectList.m_UnitList)
	{
		unit->m_FighterState = EFighterState::Defending;
		if (targetList.m_UnitList.size() == 1)
		{
			unit->m_FighterTarget = targetList.m_UnitList[0]->m_Id;
		}
		else
		{
			unit->m_FighterTarget = unit->m_Id;
		}
	}

	if (targetList.m_UnitList.size() == 1)
	{
		std::cout << "Fighters will defend unit " << targetList.m_UnitList[0]->m_Id << "." << std::endl;
	}
	else
	{
		std::cout << "Fighters will defend their motherships." << std::endl;
	}
}

void HandleRecall(std::string& line, CGame& game, const SUnitFlagList& subjectList)
{
	if (subjectList.m_UnitList.empty())
	{
		std::cout << "You must specify the unit(s) to recall fighters." << std::endl;
		return;
	}

	for (SUnit* u : subjectList.m_UnitList)
	{
		if (SUnitTypeStats::Get(u->m_Type).m_Fighters < c_Epsilon)
		{
			std::cout << "Unit " << u->m_Id << " is " << UnitTypeToStr(u->m_Type)
				<< ", which do not carry fighters." << std::endl;
			return;
		}
		if (!HasSurvivingFighters(*u))
		{
			std::cout << "Unit " << u->m_Id << " has no fighters left!" << std::endl;
			return;
		}

		if (u->m_FighterState == EFighterState::Docked)
		{
			std::cout << "Unit " << u->m_Id << " already has fighters docked." << std::endl;
			return;
		}
		if (u->m_FighterState == EFighterState::Launching)
		{
			std::cout << "Unit " << u->m_Id << " is still launching its fighters." << std::endl;
			return;
		}
		if (u->m_FighterState == EFighterState::Recalling)
		{
			std::cout << "Unit " << u->m_Id << " is already recalling its fighters." << std::endl;
			return;
		}
	}

	game.PushHistory(line);
	for (SUnit* unit : subjectList.m_UnitList)
	{
		unit->m_FighterState = EFighterState::Recalling;
	}
	std::cout << "Recalling fighters to docking bays." << std::endl;
}

void HandleEnd(std::string& line, std::stringstream& stream, CGame& game)
{
	int32_t teamId;
	stream >> teamId;
	if (!stream)
	{
		std::cout << "You must specify the team to end turn." << std::endl;
		return;
	}

	STeam* team = game.FindTeam(teamId);
	if (!team)
	{
		std::cout << "Team " << teamId << " does not exist." << std::endl;
		return;
	}

	game.PushHistory(line);
	for (SCommandGroup& cg : team->m_CommandGroups)
	{
		for (SUnit& u : cg.m_Units)
		{
			if (IsFlagSet(u.m_StatusFlags, f_Disrupted))
			{
				TryToRally(u);
			}

			ApplyFighterAttrition(u);
		}
	}

	// Start of turn for next team.
	STeam* followingTeam = game.FindFollowingTeam(teamId);
	assert(followingTeam != nullptr);
	std::cout << "Next is Team " << followingTeam->m_TeamId << std::endl;

	for (SCommandGroup& cg : followingTeam->m_CommandGroups)
	{
		for (SUnit& u : cg.m_Units)
		{
			if (u.m_FighterState == EFighterState::Launching)
			{
				u.m_FighterState = EFighterState::Defending;
				u.m_FighterTarget = u.m_Id;
			}
			else if (u.m_FighterState == EFighterState::Recalling)
			{
				u.m_FighterState = EFighterState::Docked;
			}
		}
	}
}

void HandleUndo(CGame& game)
{
	game.Undo();
}

void HandleLoad(std::stringstream& stream, CGame& game)
{
	std::string filename;
	stream >> filename;

	if (!stream)
	{
		std::cout << "Could not read filename." << std::endl;
		return;
	}

	std::ifstream file(filename);
	if (!file)
	{
		std::cout << "Could not read file: " << filename << std::endl;
		return;
	}

	CGame newGame;
	CFleetLoader loader;
	loader.Load(newGame, file);

	// TODO: Could check that game state is valid.

	game = newGame;
}

void HandleSave(std::stringstream& stream, CGame& game)
{
	std::string filename;
	stream >> filename;

	if (!stream)
	{
		std::cout << "Could not read filename." << std::endl;
		return;
	}

	std::ofstream file(filename);
	if (!file)
	{
		std::cout << "Could not write file: " << filename << std::endl;
		return;
	}

	SaveFleet(game, file);
}

void HandleQuit(CGame& game)
{
	game.m_ShouldQuit = true;
}
