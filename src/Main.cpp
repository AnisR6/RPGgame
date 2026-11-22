#include "application/bus/BattleMessageBus.hpp"
#include "application/controllers/BattleController.hpp"
#include "game_logic/modes/game_mode/battle_mode/BattleMode.hpp"

#include <SFML/Graphics.hpp>

#include "game_logic/modes/game_mode/overworld_mode/OverworldMode.hpp"
#include "game_state/ecs/system/combat/combat_action/combat_action_executor/CombatActionExecutor.hpp"
#include "presentation/rendering/FontManager.hpp"
#include "presentation/rendering/Renderer.hpp"
#include "presentation/screens/battle_screen/BattleScreen.hpp"
#include "presentation/screens/boot_screen/MainMenuScreen.hpp"
#include "presentation/screens/core/Screen.hpp"

#include <optional>

namespace {
constexpr unsigned window_width = 1280;
constexpr unsigned window_height = 720;
constexpr unsigned framerate_limit = 60;
const char *window_title = "Contract";
const char *geneva_font_path = "assets/fonts/Geneva.ttf";
constexpr bool debugStartInOverworld = false;
constexpr bool debugStartInBattle = true;
} // namespace

int main()
{
	sf::RenderWindow window(sf::VideoMode({window_width, window_height}), window_title);
	window.setFramerateLimit(framerate_limit);

	FontManager fonts;
	fonts.loadFont(FontId::Geneva, geneva_font_path);

	Renderer renderer(fonts);
	Screen *activeScreen = &MainMenuScreen::getInstance();

	OverworldMode overworldMode;

	Engine::ECS::World world;
	BattleMessageBus battleMessageBus;
	CombatActionExecutor executor{world};
	Engine::ECS::BattleMode battleMode{battleMessageBus, world, {}, executor};
	BattleController battleController{battleMessageBus, world};

	battleMode.startBattle();
	battleController.requestBattleInfo();

	sf::Clock frameClock;

	if (debugStartInBattle) {
		activeScreen = &BattleScreen::getInstance();
	}

	while (window.isOpen()) {
		while (const auto event = window.pollEvent()) {
			if (event->is<sf::Event::Closed>()) {
				window.close();
				continue;
			}
			activeScreen->handleEvent(*event);
		}

		const float deltaTime = frameClock.restart().asSeconds();
		if (debugStartInOverworld) {
			overworldMode.update(deltaTime);

			window.clear();
			overworldMode.render(window);
			window.display();
		} else if (debugStartInBattle) {
			battleMode.Update(deltaTime);
			renderer.render(window, *activeScreen);
		} else {
			renderer.render(window, *activeScreen);
		}
	}

	return 0;
}
