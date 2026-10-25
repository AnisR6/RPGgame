#pragma once
#include <vector>

#include "Combat/Structure/Stats.hpp"
#include "Combat/Structure/CombatSkill.hpp"
#include "Combat/Structure/Spell.hpp"
#include "Ability/Combatable.hpp"

class Enemy : public Combatable {
	private:
		Stats stats;
		std::vector<CombatSkill> combatSkills;
		std::vector<Spell> spells;

	public:
		const Stats& getStats() const override;
		const std::vector<Spell>& getSpells() const override;
		const std::vector<CombatSkill>& getCombatSkills() const override;


};