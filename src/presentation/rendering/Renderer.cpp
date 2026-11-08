#include "presentation/rendering/Renderer.hpp"

#include "presentation/widgets/menu/MenuText.hpp"

Renderer::Renderer(FontManager &fonts, sf::Color clearColor) : fonts(fonts), clearColor(clearColor) {}

void Renderer::bindFonts(Widget &widget)
{
	if (auto *text = dynamic_cast<MenuText *>(&widget)) {
		text->setFont(fonts.getFont(text->getFontId()));
	}

	for (const auto &child : widget.getChildren()) {
		bindFonts(*child);
	}
}

void Renderer::draw(sf::RenderTarget &target, Widget &rootWidget, sf::Vector2f position)
{
	bindFonts(rootWidget);
	rootWidget.draw(target, position);
}

void Renderer::render(sf::RenderWindow &window, Screen &screen, sf::Vector2f position)
{
	window.clear(clearColor);
	draw(window, screen.getRootWidget(), position);
	window.display();
}
