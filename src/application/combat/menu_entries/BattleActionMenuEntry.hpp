#pragma once
#include "ecs/combat/components/TargetScopeComponent.hpp"
#include "ecs/core/entity/Entity.hpp"

#include <string>

struct BattleActionMenuEntry {
        Engine::ECS::Entity entity;
        std::string name;
        TargetScope targetScope;
};
