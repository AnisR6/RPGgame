#pragma once
#include <vector>

#include "game_logic/systems/game/battle/battle_system/Structure/CombatSkill.hpp"
#include "game_logic/systems/game/battle/battle_system/Structure/Spell.hpp"
#include "game_logic/systems/game/battle/battle_system/Structure/Stats.hpp"
#include "game_state/ecs/capabilities/Combatable.hpp"

class Enemy : public Combatable {
  private:
	Stats stats;
	std::vector<CombatSkill> combatSkills;
	std::vector<Spell> spells;

  public:
	[[nodiscard]] const Stats &getStats() const override;
	[[nodiscard]] const std::vector<Spell> &getSpells() const override;
	[[nodiscard]] const std::vector<CombatSkill> &getCombatSkills() const override;
};