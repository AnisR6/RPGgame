#pragma once
#include <vector>

#include "Ability/Combatable.hpp"
#include "Combat/Structure/CombatSkill.hpp"
#include "Combat/Structure/Spell.hpp"
#include "Combat/Structure/Stats.hpp"

class Enemy : public Combatable {
  private:
	Stats stats;
	std::vector<CombatSkill> combatSkills;
	std::vector<Spell> spells;

  public:
	const Stats &getStats() const override;
	const std::vector<Spell> &getSpells() const override;
	const std::vector<CombatSkill> &getCombatSkills() const override;
};