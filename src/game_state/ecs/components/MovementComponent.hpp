#pragma once

#include <SFML/System/Vector2.hpp>

// How fast an entity walks, and which way it wants to walk this frame
struct MovementComponent {
	float speedInPixelsPerSecond = 120.0f;
	sf::Vector2f walkDirection; // x: -1 left, +1 right. y: -1 up, +1 down
};