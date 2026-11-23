#pragma once
#include "ecs/components/combat/TargetScopeComponent.hpp"
#include "ecs/entity/Entity.hpp"

#include <string>

struct BattleActionMenuEntry {
	Engine::ECS::Entity entity;
	std::string name;
	TargetScope targetScope;
};