#pragma once
#include "ecs/entity/Entity.hpp"

#include <vector>

struct KnownActionsComponent {
	std::vector<Engine::ECS::Entity> knownActionEntities;
};