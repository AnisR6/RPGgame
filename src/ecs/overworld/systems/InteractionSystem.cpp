#include "ecs/overworld/systems/InteractionSystem.hpp"

#include "ecs/core/query/Query.hpp"
#include "ecs/overworld/components/CollisionComponent.hpp"
#include "ecs/overworld/components/EncounterComponent.hpp"
#include "ecs/overworld/components/InputComponent.hpp"
#include "ecs/overworld/components/PositionComponent.hpp"

#include <optional>

namespace {
    constexpr float InteractionRangeInPixels = 40.0f;

    sf::Vector2f GetBoxCenter(const PositionComponent& position,
                              const CollisionComponent& collision) {
        const float halfBox = collision.boxSizeInPixels / 2.0f;
        return position.position + sf::Vector2f(halfBox, halfBox);
    }
}

InteractionSystem::InteractionSystem(OverworldState& overworldState)
    : overworldState(overworldState) {}

void InteractionSystem::Update(Engine::ECS::SystemContext& context, float) {
    std::optional<sf::Vector2f> interactingCenter;

    auto controlledEntities = context.CreateQuery<
        Engine::ECS::Including<InputComponent, PositionComponent, CollisionComponent>>();

    controlledEntities.ForEach([&](Engine::ECS::Entity, InputComponent& input,
                                   PositionComponent& position, CollisionComponent& collision) {
        if (input.interactJustPressed) {
            interactingCenter = GetBoxCenter(position, collision);
        }
    });

    if (!interactingCenter) {
        return;
    }

    float closestDistance = InteractionRangeInPixels;

    auto encounters = context.CreateQuery<
        Engine::ECS::Including<EncounterComponent, PositionComponent, CollisionComponent>>();

    encounters.ForEach([&](Engine::ECS::Entity entity, EncounterComponent&,
                           PositionComponent& position, CollisionComponent& collision) {
        const float distance = (GetBoxCenter(position, collision) - *interactingCenter).length();

        if (distance <= closestDistance) {
            closestDistance = distance;
            overworldState.encounteredEnemy = entity;
        }
    });
}
