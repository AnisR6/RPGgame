#pragma once

#include "game_state/ecs/components/MovementComponent.hpp"
#include "game_state/ecs/components/SpriteComponent.hpp"

// Chooses which frame of the sprite sheet to show
class AnimationSystem {
  public:

	// Look where you walk. if standing still, keep the last frame.
	static void faceWalkDirection(SpriteComponent &sprite, const MovementComponent &movement);
};