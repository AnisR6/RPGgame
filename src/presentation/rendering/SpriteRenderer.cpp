#include "presentation/rendering/SpriteRenderer.hpp"

#include <SFML/Graphics/Sprite.hpp>

void SpriteRenderer::draw(sf::RenderTarget &target, const SpriteComponent &sprite, const PositionComponent &position)
{
	sf::Sprite frameSprite(sprite.spriteSheet, getFrameRectangle(sprite, sprite.currentFrameNumber));
	frameSprite.setPosition(position.position);
	target.draw(frameSprite);
}

sf::IntRect SpriteRenderer::getFrameRectangle(const SpriteComponent &sprite, int frameNumber)
{
	const int column = frameNumber % sprite.framesPerRow;
	const int row = frameNumber / sprite.framesPerRow;

	const sf::Vector2i topLeftCorner(column * sprite.frameWidthInPixels, row * sprite.frameHeightInPixels);
	const sf::Vector2i frameSize(sprite.frameWidthInPixels, sprite.frameHeightInPixels);
	return sf::IntRect(topLeftCorner, frameSize);
}