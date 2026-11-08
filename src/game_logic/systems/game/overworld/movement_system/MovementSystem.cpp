#include "game_logic/systems/game/overworld/movement_system/MovementSystem.hpp"

#include "game_logic/systems/game/overworld/collision_system/CollisionSystem.hpp"

#include <SFML/Window/Keyboard.hpp>

sf::Vector2f MovementSystem::readArrowKeys()
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

void MovementSystem::move(PositionComponent &position, const MovementComponent &movement,
                          const CollisionComponent &collision, const TileMap &tileMap, float secondsSinceLastFrame)
{
	const sf::Vector2f step = movement.walkDirection * movement.speedInPixelsPerSecond * secondsSinceLastFrame;

	// First x, then y
	const sf::Vector2f positionAfterXMove(position.position.x + step.x, position.position.y);
	if (!CollisionSystem::isBlocked(positionAfterXMove, collision, tileMap)) {
		position.position = positionAfterXMove;
	}

	const sf::Vector2f positionAfterYMove(position.position.x, position.position.y + step.y);
	if (!CollisionSystem::isBlocked(positionAfterYMove, collision, tileMap)) {
		position.position = positionAfterYMove;
	}
}