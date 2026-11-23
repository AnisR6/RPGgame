#pragma once
#include "ecs/shared/components/NameComponent.hpp"
#include "ecs/combat/components/ActionComponent.hpp"
#include "ecs/combat/components/ActionEffectsComponent.hpp"
#include "ecs/combat/components/CombatSkillComponent.hpp"
#include "ecs/combat/components/TargetScopeComponent.hpp"
#include "ecs/core/world/World.hpp"

namespace Prefabs {
    struct CombatSkillBlueprint {
            ActionComponent actionComponent;
            CombatSkillComponent combatSkillComponent;
            NameComponent nameComponent;
            TargetScopeComponent targetScopeComponent;
            ActionEffectsComponent actionEffectsComponent;
    };

    inline Engine::ECS::Entity CreateCombatSkill(Engine::ECS::World& world,
                                                 CombatSkillBlueprint&& blueprint) {
        const auto entity = world.CreateEntity();
        world.EmplaceComponent<ActionComponent>(entity, blueprint.actionComponent);
        world.EmplaceComponent<CombatSkillComponent>(entity, blueprint.combatSkillComponent);
        world.EmplaceComponent<NameComponent>(entity, blueprint.nameComponent);
        world.EmplaceComponent<TargetScopeComponent>(entity, blueprint.targetScopeComponent);
        world.EmplaceComponent<ActionEffectsComponent>(entity, blueprint.actionEffectsComponent);
        return entity;
    }
}
