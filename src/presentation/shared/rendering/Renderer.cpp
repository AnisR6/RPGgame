#include "presentation/shared/rendering/Renderer.hpp"

Renderer::Renderer(const AssetManager& assets, sf::Color clearColor)
    : widgetRenderer(assets),
      clearColor(clearColor) {}

void Renderer::Render(sf::RenderWindow& window, Screen& screen, sf::Vector2f position) {
    window.clear(clearColor);
    widgetRenderer.Draw(window, screen.GetRootWidget(), position);
    window.display();
}

void Renderer::DrawWorld(sf::RenderTarget& target, Engine::ECS::World& world) {
    worldRenderer.Draw(target, world);
}

void Renderer::DrawWidget(sf::RenderTarget& target, Widget& rootWidget, sf::Vector2f position) {
    widgetRenderer.Draw(target, rootWidget, position);
}
