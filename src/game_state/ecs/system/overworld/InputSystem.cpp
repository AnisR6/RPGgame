#include "game_state/ecs/system/overworld/InputSystem.hpp"

#include "game_state/ecs/components/InputComponent.hpp"
#include "game_state/ecs/query/Query.hpp"

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
        auto controlled = context.CreateQuery<Including<InputComponent>>();

        controlled.ForEach([](Entity, InputComponent& input) { input.walkDirection = readArrowKeys(); });
    }

} // namespace Engine::ECS
