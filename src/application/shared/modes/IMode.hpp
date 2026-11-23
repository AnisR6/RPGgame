#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

class IMode {
    public:
        virtual ~IMode() = default;

        virtual void OnEnter() {}
        virtual void OnExit() {}

        virtual bool HandleEvent(const sf::Event&) { return false; }
        virtual void Update(float) {}
        virtual void Render(sf::RenderWindow& window) = 0;
};
