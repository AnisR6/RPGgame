#pragma once

#include <SFML/Graphics.hpp>

#include "game_state/ecs/prefabs/characters/player/Player.hpp"
#include "game_state/world/TileMap.hpp"

// The overworld: the map and the player walking on it
class OverworldMode {
  public:
	OverworldMode();

	void update(float secondsSinceLastFrame);
	void render(sf::RenderTarget &target) const;

  private:
	TileMap tileMap_;
	Player player_;
};