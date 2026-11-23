#include "presentation/shared/rendering/WidgetRenderer.hpp"

WidgetRenderer::WidgetRenderer(const AssetManager& assets)
    : assets(assets) {}

void WidgetRenderer::Draw(sf::RenderTarget& target, Widget& rootWidget, sf::Vector2f position) {
    rootWidget.BindFonts(assets);
    rootWidget.Draw(target, position);
}
