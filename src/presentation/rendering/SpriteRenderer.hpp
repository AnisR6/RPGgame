#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "game_state/ecs/components/PositionComponent.hpp"
#include "game_state/ecs/components/SpriteComponent.hpp"

// Draws an entity's current frame from its sprite sheet
class SpriteRenderer {
  public:
	static void draw(sf::RenderTarget &target, const SpriteComponent &sprite, const PositionComponent &position);

  private:
	// Frame number -> rectangle in the sprite sheet
	static sf::IntRect getFrameRectangle(const SpriteComponent &sprite, int frameNumber);
};