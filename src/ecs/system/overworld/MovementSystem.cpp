#include "ecs/system/overworld/MovementSystem.hpp"

#include "ecs/components/CollisionComponent.hpp"
#include "ecs/components/InputComponent.hpp"
#include "ecs/components/MovementComponent.hpp"
#include "ecs/components/PositionComponent.hpp"
#include "ecs/query/Query.hpp"
#include "ecs/system/overworld/CollisionSystem.hpp"

#include <cmath>

namespace {

    // -----------------------------------------------------------------------------
    // Movement Direction
    // -----------------------------------------------------------------------------
    sf::Vector2f NormalizeMovementDirection(sf::Vector2f movementDirection) {
        const float movementDirectionLength = std::sqrt(movementDirection.x * movementDirection.x +
                                                        movementDirection.y * movementDirection.y);

        if (movementDirectionLength > 1.0f) {
            movementDirection /= movementDirectionLength;
        }

        return movementDirection;
    }

    // -----------------------------------------------------------------------------
    // Movement Distance
    // -----------------------------------------------------------------------------
    sf::Vector2f CalculateMovementStep(const sf::Vector2f& movementDirection,
                                       float movementSpeedInPixelsPerSecond, float deltaTime) {
        return movementDirection * movementSpeedInPixelsPerSecond * deltaTime;
    }

    // -----------------------------------------------------------------------------
    // Horizontal Movement
    // -----------------------------------------------------------------------------
    void ApplyHorizontalMovement(const TileMap& tileMap, sf::Vector2f& entityPosition,
                                 const sf::Vector2f& movementStep,
                                 const CollisionComponent& collisionComponent) {
        const sf::Vector2f horizontalPosition(entityPosition.x + movementStep.x, entityPosition.y);

        if (!CollisionSystem::isBlocked(tileMap, horizontalPosition, collisionComponent)) {
            entityPosition.x += movementStep.x;
        }
    }

    // -----------------------------------------------------------------------------
    // Vertical Movement
    // -----------------------------------------------------------------------------
    void ApplyVerticalMovement(const TileMap& tileMap, sf::Vector2f& entityPosition,
                               const sf::Vector2f& movementStep,
                               const CollisionComponent& collisionComponent) {
        const sf::Vector2f verticalPosition(entityPosition.x, entityPosition.y + movementStep.y);

        if (!CollisionSystem::isBlocked(tileMap, verticalPosition, collisionComponent)) {
            entityPosition.y += movementStep.y;
        }
    }

    // -----------------------------------------------------------------------------
    // Entity Movement
    // -----------------------------------------------------------------------------
    void ApplyEntityMovement(const TileMap& tileMap, sf::Vector2f& entityPosition,
                             const sf::Vector2f& movementStep,
                             const CollisionComponent& collisionComponent) {
        ApplyHorizontalMovement(tileMap, entityPosition, movementStep, collisionComponent);

        ApplyVerticalMovement(tileMap, entityPosition, movementStep, collisionComponent);
    }

} // namespace

namespace Engine::ECS {

    // -----------------------------------------------------------------------------
    // Construction
    // -----------------------------------------------------------------------------
    MovementSystem::MovementSystem(const TileMap& tileMap)
        : tileMap(tileMap) {}

    // -----------------------------------------------------------------------------
    // System Update
    // -----------------------------------------------------------------------------
    void MovementSystem::Update(SystemContext& context, float deltaTime) {
        auto movableEntities = context.CreateQuery<
            Including<InputComponent, PositionComponent, MovementComponent, CollisionComponent>>();

        movableEntities.ForEach(
            [&](Entity, InputComponent& inputComponent, PositionComponent& positionComponent,
                MovementComponent& movementComponent, CollisionComponent& collisionComponent) {
                const sf::Vector2f movementDirection =
                    NormalizeMovementDirection(inputComponent.walkDirection);

                const sf::Vector2f movementStep = CalculateMovementStep(
                    movementDirection, movementComponent.speedInPixelsPerSecond, deltaTime);

                ApplyEntityMovement(tileMap, positionComponent.position, movementStep,
                                    collisionComponent);
            });
    }

} // namespace Engine::ECS