#include "presentation/screens/overworld_screen/OverworldScreen.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

namespace {
    const char* mapFilePath = "assets/maps/overworld1.json";
    const char* tilesetFolderPath = "assets/tilesets";
    const char* playerSpriteSheetPath = "assets/tilesets/Player.png";
    const sf::Vector2f playerStartPosition(640.0f, 576.0f);
} // namespace

OverworldScreen::OverworldScreen()
    : tileMap(assets.LoadTileMap(mapFilePath)),
      systems(*tileMap, assets, tilesetFolderPath, playerStartPosition, playerSpriteSheetPath),
      renderer(assets) {
    renderer.SetWorld(world, *tileMap, tilesetFolderPath);
}

void OverworldScreen::update(float secondsSinceLastFrame) {
    world.UpdateSystems(systems, secondsSinceLastFrame);
}

void OverworldScreen::render(sf::RenderTarget& target) {
    const sf::Vector2f viewSize = target.getDefaultView().getSize();
    renderer.RenderLayers(target, renderer.GetCameraCenter(viewSize), viewSize);
}

void OverworldScreen::addWidgetLayer(Renderer::RenderLayer layer, std::unique_ptr<Widget> root) {
    renderer.AddWidgetLayer(layer, std::move(root));
}
