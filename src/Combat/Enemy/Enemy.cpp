#include "Enemy.hpp"

#include "Combat/Structure/CombatAction.hpp"

const Stats& Enemy::getStats() const{
	return stats;
}

const std::vector<Spell>& Enemy::getSpells() const{
	return spells;
}

const std::vector<CombatSkill>& Enemy::getCombatSkills() const{
	return combatSkills;
}