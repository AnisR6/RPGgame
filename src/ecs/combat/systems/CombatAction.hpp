#pragma once
#include "ecs/core/entity/Entity.hpp"
#include <vector>

struct CombatAction {
        Engine::ECS::Entity actorEntity;
        std::vector<Engine::ECS::Entity> targetEntities;
        Engine::ECS::Entity actionEntity;
};
