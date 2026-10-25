#include "Combat/Party/PartyMember.hpp"

const Stats &PartyMember::getStats() const
{
	return stats;
};

const std::vector<Spell> &PartyMember::getSpells() const
{
	return spells;
}

const std::vector<CombatSkill> &PartyMember::getCombatSkills() const
{
	return combatSkills;
}
