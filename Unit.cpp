#include "Unit.h"

#include "Fighters.h"
#include "Formation.h"
#include "Quality.h"
#include "UnitType.h"
#include <sstream>
#include <iomanip>
#include <iostream>

std::string GetUnitStatusString(const SUnit& unit)
{
	std::stringstream out;

	out << std::fixed << std::setprecision(2);

	out << unit.m_Id << " "
		<< UnitTypeToStr(unit.m_Type) << " "
		<< QualityToStr(unit.m_Quality) << " "
		<< FormationToStr(unit.m_Formation) << " "
		<< "strength " << unit.m_Strength << " "
		<< "morale " << unit.m_Morale << " ";

	if (SUnitTypeStats::Get(unit.m_Type).m_Fighters > 0)
	{
		out << "fighters " << unit.m_FighterStrength << " "
			<< FighterStateToStr(unit.m_FighterState) << " ";
		if (unit.m_FighterState == EFighterState::Defending)
		{
			out << unit.m_FighterTarget << " ";
		}
	}

	if (IsFlagSet(unit.m_StatusFlags, f_Disrupted))
		out << "disrupted ";
	if (IsFlagSet(unit.m_StatusFlags, f_ReceivedFire))
		out << "received ";
	if (IsFlagSet(unit.m_StatusFlags, f_Acted))
		out << "acted ";
	if (IsFlagSet(unit.m_StatusFlags, f_Flagship))
		out << "flagship ";
	if (IsFlagSet(unit.m_StatusFlags, f_FlagshipDamaged))
		out << "flagdmg ";

	return out.str();
}
