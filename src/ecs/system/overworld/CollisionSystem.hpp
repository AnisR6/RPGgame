#pragma once

#include "ecs/components/CollisionComponent.hpp"

#include <SFML/System/Vector2.hpp>

class TileMap;

namespace Engine::ECS {

    class CollisionSystem {
    public:
        // answers if this collision box touch a wall of the map
        static bool isBlocked(const TileMap& tileMap, sf::Vector2f boxPosition, const CollisionComponent& collision);
    };

}