#pragma once

#include <SFML/System/Vector2.hpp>

#include "game_state/ecs/components/CollisionComponent.hpp"
#include "game_state/world/TileMap.hpp"

// Answers one question would a collision box at this position touch a solid tile?
class CollisionSystem {
  public:
	static bool isBlocked(sf::Vector2f boxPosition, const CollisionComponent &collision, const TileMap &tileMap);
};