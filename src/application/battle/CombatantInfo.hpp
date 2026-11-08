#pragma once
#include "game_logic/systems/game/battle/battle_system/Structure/CombatSkill.hpp"
#include "game_logic/systems/game/battle/battle_system/Structure/Spell.hpp"

#include <string>
#include <vector>

struct CombatantInfo {
	int id;
	std::string name;
	int currHP;
	int maxHP;
	int currMP;
	int maxMP;
	std::vector<Spell> spells;
	std::vector<CombatSkill> skills;
	// inventory
};