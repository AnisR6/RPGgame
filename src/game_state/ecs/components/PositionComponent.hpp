#pragma once

#include <SFML/System/Vector2.hpp>

// Where an entity is: top-left corner of its collision box, in pixels
struct PositionComponent {
	sf::Vector2f position;
};