#include "presentation/screens/overworld_screen/OverworldScreen.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/View.hpp>

namespace {
const char *mapFilePath = "assets/maps/overworld1.json";
const char *tilesetFolderPath = "assets/tilesets";
const char *playerSpriteSheetPath = "assets/tilesets/Player.png";
const sf::Vector2f playerStartPosition(640.0f, 576.0f);
} // namespace

OverworldScreen::OverworldScreen()
    : systems(assets, mapFilePath, tilesetFolderPath, playerStartPosition, playerSpriteSheetPath)
{
}

void OverworldScreen::update(float secondsSinceLastFrame)
{
	world.UpdateSystems(systems, secondsSinceLastFrame);
	presentation.sync(world);
}

void OverworldScreen::render(sf::RenderTarget &target) const
{
	sf::View camera = target.getDefaultView();
	if (presentation.cameraCenter()) {
		camera.setCenter(*presentation.cameraCenter());
	}

	target.setView(camera);
	presentation.drawEntities(target);

	target.setView(target.getDefaultView());
	presentation.drawWidgetLayers(target);
}

void OverworldScreen::addWidgetLayer(int zIndex, std::unique_ptr<Widget> root)
{
	presentation.addWidgetLayer(zIndex, std::move(root));
}
