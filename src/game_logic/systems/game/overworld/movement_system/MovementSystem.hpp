#pragma once

#include <SFML/System/Vector2.hpp>

#include "game_state/ecs/components/CollisionComponent.hpp"
#include "game_state/ecs/components/MovementComponent.hpp"
#include "game_state/ecs/components/PositionComponent.hpp"
#include "game_state/world/TileMap.hpp"

// Moves entities, IT reads where they want to go, and moves them unless a wall is in the way
class MovementSystem {
  public:
	// Which way does the player want to walk? x: -1 left, +1 right. y: -1 up, +1 down.
	static sf::Vector2f readArrowKeys();

	// Move by walkDirection * speed * time. Blocked directions are skipped, so it slides along walls.
	static void move(PositionComponent &position, const MovementComponent &movement,
	                 const CollisionComponent &collision, const TileMap &tileMap, float secondsSinceLastFrame);
};