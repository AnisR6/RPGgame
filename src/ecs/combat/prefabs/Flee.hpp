#pragma once
#include "ecs/shared/components/NameComponent.hpp"
#include "ecs/combat/components/ActionComponent.hpp"
#include "ecs/combat/components/FleeComponent.hpp"
#include "ecs/core/world/World.hpp"

namespace Prefabs {
    struct FleeBlueprint {
            ActionComponent actionComponent;
            FleeComponent fleeComponent;
            NameComponent nameComponent;
            TargetScopeComponent targetScopeComponent{TargetScope::Self};
    };

    inline Engine::ECS::Entity CreateFlee(Engine::ECS::World& world, FleeBlueprint&& blueprint) {
        const auto entity = world.CreateEntity();
        world.EmplaceComponent<ActionComponent>(entity, blueprint.actionComponent);
        world.EmplaceComponent<FleeComponent>(entity, blueprint.fleeComponent);
        world.EmplaceComponent<NameComponent>(entity, blueprint.nameComponent);
        world.EmplaceComponent<TargetScopeComponent>(entity, blueprint.targetScopeComponent);
        return entity;
    }
}
