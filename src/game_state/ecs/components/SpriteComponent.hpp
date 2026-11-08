#pragma once

#include <SFML/Graphics/Texture.hpp>

// The sprite sheet and which of its frames is shown right now
struct SpriteComponent {
	sf::Texture spriteSheet;
	int frameWidthInPixels = 32;
	int frameHeightInPixels = 48;
	int framesPerRow = 4;
	int currentFrameNumber = 1;

	// Which frame to show for each direction (frames are numbered 0 to 15, like tiles)
	int standingDownFrame = 1;
	int standingRightFrame = 5;
	int standingLeftFrame = 10;
	int standingUpFrame = 13;
};
