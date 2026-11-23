#include "ecs/system/overworld/InputSystem.hpp"

#include "ecs/components/InputComponent.hpp"
#include "ecs/query/Query.hpp"

#include <SFML/Window/Keyboard.hpp>

namespace {
    sf::Vector2f readArrowKeys()
    {
        sf::Vector2f walkDirection(0.0f, 0.0f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
            walkDirection.x += 1.0f;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
            walkDirection.x -= 1.0f;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
            walkDirection.y += 1.0f;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
            walkDirection.y -= 1.0f;
        }
        return walkDirection;
    }
} // namespace

namespace Engine::ECS {

    void InputSystem::Update(SystemContext& context, float)
    {
        // E: only the frame it goes down counts, not while it is held
        const bool isInteractKeyDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E);
        const bool interactJustPressed = isInteractKeyDown && !wasInteractKeyDown;
        wasInteractKeyDown = isInteractKeyDown;

        auto controlled = context.CreateQuery<Including<InputComponent>>();
        controlled.ForEach([&](Entity, InputComponent& input) {
            input.walkDirection = readArrowKeys();
            input.interactJustPressed = interactJustPressed;
        });
    }

} // namespace Engine::ECS
