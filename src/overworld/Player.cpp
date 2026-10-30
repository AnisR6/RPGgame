#include "overworld/Player.hpp"

#include <iostream>

// Frames of the player mouvement directions
const int standingDownFrame = 1;
const int standingRightFrame = 5;
const int standingLeftFrame = 10;
const int standingUpFrame = 13;

// Column of the sheet with the standing pose
const int standingColumn = 1;

Player::Player(sf::Vector2f startPosition, const std::string &spriteSheetPath)
{
	boxPosition_ = startPosition;

	if (!spriteSheet_.loadFromFile(spriteSheetPath)) {
		std::cout << "Could not load " << spriteSheetPath << "\n";
	}
}

void Player::update(float secondsSinceLastFrame, const TileMap &tileMap)
{
	const sf::Vector2f walkDirection = readArrowKeys();
	updateFacingRow(walkDirection);

	const sf::Vector2f movement = walkDirection * speedInPixelsPerSecond_ * secondsSinceLastFrame;
	moveWithCollision(movement, tileMap);
}

void Player::render(sf::RenderTarget &target) const
{
	drawCharacter(target);
}

// Which way does the player want to walk? x: -1 left, +1 right. y: -1 up, +1 down.
sf::Vector2f Player::readArrowKeys() const
{
	sf::Vector2f walkDirection(0.0f, 0.0f);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
		walkDirection.x += 1.0f;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
		walkDirection.x -= 1.0f;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
		walkDirection.y += 1.0f;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
		walkDirection.y -= 1.0f;
	}

	return walkDirection;
}

// Look where you walk. Standing still, keep looking the same way.
void Player::updateFacingRow(sf::Vector2f walkDirection)
{
	if (walkDirection.x < 0.0f) { // if he walks diagonally, we prefer left/right over up/down, thats why we use it by
		                          // else if statements here
		currentFrameNumber_ = standingLeftFrame;
	} else if (walkDirection.x > 0.0f) {
		currentFrameNumber_ = standingRightFrame;
	} else if (walkDirection.y < 0.0f) {
		currentFrameNumber_ = standingUpFrame;
	} else if (walkDirection.y > 0.0f) {
		currentFrameNumber_ = standingDownFrame;
	}
}

// Move on x first, then on y.

void Player::moveWithCollision(sf::Vector2f movement, const TileMap &tileMap)
{
	const sf::Vector2f positionAfterXMove(boxPosition_.x + movement.x, boxPosition_.y);
	if (!touchesSolidTile(positionAfterXMove, tileMap)) {
		boxPosition_ = positionAfterXMove;
	}

	const sf::Vector2f positionAfterYMove(boxPosition_.x, boxPosition_.y + movement.y);
	if (!touchesSolidTile(positionAfterYMove, tileMap)) {
		boxPosition_ = positionAfterYMove;
	}
}

// check if would the box at this position touch a wall,  we check its 4 corners
bool Player::touchesSolidTile(sf::Vector2f boxPosition, const TileMap &tileMap) const
{
	const float tileSize = static_cast<float>(tileMap.getTileSizeInPixels());

	const int leftTileX = static_cast<int>(boxPosition.x / tileSize);
	const int rightTileX = static_cast<int>((boxPosition.x + boxSizeInPixels_ - 1.0f) / tileSize);
	const int topTileY = static_cast<int>(boxPosition.y / tileSize);
	const int bottomTileY = static_cast<int>((boxPosition.y + boxSizeInPixels_ - 1.0f) / tileSize);

	return tileMap.isSolidTile(leftTileX, topTileY) || tileMap.isSolidTile(rightTileX, topTileY)
	       || tileMap.isSolidTile(leftTileX, bottomTileY) || tileMap.isSolidTile(rightTileX, bottomTileY);
}

// get the frame number from the sprites
sf::IntRect Player::getFrameRectangle(int frameNumber) const
{
	const int column = frameNumber % framesPerRow_;
	const int row = frameNumber / framesPerRow_;

	const sf::Vector2i topLeftCorner(column * frameWidthInPixels_, row * frameHeightInPixels_);
	const sf::Vector2i frameSize(frameWidthInPixels_, frameHeightInPixels_);
	return sf::IntRect(topLeftCorner, frameSize);
}


// draw the current frame at the plaers position
void Player::drawCharacter(sf::RenderTarget &target) const
{
	sf::Sprite characterSprite(spriteSheet_, getFrameRectangle(currentFrameNumber_));
	
    characterSprite.setPosition(boxPosition_);
	target.draw(characterSprite);
}

