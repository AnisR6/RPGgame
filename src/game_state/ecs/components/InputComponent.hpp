#pragma once

#include <SFML/System/Vector2.hpp>

// Which way an entity wants to walk this frame. Written by InputSystem from the controls.
struct InputComponent {
	sf::Vector2f walkDirection; // x: -1 left, +1 right. y: -1 up, +1 down
};
