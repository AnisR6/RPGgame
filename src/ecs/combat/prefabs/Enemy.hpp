#pragma once
#include "ecs/combat/components/StatsComponent.hpp"
#include "ecs/shared/components/EnemyComponent.hpp"
#include "ecs/shared/components/NameComponent.hpp"
#include "ecs/shared/components/SpriteComponent.hpp"
#include "ecs/combat/components/EnemyAIComponent.hpp"
#include "ecs/combat/components/KnownActionsComponent.hpp"
#include "ecs/core/entity/Entity.hpp"
#include "ecs/core/world/World.hpp"

namespace Prefabs {
    struct EnemyBlueprint {
            NameComponent nameComponent;
            EnemyComponent enemyComponent;
            StatsComponent statsComponent;
            KnownActionsComponent knownActionsComponent;
            EnemyAIComponent enemyAIComponent{};
            SpriteComponent spriteComponent;
    };

    inline Engine::ECS::Entity CreateEnemy(Engine::ECS::World& world, EnemyBlueprint&& blueprint) {
        const auto entity = world.CreateEntity();
        world.EmplaceComponent<NameComponent>(entity, std::move(blueprint.nameComponent));
        world.EmplaceComponent<StatsComponent>(entity, blueprint.statsComponent);
        world.EmplaceComponent<EnemyComponent>(entity, blueprint.enemyComponent);
        world.EmplaceComponent<KnownActionsComponent>(entity, blueprint.knownActionsComponent);
        world.EmplaceComponent<EnemyAIComponent>(entity, blueprint.enemyAIComponent);
        world.EmplaceComponent<SpriteComponent>(entity, std::move(blueprint.spriteComponent));
        return entity;
    }
}
