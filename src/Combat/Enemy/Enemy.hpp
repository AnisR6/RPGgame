#pragma once
#include <vector>

#include "Combat/Structure/Stats.hpp"
#include "Combat/Structure/CombatSkill.hpp"
#include "Combat/Structure/Spell.hpp"

class Enemy {
	private:
		Stats stats;
		std::vector<CombatSkill> combatSkills;
		std::vector<Spell> spells;

	public:

};