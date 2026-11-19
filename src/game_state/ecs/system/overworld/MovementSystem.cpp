#include "game_state/ecs/system/overworld/MovementSystem.hpp"

#include "game_state/ecs/components/CollisionComponent.hpp"
#include "game_state/ecs/components/InputComponent.hpp"
#include "game_state/ecs/components/MovementComponent.hpp"
#include "game_state/ecs/components/PositionComponent.hpp"
#include "game_state/ecs/query/Query.hpp"
#include "game_state/ecs/system/overworld/CollisionSystem.hpp"

namespace Engine::ECS {

    void MovementSystem::Update(SystemContext& context, float secondsSinceLastFrame)
    {
        auto movers = context.CreateQuery<
            Including<PositionComponent, MovementComponent, CollisionComponent, InputComponent>>();

        movers.ForEach([&](Entity, PositionComponent& position, MovementComponent& movement,
                           CollisionComponent& collision, InputComponent& input) {
            const sf::Vector2f step =
                input.walkDirection * movement.speedInPixelsPerSecond * secondsSinceLastFrame;

            // First x, then y. A blocked axis is skipped, so the entity slides along walls.
            const sf::Vector2f positionAfterXMove(position.position.x + step.x, position.position.y);
            if (!CollisionSystem::isBlocked(context, positionAfterXMove, collision)) {
                position.position = positionAfterXMove;
            }

            const sf::Vector2f positionAfterYMove(position.position.x, position.position.y + step.y);
            if (!CollisionSystem::isBlocked(context, positionAfterYMove, collision)) {
                position.position = positionAfterYMove;
            }
        });
    }

} // namespace Engine::ECS
