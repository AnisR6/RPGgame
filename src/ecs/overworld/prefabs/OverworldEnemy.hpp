#pragma once

#include "ecs/core/entity/Entity.hpp"
#include "ecs/core/world/World.hpp"
#include "ecs/overworld/components/CollisionComponent.hpp"
#include "ecs/overworld/components/EncounterComponent.hpp"
#include "ecs/overworld/components/PositionComponent.hpp"
#include "ecs/shared/components/SpriteComponent.hpp"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>

#include <string>

namespace Prefabs {
    inline Engine::ECS::Entity CreateOverworldEnemy(Engine::ECS::World& world, std::string name,
                                                    const sf::Vector2f standingPoint,
                                                    const sf::Texture* spriteSheet,
                                                    const sf::Color tint) {
        const auto enemy = world.CreateEntity();
        const CollisionComponent collision;
        const float halfBox = collision.boxSizeInPixels / 2.0f;

        world.EmplaceComponent<PositionComponent>(
            enemy, PositionComponent{standingPoint - sf::Vector2f(halfBox, halfBox)});
        world.EmplaceComponent<CollisionComponent>(enemy, collision);
        world.EmplaceComponent<EncounterComponent>(enemy, EncounterComponent{std::move(name)});

        auto& sprite = world.EmplaceComponent<SpriteComponent>(enemy);
        sprite.spriteSheet = spriteSheet;
        sprite.tint = tint;
        sprite.drawOffset =
            sf::Vector2f((collision.boxSizeInPixels - sprite.frameWidthInPixels) / 2.0f,
                         collision.boxSizeInPixels - sprite.frameHeightInPixels);

        return enemy;
    }
}
