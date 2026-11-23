#pragma once
#include "ecs/shared/components/NameComponent.hpp"
#include "ecs/combat/components/ActionComponent.hpp"
#include "ecs/combat/components/ActionEffectsComponent.hpp"
#include "ecs/combat/components/BaseAttackComponent.hpp"
#include "ecs/combat/components/TargetScopeComponent.hpp"
#include "ecs/core/world/World.hpp"

namespace Prefabs {
    struct BaseAttackBlueprint {
            ActionComponent actionComponent;
            BaseAttackComponent baseAttackComponent;
            NameComponent nameComponent;
            TargetScopeComponent targetScopeComponent;
            ActionEffectsComponent actionEffectsComponent;
    };

    inline Engine::ECS::Entity CreateBaseAttack(Engine::ECS::World& world,
                                                BaseAttackBlueprint&& blueprint) {
        const auto entity = world.CreateEntity();
        world.EmplaceComponent<ActionComponent>(entity, blueprint.actionComponent);
        world.EmplaceComponent<BaseAttackComponent>(entity, blueprint.baseAttackComponent);
        world.EmplaceComponent<NameComponent>(entity, blueprint.nameComponent);
        world.EmplaceComponent<TargetScopeComponent>(entity, blueprint.targetScopeComponent);
        world.EmplaceComponent<ActionEffectsComponent>(entity, blueprint.actionEffectsComponent);
        return entity;
    }
}
