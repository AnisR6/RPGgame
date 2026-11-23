#include "presentation/shared/widgets/menu/MenuText.hpp"

MenuText::MenuText(std::string value, unsigned int characterSize, sf::Color fillColor,
                   FontId fontId)
    : fontId(fontId),
      value(std::move(value)),
      characterSize(characterSize),
      fillColor(fillColor) {}

void MenuText::BindFonts(const AssetManager& assets) {
    font = &assets.GetFont(fontId);
}

sf::Vector2f MenuText::GetSize() const {
    if (!font) {
        throw std::logic_error("MenuText::GetSize requires a font bound by the Renderer");
    }

    const sf::Text drawable(*font, value, characterSize);

    const auto bounds = drawable.getLocalBounds();

    return {bounds.size.x, bounds.size.y};
}

void MenuText::Draw(sf::RenderTarget& target, sf::Vector2f position) {
    if (!font) {
        throw std::logic_error("MenuText::Draw requires a font bound by the Renderer");
    }

    sf::Text drawable(*font, value, characterSize);

    drawable.setFillColor(fillColor);
    drawable.setPosition(position);

    target.draw(drawable);
}
