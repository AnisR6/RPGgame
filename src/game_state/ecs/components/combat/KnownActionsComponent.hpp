#pragma once
#include "game_state/ecs/entity/Entity.hpp"

#include <vector>

struct KnownActionsComponent {
	std::vector<Engine::ECS::Entity> knownActionEntities;
};