#include "game_logic/systems/game/battle/battle_system/Party/PartyMember.hpp"

const Stats &PartyMember::getStats() const
{
	return stats;
};

Stats &PartyMember::getStats()
{
	return stats;
}

const std::vector<Spell> &PartyMember::getSpells() const
{
	return spells;
}

const std::vector<CombatSkill> &PartyMember::getCombatSkills() const
{
	return combatSkills;
}

std::string PartyMember::getName() const
{
	return name;
}
