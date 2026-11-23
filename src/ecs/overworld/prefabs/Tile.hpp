#pragma once

#include "ecs/overworld/components/CollisionComponent.hpp"
#include "ecs/overworld/components/LayerComponent.hpp"
#include "ecs/overworld/components/PositionComponent.hpp"
#include "ecs/shared/components/SpriteComponent.hpp"
#include "ecs/core/entity/Entity.hpp"
#include "ecs/core/world/World.hpp"

#include <utility>

namespace Prefabs {
    struct TileBlueprint {
            PositionComponent positionComponent;
            SpriteComponent spriteComponent;
            LayerComponent layerComponent;
            CollisionComponent collisionComponent;
    };

    inline Engine::ECS::Entity CreateTilePrefab(Engine::ECS::World& world,
                                                TileBlueprint&& blueprint) {
        const Engine::ECS::Entity entity = world.CreateEntity();

        world.EmplaceComponent<PositionComponent>(entity, blueprint.positionComponent);

        world.EmplaceComponent<SpriteComponent>(entity, std::move(blueprint.spriteComponent));

        world.EmplaceComponent<LayerComponent>(entity, blueprint.layerComponent);

        return entity;
    }

    inline Engine::ECS::Entity CreateCollidableTilePrefab(Engine::ECS::World& world,
                                                          TileBlueprint&& blueprint) {
        const CollisionComponent collision = blueprint.collisionComponent;
        const Engine::ECS::Entity entity = CreateTilePrefab(world, std::move(blueprint));

        world.EmplaceComponent<CollisionComponent>(entity, collision);

        return entity;
    }
}
