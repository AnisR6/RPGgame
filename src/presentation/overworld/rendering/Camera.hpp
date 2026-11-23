#pragma once

#include "ecs/core/world/World.hpp"

#include <SFML/Graphics/View.hpp>
#include <SFML/System/Vector2.hpp>

namespace Camera {
    sf::View FollowPlayer(Engine::ECS::World& world, sf::Vector2f viewSize);
}
