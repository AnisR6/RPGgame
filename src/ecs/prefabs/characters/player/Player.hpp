#pragma once

#include <SFML/Graphics.hpp>
#include <string>

#include "ecs/components/combat/KnownActionsComponent.hpp"
#include "ecs/components/combat/StatsComponent.hpp"
#include "ecs/components/overworld/CollisionComponent.hpp"
#include "ecs/components/overworld/MovementComponent.hpp"
#include "ecs/components/overworld/NameComponent.hpp"
#include "ecs/components/overworld/PartyMemberComponent.hpp"
#include "ecs/components/overworld/PositionComponent.hpp"
#include "ecs/components/overworld/SpriteComponent.hpp"
#include "ecs/entity/Entity.hpp"
#include "ecs/world/World.hpp"

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

    inline Engine::ECS::Entity createPlayer(Engine::ECS::World& world,
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

    class Player {
        public:
            Player(sf::Vector2f startPosition, const std::string& spriteSheetPath);

            // The player's data, as components
            PositionComponent position;
            CollisionComponent collision;
            MovementComponent movement;
            SpriteComponent sprite;

        private:
            sf::Texture spriteTexture;
    };
} // namespace Prefabs
