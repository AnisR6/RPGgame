#include "presentation/widgets/menu/MenuText.hpp"

MenuText::MenuText(std::string value, unsigned int characterSize, sf::Color fillColor, FontId fontId)
    : fontId(fontId), value(std::move(value)), characterSize(characterSize), fillColor(fillColor)
{
}

void MenuText::setFont(const sf::Font &resolvedFont)
{
	font = &resolvedFont;
}

FontId MenuText::getFontId() const noexcept
{
	return fontId;
}

bool MenuText::hasFont() const noexcept
{
	return font != nullptr;
}

sf::Vector2f MenuText::getSize() const
{
	if (!font) {
		throw std::logic_error("MenuText::getSize requires a font bound by the Renderer");
	}

	const sf::Text drawable(*font, value, characterSize);

	const auto bounds = drawable.getLocalBounds();

	return {bounds.size.x, bounds.size.y};
}

void MenuText::draw(sf::RenderTarget &target, sf::Vector2f position)
{
	if (!font) {
		throw std::logic_error("MenuText::draw requires a font bound by the Renderer");
	}

	sf::Text drawable(*font, value, characterSize);

	drawable.setFillColor(fillColor);
	drawable.setPosition(position);

	target.draw(drawable);
}
