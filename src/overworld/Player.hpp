#pragma once

#include <SFML/Graphics.hpp>
#include <string>

#include "overworld/TileMap.hpp"

class Player {
  public:
	Player(sf::Vector2f startPosition, const std::string &spriteSheetPath);

	void update(float secondsSinceLastFrame, const TileMap &tileMap); // moves the player
	void render(sf::RenderTarget &target) const;                      // draws the player

  private:
	sf::Vector2f readArrowKeys() const;
	void updateFacingRow(sf::Vector2f walkDirection);
	void moveWithCollision(sf::Vector2f movement, const TileMap &tileMap);
	bool touchesSolidTile(sf::Vector2f boxPosition, const TileMap &tileMap) const;

	void drawCharacter(sf::RenderTarget &target) const;
	sf::IntRect getFrameRectangle(int frameNumber) const;

	sf::Vector2f boxPosition_; 
	float boxSizeInPixels_ = 24.0f;
	float speedInPixelsPerSecond_ = 120.0f;

	// Sprite sheet: 4 rows x 4 columns
	sf::Texture spriteSheet_;
	int frameWidthInPixels_ = 32;
	int frameHeightInPixels_ = 48;
	int framesPerRow_ = 4; 
	int currentFrameNumber_ = 1;
};