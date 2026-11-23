#pragma once

#include "ecs/overworld/components/CollisionComponent.hpp"
#include "ecs/overworld/components/MapComponent.hpp"

#include <SFML/System/Vector2.hpp>

namespace TileCollision {
    bool IsBlocked(const MapComponent& map, sf::Vector2f boxPosition,
                   const CollisionComponent& collision);
}
