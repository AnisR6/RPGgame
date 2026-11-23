#pragma once

#include <SFML/System/Vector2.hpp>

struct InputComponent {
        sf::Vector2f walkDirection;
        bool interactJustPressed = false;
};
