#include <SFML/Graphics.hpp>
#include <optional>

#include "overworld/Player.hpp"
#include "overworld/TileMap.hpp"

// full paths for the moments :/ will fix this later
const char *mapFilePath = "C:/Users/BAB AL SAFA/source/repos/RPGgame/assests/maps/demo_map1.json";
const char *tilesetFolderPath = "C:/Users/BAB AL SAFA/source/repos/RPGgame/assests/tilesets";
const char *playerSpriteSheetPath = "C:/Users/BAB AL SAFA/source/repos/RPGgame/assests/tilesets/Player.png";

int main()
{
	sf::RenderWindow window(sf::VideoMode({960, 640}), "mini RPG demo");
	window.setFramerateLimit(60);

	TileMap tileMap(mapFilePath, tilesetFolderPath);

	Player player(sf::Vector2f(100.0f, 100.0f), playerSpriteSheetPath);

	sf::Clock frameClock;

	while (window.isOpen()) {

		// 1. Input
		while (const std::optional event = window.pollEvent()) {
			if (event->is<sf::Event::Closed>()) {
				window.close();
			}
		}

		// 2. Update
		const float secondsSinceLastFrame = frameClock.restart().asSeconds();
		player.update(secondsSinceLastFrame, tileMap);

		// 3. Draw
		window.clear();
		tileMap.render(window);
		player.render(window);
		window.display();
	}
}