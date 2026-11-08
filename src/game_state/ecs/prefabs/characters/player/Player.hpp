#pragma once

#include <SFML/Graphics.hpp>
#include <string>


#include "game_state/ecs/components/CollisionComponent.hpp"
#include "game_state/ecs/components/MovementComponent.hpp"
#include "game_state/ecs/components/PositionComponent.hpp"
#include "game_state/ecs/components/SpriteComponent.hpp"
#include "game_state/world/TileMap.hpp"

class Player {
  public:
	Player(sf::Vector2f startPosition, const std::string &spriteSheetPath);

	// The player's data, as components
	PositionComponent position;
	CollisionComponent collision;
	MovementComponent movement;
	SpriteComponent sprite;

};