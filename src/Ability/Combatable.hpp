#pragma once
#include <algorithm>
#include <vector>

#include "../Combat/Structure/CombatSkill.hpp"
#include "../Combat/Structure/Spell.hpp"
#include "../Combat/Structure/Stats.hpp"

class Combatable {
  public:
	virtual Stats &getStats() = 0;
	virtual const Stats &getStats() const = 0;
	virtual const std::vector<Spell> &getSpells() const = 0;
	virtual const std::vector<CombatSkill> &getCombatSkills() const = 0;
	virtual ~Combatable() = default;

	bool isAlive() const { return getStats().currHP > 0; }

	void takeDamage(const unsigned amount)
	{
		getStats().currHP = getStats().currHP < amount ? 0 : getStats().currHP - amount;
	}

	void heal(const unsigned healValue)
	{
		getStats().currHP = std::min(getStats().currHP + healValue, getStats().maxHP);
	}
};
