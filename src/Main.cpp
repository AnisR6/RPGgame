#include "application/bus/BattleMessageBus.hpp"
#include "application/controllers/BattleController.hpp"
#include "ecs/modes/battle_mode/BattleMode.hpp"

#include <SFML/Graphics.hpp>

#include "ecs/system/combat/combat_action/combat_action_executor/CombatActionExecutor.hpp"
#include "presentation/rendering/AssetManager.hpp"
#include "presentation/rendering/Renderer.hpp"
#include "presentation/screens/battle_screen/BattleScreen.hpp"
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
constexpr bool debugStartInBattle = true;
} // namespace

int main()
{
	sf::RenderWindow window(sf::VideoMode({window_width, window_height}), window_title);
	window.setFramerateLimit(framerate_limit);

	AssetManager assets;
	assets.LoadFont(FontId::Geneva, geneva_font_path);

	Renderer renderer(assets);
	Screen *activeScreen = &MainMenuScreen::getInstance();

	OverworldScreen overworldScreen;

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
			overworldScreen.update(deltaTime);

			window.clear();
			overworldScreen.render(window);
			window.display();
		} else if (debugStartInBattle) {
			battleMode.Update(deltaTime);
			renderer.Render(window, *activeScreen);
		} else {
			renderer.Render(window, *activeScreen);
		}
	}

	return 0;
}
