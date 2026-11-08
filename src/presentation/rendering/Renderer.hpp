#pragma once

#include "presentation/rendering/FontManager.hpp"
#include "presentation/screens/core/Screen.hpp"
#include "presentation/widgets/core/Widget.hpp"

#include <SFML/Graphics.hpp>

class Renderer {
  private:
	FontManager &fonts;
	sf::Color clearColor;

	void bindFonts(Widget &widget);

  public:
	explicit Renderer(FontManager &fonts, sf::Color clearColor = sf::Color{8, 10, 16});

	void render(sf::RenderWindow &window, Screen &screen, sf::Vector2f position = {});

	void draw(sf::RenderTarget &target, Widget &rootWidget, sf::Vector2f position = {});
};
