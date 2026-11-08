#pragma once
#include <vector>

#include "game_logic/systems/game/battle/battle_system/Structure/CombatSkill.hpp"
#include "game_logic/systems/game/battle/battle_system/Structure/Spell.hpp"
#include "game_logic/systems/game/battle/battle_system/Structure/Stats.hpp"
#include "game_state/ecs/capabilities/Combatable.hpp"

class PartyMember : public Combatable {
  protected:
	Stats stats;
	std::string name;
	std::vector<CombatSkill> combatSkills;
	std::vector<Spell> spells;

  public:
	PartyMember(const Stats &stats, const std::vector<CombatSkill> &combatSkills, const std::vector<Spell> &spells)
	    : stats(stats), combatSkills(combatSkills), spells(spells)
	{
	}

	[[nodiscard]] const Stats &getStats() const override;
	Stats &getStats() override;
	[[nodiscard]] const std::vector<Spell> &getSpells() const override;
	[[nodiscard]] const std::vector<CombatSkill> &getCombatSkills() const override;
	[[nodiscard]] std::string getName() const;
};
