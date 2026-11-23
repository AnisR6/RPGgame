#pragma once

#include <SFML/Graphics/Texture.hpp>
#include <string>

#include "ecs/components/overworld/CollisionComponent.hpp"
#include "ecs/components/overworld/MovementComponent.hpp"
#include "ecs/components/overworld/PositionComponent.hpp"
#include "ecs/components/overworld/SpriteComponent.hpp"

namespace Prefabs {
class OverworldPlayer {
  public:
	OverworldPlayer(sf::Vector2f startPosition, const std::string &spriteSheetPath);

	PositionComponent position;
	CollisionComponent collision;
	MovementComponent movement;
	SpriteComponent sprite;

  private:
	sf::Texture spriteTexture;
};
} // namespace Prefabs
