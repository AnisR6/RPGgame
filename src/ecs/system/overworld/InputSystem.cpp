#include "ecs/system/overworld/InputSystem.hpp"

#include "ecs/components/InputComponent.hpp"
#include "ecs/query/Query.hpp"

#include <SFML/Window/Keyboard.hpp>

namespace {
    // -----------------------------------------------------------------------------
    // Keyboard Input
    // -----------------------------------------------------------------------------
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

} // namespace

namespace Engine::ECS {

    // -----------------------------------------------------------------------------
    // System Update
    // -----------------------------------------------------------------------------
    void InputSystem::Update(SystemContext& context, float) {
        const bool isInteractKeyDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E);

        const bool interactKeyJustPressed = isInteractKeyDown && !wasInteractKeyDown;

        wasInteractKeyDown = isInteractKeyDown;

        auto controlledEntities = context.CreateQuery<Including<InputComponent>>();

        controlledEntities.ForEach([&](Entity, InputComponent& inputComponent) {
            inputComponent.walkDirection = ReadMovementDirectionFromArrowKeys();
            inputComponent.interactJustPressed = interactKeyJustPressed;
        });
    }
} // namespace Engine::ECS