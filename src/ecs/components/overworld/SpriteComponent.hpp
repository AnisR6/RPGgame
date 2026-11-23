#pragma once

#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>


// The sprite sheet and which of its frames is shown right now
struct SpriteComponent {
	const sf::Texture* spriteSheet = nullptr;
	int frameWidthInPixels = 32;
	int frameHeightInPixels = 48;
	int framesPerRow = 4;
	int currentFrameNumber = 1;

	sf::Vector2f drawOffset;

	// Which frame to show for each direction (frames are numbered 0 to 15, like tiles)
	int standingDownFrame = 1;
	int standingRightFrame = 5;
	int standingLeftFrame = 10;
	int standingUpFrame = 13;

	// the walk animation
	int facingStandingFrame = 1;       // the standing frame of the direction we look at now
	int walkStep = 0;                  // which of the 4 frames of the row
	float secondsOnThisStep = 0.0f;    // how long this frame has been shown
	float secondsPerStep = 0.15f;      // how long one frame stays on screen
};
