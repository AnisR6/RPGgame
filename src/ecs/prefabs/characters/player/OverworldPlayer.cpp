#include "ecs/prefabs/characters/player/OverworldPlayer.hpp"

#include <iostream>

Prefabs::OverworldPlayer::OverworldPlayer(sf::Vector2f startPosition, const std::string &spriteSheetPath)
{
	position.position = startPosition;

	if (!spriteTexture.loadFromFile(spriteSheetPath)) {
		std::cout << "Could not load " << spriteSheetPath << "\n";
	}
	sprite.spriteSheet = &spriteTexture;
}
