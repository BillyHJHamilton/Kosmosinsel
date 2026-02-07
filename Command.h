#pragma once

/*
Command format:
(command) (subject) (target)

Subject:
The subject of a command is normally or more units, separated by commas: 100,110
When firing or attacking, the subject is the attacking unit(s).
A number may be followed by "+" to indicate an advantage: 100,110+,120
When firing, this "+" indicates enfilading fire.
When attacking, this "+" indicates a flanking attack.

Commands:
near		fire at near range
far		    fire at far range
attack		run a close combat and apply all results
form		change formation
stat		report status
test		test morale and become disrupted on failure

Target:
near, far   A single unit to fire on.
            Special: In sphere formation, multiple targets may be given.
attack      The defending unit or units.  May use + to indicate defender flanks attacker.
form        The name of the formation: line, column, sphere, skirmish, or rout.
stat, test  (No target)

Examples of valid commands:
100 near 200
100,110 far 200
100,110,120+ attack 200,210
100 attack 200,210+,220+
100 form line
100,110 stat
120 test

Commands with no subject:
undo

Possible commands to add:
launch      deploy fighters
recall      recall fighters
swarm       attack with fighters
turn        end turn, try to rally units
assign      set which player controls units
attach      attach commander to a unit (and detach from any other unit)
 - attached commander risks being hit, but provides combat advantage

Not sure what numbers to use for fighters - Could use a convention like add 1, or add f or something.

*/

#include "Common.h"
#include <string>
#include <sstream>
#include <vector>

enum class EFireRange : uint8_t;
enum class EFormation : uint8_t;
class CGame;
struct SUnit;

enum class ECommand
{
    Near,       // Fire at near range
    Far,        // Fire at far range
    Attack,     // Engage in close battle
    Form,       // Change formation
    Stat,       // Print stats
    Test,       // Test morale and become disrupted on failure

    Launch,     // Launch docked fighters
    Strike,     // Fighters attack enemy unit
    Defend,     // Fighters defend mother unit  (what about defending another allied unit?)
    Recall,     // Fighters return to docking bays

    End,        // End turn and attempt to rally disrupted units

    Undo,
    Load,
    Save,
    Quit,

    Count,
    None = Count
};

struct SUnitFlagList
{
    std::vector<SUnit*> m_UnitList;
    std::vector<Flag> m_FlagList;
};

void HandleCommand(CGame& game);

SUnitFlagList TryParseUnitFlagList(std::stringstream& stream, CGame& game);
ECommand TryParseCommand(std::stringstream& stream);
ECommand StrToCommand(std::string word);
EFormation TryParseFormation(std::stringstream& stream);

bool AreTeamsDistinct(const std::vector<SUnit*>& a, const std::vector<SUnit*>& b);

void HandleFire(std::string& line, CGame& game, const SUnitFlagList& subjectList, const SUnitFlagList& targetList, EFireRange range);
void HandleAttack(std::string& line, CGame& game, const SUnitFlagList& subjectList, const SUnitFlagList& targetList);
void HandleForm(std::string& line, CGame& game, const SUnitFlagList& subjectList, EFormation formation);
void HandleStat(std::string& line, CGame& game, const SUnitFlagList& subjectList);
void HandleTest(std::string& line, CGame& game, const SUnitFlagList& subjectList);

void HandleLaunch(std::string& line, CGame& game, const SUnitFlagList& subjectList);
void HandleStrike(std::string& line, CGame& game, const SUnitFlagList& subjectList, const SUnitFlagList& targetList);
void HandleDefend(std::string& line, CGame& game, const SUnitFlagList& subjectList, const SUnitFlagList& targetList);
void HandleRecall(std::string& line, CGame& game, const SUnitFlagList& subjectList);

void HandleEnd(std::string& line, std::stringstream& stream, CGame& game);

void HandleUndo(CGame& game);
void HandleLoad(std::stringstream& stream, CGame& game);
void HandleSave(std::stringstream& stream, CGame& game);
void HandleQuit(CGame& game);
