#include <SFML/Graphics.hpp>

#include "presentation/rendering/FontManager.hpp"
#include "presentation/rendering/Renderer.hpp"
#include "presentation/screens/boot_screen/MainMenuScreen.hpp"
#include "presentation/screens/core/Screen.hpp"
#include "presentation/screens/overworld_screen/OverworldScreen.hpp"

#include <optional>

namespace {
constexpr unsigned window_width = 1280;
constexpr unsigned window_height = 720;
constexpr unsigned framerate_limit = 60;
const char *window_title = "Contract";
const char *geneva_font_path = "assets/fonts/Geneva.ttf";
constexpr bool debugStartInOverworld = false;
} // namespace

int main()
{
	sf::RenderWindow window(sf::VideoMode({window_width, window_height}), window_title);
	window.setFramerateLimit(framerate_limit);

	FontManager fonts;
	fonts.loadFont(FontId::Geneva, geneva_font_path);

	Renderer renderer(fonts);
	Screen &activeScreen = MainMenuScreen::getInstance();

	OverworldScreen overworldScreen;
	sf::Clock frameClock;

	while (window.isOpen()) {
		while (const std::optional event = window.pollEvent()) {
			if (event->is<sf::Event::Closed>()) {
				window.close();
			}
		}
		if (debugStartInOverworld) {
			const float secondsSinceLastFrame = frameClock.restart().asSeconds();
			overworldScreen.update(secondsSinceLastFrame);

			window.clear();
			overworldScreen.render(window);
			window.display();
		} else {
			renderer.render(window, activeScreen);
		}
	}

	return 0;
}
