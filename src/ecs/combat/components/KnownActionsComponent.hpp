#pragma once
#include "ecs/core/entity/Entity.hpp"

#include <vector>

struct KnownActionsComponent {
        std::vector<Engine::ECS::Entity> knownActionEntities;
};
