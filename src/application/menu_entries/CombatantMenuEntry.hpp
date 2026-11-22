#pragma once
#include "game_state/ecs/entity/Entity.hpp"

#include <string>

struct CombatantMenuEntry {
	Engine::ECS::Entity entity;
	std::string name;
	int maxHP;
	int currHP;
	int maxMP;
	int currMP;
	bool isDead = false;
};