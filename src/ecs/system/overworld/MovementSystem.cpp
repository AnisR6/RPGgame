#include "ecs/system/overworld/MovementSystem.hpp"

#include "ecs/components/overworld/CollisionComponent.hpp"
#include "ecs/components/overworld/InputComponent.hpp"
#include "ecs/components/overworld/MovementComponent.hpp"
#include "ecs/components/overworld/PositionComponent.hpp"
#include "ecs/query/Query.hpp"
#include "ecs/system/overworld/CollisionSystem.hpp"

#include <cmath>

namespace Engine::ECS {

    MovementSystem::MovementSystem(const TileMap& tileMap) : tileMap(tileMap) {}

    void MovementSystem::Update(SystemContext& context, float secondsSinceLastFrame)
    {
        // InputComponent first: a query loops over the entities of its FIRST component (only the player has one)
        auto movers = context.CreateQuery<Including<InputComponent, PositionComponent, MovementComponent, CollisionComponent>>();

        movers.ForEach([&](Entity, InputComponent& input, PositionComponent& position, MovementComponent& movement,
                           CollisionComponent& collision) {
            // diagonal (1, 1) is longer than 1: make it 1, so walking diagonally is not faster
            sf::Vector2f direction = input.walkDirection;
            const float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
            if (length > 1.0f) {
                direction /= length;
            }
            const sf::Vector2f step = direction * movement.speedInPixelsPerSecond * secondsSinceLastFrame;

            // first x, then y: a blocked side is skipped, so the player slides along walls
            if (!CollisionSystem::isBlocked(tileMap, { position.position.x + step.x, position.position.y }, collision)) {
                position.position.x += step.x;
            }
            if (!CollisionSystem::isBlocked(tileMap, { position.position.x, position.position.y + step.y }, collision)) {
                position.position.y += step.y;
            }
        });
    }

}
