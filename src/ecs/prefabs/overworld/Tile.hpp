
#pragma once

#include "ecs/components/CollisionComponent.hpp"
#include "ecs/components/LayerComponent.hpp"
#include "ecs/components/PositionComponent.hpp"
#include "ecs/components/SpriteComponent.hpp"
#include "ecs/entity/Entity.hpp"
#include "ecs/system/SystemContext.hpp"

#include <utility>

namespace Prefabs {

    // -----------------------------------------------------------------------------
    // Tile Blueprint
    // -----------------------------------------------------------------------------
    struct TileBlueprint {
            PositionComponent positionComponent;
            SpriteComponent spriteComponent;
            LayerComponent layerComponent;
            CollisionComponent collisionComponent;
    };

    // -----------------------------------------------------------------------------
    // Tile Prefab
    // -----------------------------------------------------------------------------
    inline Engine::ECS::Entity CreateTilePrefab(Engine::ECS::SystemContext& context,
                                                TileBlueprint&& blueprint) {
        const Engine::ECS::Entity entity = context.CreateEntity();

        context.EmplaceComponent<PositionComponent>(entity, blueprint.positionComponent);

        context.EmplaceComponent<SpriteComponent>(entity, std::move(blueprint.spriteComponent));

        context.EmplaceComponent<LayerComponent>(entity, blueprint.layerComponent);

        return entity;
    }

    // -----------------------------------------------------------------------------
    // Collidable Tile Prefab
    // -----------------------------------------------------------------------------
    inline Engine::ECS::Entity CreateCollidableTilePrefab(Engine::ECS::SystemContext& context,
                                                          TileBlueprint&& blueprint) {
        const Engine::ECS::Entity entity = CreateTilePrefab(context, std::move(blueprint));

        context.EmplaceComponent<CollisionComponent>(entity, blueprint.collisionComponent);

        return entity;
    }

} // namespace Prefabs
