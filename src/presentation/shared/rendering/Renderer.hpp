#pragma once

#include "ecs/core/world/World.hpp"
#include "presentation/shared/rendering/AssetManager.hpp"
#include "presentation/shared/rendering/WidgetRenderer.hpp"
#include "presentation/shared/rendering/WorldRenderer.hpp"
#include "presentation/shared/screens/Screen.hpp"
#include "presentation/shared/widgets/core/Widget.hpp"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>

class Renderer {
    public:
        explicit Renderer(const AssetManager& assets, sf::Color clearColor = sf::Color{8, 10, 16});

        void Render(sf::RenderWindow& window, Screen& screen, sf::Vector2f position = {});

        void DrawWorld(sf::RenderTarget& target, Engine::ECS::World& world);

        void DrawWidget(sf::RenderTarget& target, Widget& rootWidget, sf::Vector2f position = {});

    private:
        WidgetRenderer widgetRenderer;
        WorldRenderer worldRenderer;
        sf::Color clearColor;
};
