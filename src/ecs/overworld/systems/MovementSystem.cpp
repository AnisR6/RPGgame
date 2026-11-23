#include "ecs/overworld/systems/MovementSystem.hpp"

#include "ecs/overworld/components/CollisionComponent.hpp"
#include "ecs/overworld/components/InputComponent.hpp"
#include "ecs/overworld/components/MapComponent.hpp"
#include "ecs/overworld/components/MovementComponent.hpp"
#include "ecs/overworld/components/PositionComponent.hpp"
#include "ecs/core/query/Query.hpp"
#include "ecs/overworld/systems/MapLookup.hpp"
#include "ecs/overworld/systems/TileCollision.hpp"

#include <cmath>

namespace {
    sf::Vector2f NormalizeMovementDirection(sf::Vector2f movementDirection) {
        const float movementDirectionLength = std::sqrt(movementDirection.x * movementDirection.x +
                                                        movementDirection.y * movementDirection.y);

        if (movementDirectionLength > 1.0f) {
            movementDirection /= movementDirectionLength;
        }

        return movementDirection;
    }

    sf::Vector2f CalculateMovementStep(const sf::Vector2f& movementDirection,
                                       float movementSpeedInPixelsPerSecond, float deltaTime) {
        return movementDirection * movementSpeedInPixelsPerSecond * deltaTime;
    }

    void ApplyHorizontalMovement(const MapComponent& map, sf::Vector2f& entityPosition,
                                 const sf::Vector2f& movementStep,
                                 const CollisionComponent& collisionComponent) {
        const sf::Vector2f horizontalPosition(entityPosition.x + movementStep.x, entityPosition.y);

        if (!TileCollision::IsBlocked(map, horizontalPosition, collisionComponent)) {
            entityPosition.x += movementStep.x;
        }
    }

    void ApplyVerticalMovement(const MapComponent& map, sf::Vector2f& entityPosition,
                               const sf::Vector2f& movementStep,
                               const CollisionComponent& collisionComponent) {
        const sf::Vector2f verticalPosition(entityPosition.x, entityPosition.y + movementStep.y);

        if (!TileCollision::IsBlocked(map, verticalPosition, collisionComponent)) {
            entityPosition.y += movementStep.y;
        }
    }

    void ApplyEntityMovement(const MapComponent& map, sf::Vector2f& entityPosition,
                             const sf::Vector2f& movementStep,
                             const CollisionComponent& collisionComponent) {
        ApplyHorizontalMovement(map, entityPosition, movementStep, collisionComponent);

        ApplyVerticalMovement(map, entityPosition, movementStep, collisionComponent);
    }
}

void MovementSystem::Update(Engine::ECS::SystemContext& context, float deltaTime) {
    const MapComponent* map = MapLookup::FindMap(context);

    if (map == nullptr) {
        return;
    }

    auto movableEntities =
        context.CreateQuery<Engine::ECS::Including<InputComponent, PositionComponent,
                                                   MovementComponent, CollisionComponent>>();

    movableEntities.ForEach([&](Engine::ECS::Entity, InputComponent& inputComponent,
                                PositionComponent& positionComponent,
                                MovementComponent& movementComponent,
                                CollisionComponent& collisionComponent) {
        const sf::Vector2f movementDirection =
            NormalizeMovementDirection(inputComponent.walkDirection);

        const sf::Vector2f movementStep = CalculateMovementStep(
            movementDirection, movementComponent.speedInPixelsPerSecond, deltaTime);

        ApplyEntityMovement(*map, positionComponent.position, movementStep, collisionComponent);
    });
}
