#pragma once
#include "game_state/ecs/components/combat/TargetScopeComponent.hpp"
#include "game_state/ecs/entity/Entity.hpp"

#include <string>

struct BattleActionMenuEntry {
	Engine::ECS::Entity entity;
	std::string name;
	TargetScope targetScope;
};