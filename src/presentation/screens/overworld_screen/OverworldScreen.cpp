#include "presentation/screens/overworld_screen/OverworldScreen.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/View.hpp>

namespace {
	const char* mapFilePath = "assets/maps/overworld1.json";
	const char* tilesetFolderPath = "assets/tilesets";
	const char* playerSpriteSheetPath = "assets/tilesets/Player.png";
	const sf::Vector2f playerStartPosition(640.0f, 576.0f);
}
	OverworldScreen::OverworldScreen()
		: tileMap(mapFilePath),
		systems(tileMap, assets, tilesetFolderPath, playerStartPosition, playerSpriteSheetPath),
		presentation(tileMap, world, assets, tilesetFolderPath)
	{
	}

		void OverworldScreen::update(float secondsSinceLastFrame)
		{
			world.UpdateSystems(systems, secondsSinceLastFrame);
		}

		void OverworldScreen::render(sf::RenderTarget & target) const
		{
			sf::View camera = target.getDefaultView();
			camera.setCenter(presentation.cameraCenter(camera.getSize()));

			target.setView(camera);
			presentation.drawWorld(target);

			target.setView(target.getDefaultView());
			presentation.drawWidgetLayers(target);
		}

		void OverworldScreen::addWidgetLayer(int zIndex, std::unique_ptr<Widget> root)
		{
			presentation.addWidgetLayer(zIndex, std::move(root));
		}