#include "ecs/shared/prefabs/Player.hpp"

#include "ecs/overworld/components/InputComponent.hpp"

namespace Prefabs {
    Engine::ECS::Entity CreateOverworldPlayer(Engine::ECS::World& world, sf::Vector2f startPosition,
                                              const sf::Texture* spriteSheet) {
        const auto player = world.CreateEntity();
        const CollisionComponent collision;

        world.EmplaceComponent<PositionComponent>(player, PositionComponent{startPosition});
        world.EmplaceComponent<CollisionComponent>(player, collision);
        world.EmplaceComponent<MovementComponent>(player);
        world.EmplaceComponent<InputComponent>(player);

        auto& sprite = world.EmplaceComponent<SpriteComponent>(player);
        sprite.spriteSheet = spriteSheet;

        sprite.drawOffset =
            sf::Vector2f((collision.boxSizeInPixels - sprite.frameWidthInPixels) / 2.0f,
                         collision.boxSizeInPixels - sprite.frameHeightInPixels);

        return player;
    }
}
