#include "ecs/overworld/systems/InputSystem.hpp"

#include "ecs/overworld/components/InputComponent.hpp"
#include "ecs/core/query/Query.hpp"

#include <SFML/Window/Keyboard.hpp>

namespace {
    sf::Vector2f ReadMovementDirectionFromArrowKeys() {
        sf::Vector2f movementDirection(0.0f, 0.0f);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
            movementDirection.x += 1.0f;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
            movementDirection.x -= 1.0f;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
            movementDirection.y += 1.0f;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
            movementDirection.y -= 1.0f;
        }

        return movementDirection;
    }
}

void InputSystem::Update(Engine::ECS::SystemContext& context, float) {
    const bool isInteractKeyDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E);

    const bool interactKeyJustPressed = isInteractKeyDown && !wasInteractKeyDown;

    wasInteractKeyDown = isInteractKeyDown;

    auto controlledEntities = context.CreateQuery<Engine::ECS::Including<InputComponent>>();

    controlledEntities.ForEach([&](Engine::ECS::Entity, InputComponent& inputComponent) {
        inputComponent.walkDirection = ReadMovementDirectionFromArrowKeys();
        inputComponent.interactJustPressed = interactKeyJustPressed;
    });
}
