#pragma once
#include <vector>

#include "game_logic/systems/game/battle/battle_system/Structure/CombatSkill.hpp"
#include "game_logic/systems/game/battle/battle_system/Structure/Spell.hpp"
#include "game_logic/systems/game/battle/battle_system/Structure/Stats.hpp"
#include "game_state/ecs/capabilities/Combatable.hpp"

class PartyMember : public Combatable {
  private:
	Stats stats;
	std::vector<CombatSkill> combatSkills;
	std::vector<Spell> spells;

  public:
	PartyMember(const Stats &stats, const std::vector<CombatSkill> &combatSkills, const std::vector<Spell> &spells)
	    : stats(stats), combatSkills(combatSkills), spells(spells)
	{
	}

	const Stats &getStats() const override;
	const std::vector<Spell> &getSpells() const override;
	const std::vector<CombatSkill> &getCombatSkills() const override;
};
