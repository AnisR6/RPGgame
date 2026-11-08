#include "game_logic/systems/core/animation_system/AnimationSystem.hpp"

void AnimationSystem::faceWalkDirection(SpriteComponent &sprite, const MovementComponent &movement)
{
	const sf::Vector2f walkDirection = movement.walkDirection;

	// Walking diagonally: left/right wins over up/down, because x is checked first
	if (walkDirection.x < 0.0f) {
		sprite.currentFrameNumber = sprite.standingLeftFrame;
	} else if (walkDirection.x > 0.0f) {
		sprite.currentFrameNumber = sprite.standingRightFrame;
	} else if (walkDirection.y < 0.0f) {
		sprite.currentFrameNumber = sprite.standingUpFrame;
	} else if (walkDirection.y > 0.0f) {
		sprite.currentFrameNumber = sprite.standingDownFrame;
	}
}