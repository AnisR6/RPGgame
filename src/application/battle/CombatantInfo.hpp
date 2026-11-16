#pragma once

#include "game_state/ecs/entity/Entity.hpp"
#include "game_state/ecs/system/combat/Structure/CombatSkill.hpp"
#include "game_state/ecs/system/combat/Structure/Spell.hpp"

#include <string>
#include <vector>

struct CombatantInfo {
	Engine::ECS::Entity entity;
	std::string name;
	int currHP;
	int maxHP;
	int currMP;
	int maxMP;
	std::vector<Spell> spells;
	std::vector<CombatSkill> skills;
	// inventory
};