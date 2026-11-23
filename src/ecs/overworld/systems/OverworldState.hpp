#pragma once

#include "ecs/core/entity/Entity.hpp"

#include <optional>

struct OverworldState {
        std::optional<Engine::ECS::Entity> encounteredEnemy;
};
