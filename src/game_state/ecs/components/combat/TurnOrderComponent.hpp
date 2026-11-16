#pragma once
#include "game_state/ecs/entity/Entity.hpp"

#include <vector>

struct TurnOrderComponent {
	std::vector<Engine::ECS::Entity> playerPartyEntities;
	std::vector<Engine::ECS::Entity> enemyEntities;
	size_t currentActorIndex = 0;
};
