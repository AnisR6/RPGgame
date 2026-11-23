#pragma once

#include "ecs/components/overworld/CollisionComponent.hpp"

#include <SFML/System/Vector2.hpp>

struct TileMapData;

namespace Engine::ECS {

    class CollisionSystem {
    public:
        // answers if this collision box touch a wall of the map
        static bool isBlocked(const TileMapData& tileMap, sf::Vector2f boxPosition,
                              const CollisionComponent& collision);
    };

}