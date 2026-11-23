#pragma once

#include <SFML/System/Vector2.hpp>

// How fast an entity walks
struct MovementComponent {
	float speedInPixelsPerSecond = 120.0f;
};