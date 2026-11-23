#pragma once
#include "ecs/entity/Entity.hpp"

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