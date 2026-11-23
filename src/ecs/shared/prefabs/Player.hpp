#pragma once

#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>

#include "ecs/combat/components/KnownActionsComponent.hpp"
#include "ecs/combat/components/StatsComponent.hpp"
#include "ecs/overworld/components/CollisionComponent.hpp"
#include "ecs/overworld/components/MovementComponent.hpp"
#include "ecs/shared/components/NameComponent.hpp"
#include "ecs/shared/components/PartyMemberComponent.hpp"
#include "ecs/overworld/components/PositionComponent.hpp"
#include "ecs/shared/components/SpriteComponent.hpp"
#include "ecs/core/entity/Entity.hpp"
#include "ecs/core/world/World.hpp"

namespace Prefabs {
    struct PlayerBlueprint {
            NameComponent nameComponent;
            PositionComponent positionComponent;
            CollisionComponent collisionComponent;
            MovementComponent movementComponent;
            SpriteComponent spriteComponent;
            PartyMemberComponent partyMemberComponent;
            StatsComponent statsComponent;
            KnownActionsComponent knownActionsComponent;
    };

    inline Engine::ECS::Entity CreatePlayer(Engine::ECS::World& world,
                                            PlayerBlueprint&& blueprint) {
        const auto entity = world.CreateEntity();
        world.EmplaceComponent<NameComponent>(entity, std::move(blueprint.nameComponent));
        world.EmplaceComponent<PositionComponent>(entity, blueprint.positionComponent);
        world.EmplaceComponent<CollisionComponent>(entity, blueprint.collisionComponent);
        world.EmplaceComponent<MovementComponent>(entity, blueprint.movementComponent);
        world.EmplaceComponent<SpriteComponent>(entity, std::move(blueprint.spriteComponent));
        world.EmplaceComponent<PartyMemberComponent>(entity, blueprint.partyMemberComponent);
        world.EmplaceComponent<StatsComponent>(entity, blueprint.statsComponent);
        world.EmplaceComponent<KnownActionsComponent>(entity, blueprint.knownActionsComponent);
        return entity;
    }

    Engine::ECS::Entity CreateOverworldPlayer(Engine::ECS::World& world, sf::Vector2f startPosition,
                                              const sf::Texture* spriteSheet);
}
